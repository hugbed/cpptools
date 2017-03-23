//
// Created by jon on 22/03/17.
//

#ifndef CPPTOOLS_PROJECT_SYNCCONSUMER_H
#define CPPTOOLS_PROJECT_SYNCCONSUMER_H

#include "sync_producer.h"

#include <mutex>
#include <vector>

template <class T>
class SyncConsumer {
public:
    SyncConsumer(std::mutex &doneProducingMutex, std::condition_variable &shouldConsumeCV, const std::vector<SyncProducer<T>*> producers)
    : doneProducingMutex(doneProducingMutex)
    , shouldConsumeCV(shouldConsumeCV)
    , producers(producers)
    {}

    void startConsuming()
    {
        while (!shouldStopConsuming()) {
            waitToConsume();
            consume();
            notifyDoneConsuming();
        }
    }

    virtual bool shouldStopConsuming()
    {
        return false;
    }

private:
    virtual void consume() = 0; // use producer->product

    bool allAreDoneProducing()
    {
        return std::all_of(std::begin(producers), std::end(producers), [](auto &p) { return p->doneProducingCondition; });
    }

    void waitToConsume()
    {
        // wait until all producers are done producing
        std::unique_lock<std::mutex> lk(doneProducingMutex);
        while (!allAreDoneProducing()) {
            shouldConsumeCV.wait(lk, [this] {
                return allAreDoneProducing();
            });
        }

        // we acknowledged it, set it back to default state
        for (auto& producer : producers)
            producer->doneProducingCondition = false;
    }

    void notifyDoneConsuming()
    {
        for (auto &producer : producers)
        {
            std::unique_lock<std::mutex> lk(producer->doneConsumingMutex);
            producer->doneConsumingCondition = true;
        }
        for (auto &producer : producers)
        {
            producer->shouldProduceCV.notify_one();
        }
    }

    std::mutex &doneProducingMutex;
    std::condition_variable &shouldConsumeCV;

protected:
    std::vector<SyncProducer<T>*> producers;
};


#endif //CPPTOOLS_PROJECT_SYNCCONSUMER_H
