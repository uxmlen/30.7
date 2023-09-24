#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

template <typename T> class BlockedQueue {
public:
    void push(T& item)
    {
        std::lock_guard<std::mutex> l(locker_);

        task_queue_.push(item);
        event_holder_.notify_one();
    }

    bool isEmpty() { return task_queue_.empty(); }

    void pop(T& item)
    {
        std::unique_lock<std::mutex> l(locker_);
        if (task_queue_.empty())
            event_holder_.wait(l, [this] { return !task_queue_.empty(); });

        item = task_queue_.front();
        task_queue_.pop();
    }

    bool fast_pop(T& item)
    {
        std::lock_guard<std::mutex> l(locker_);
        if (task_queue_.empty()) {
            return false;
        }
        item = task_queue_.front();
        task_queue_.pop();
        return true;
    }

private:
    std::mutex locker_;
    std::queue<T> task_queue_;
    std::condition_variable event_holder_;
};
