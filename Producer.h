//
// Created by Hugo Bedard on 17-02-04.
//

#ifndef FILEREAD_PRODUCER_H
#define FILEREAD_PRODUCER_H

#include "Queue.h"

template<class T>
class Producer {
public:
    using value_type = T;

    Producer() = default;

    T consume()
    {
        return queue_.pop();
    }

    void produce(const T& t)
    {
        queue_.push(t);
    }

    void produce(T&& t)
    {
        queue_.push(std::forward<T>(t));
    }

private:
    Queue<T> queue_;
};

#endif //FILEREAD_PRODUCER_H
