//
// Created by Hugo Bedard on 17-01-31.
//

#ifndef FILEREAD_QUEUE_H
#define FILEREAD_QUEUE_H

#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

template<class T>
class Queue {
public:
    T pop()
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        while (queue_.empty()) {
            cond_.wait(mlock);
        }
        auto val = std::move(queue_.front());
        queue_.pop();
        return val;
    }

    void pop(T& item)
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        cond_.wait(mlock, [this]() {
          return queue_.empty();
        });
        item = queue_.front();
        queue_.pop();
    }

    void push(const T& item)
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        queue_.push(item);
        mlock.unlock();
        cond_.notify_one();
    }

    void push(T&& item)
    {
        std::unique_lock<std::mutex> mlock(mutex_);
        queue_.push(std::forward<T>(item));
        mlock.unlock();
        cond_.notify_one();
    }

    Queue() = default;
    Queue(const Queue&) = delete;            // disable copying
    Queue& operator=(const Queue&) = delete; // disable assignment

private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable cond_;
};

#endif //FILEREAD_QUEUE_H
