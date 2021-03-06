//
// Created by jon on 22/03/17.
//

#ifndef CPPTOOLS_PROJECT_SYNCPRODUCER_H
#define CPPTOOLS_PROJECT_SYNCPRODUCER_H

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>
#include <algorithm>

template <class T>
class SyncProducer {
public:
    SyncProducer(std::mutex &doneProducingMutex, std::condition_variable &shouldConsumeCV)
        : doneProducingCondition{false}
        , doneConsumingCondition{false}
        , shouldConsumeCV{shouldConsumeCV}
        , doneProducingMutex{doneProducingMutex}
    {}

    virtual const T& getProduct() = 0;

    void startProducing()
    {
        while (!shouldStopProducing())
        {
            produce();
            notifyDoneProducing();
            waitToProduce();
        }
    }

    virtual bool shouldStopProducing()
    {
        return false;
    }

    virtual void produce() = 0; // where the work happens : set product

    void notifyDoneProducing()
    {
        {
            std::unique_lock<std::mutex> lk(doneProducingMutex);
            doneProducingCondition = true;
        }
        shouldConsumeCV.notify_one();
    }

    void waitToProduce()
    {
        // wait until doneConsuming
        std::unique_lock<std::mutex> lk(doneConsumingMutex);
        while (!(doneConsumingCondition)) shouldProduceCV.wait(lk, [this] { return doneConsumingCondition; });

        // we acknowledged it, set it back to default state
        doneConsumingCondition = false;
    }

    bool doneProducingCondition;
    bool doneConsumingCondition;
    std::condition_variable shouldProduceCV;
    std::condition_variable& shouldConsumeCV;
    std::mutex& doneProducingMutex;
    std::mutex doneConsumingMutex;
};


#endif //CPPTOOLS_PROJECT_SYNCPRODUCER_H
