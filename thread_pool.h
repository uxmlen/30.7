#pragma once

#include <condition_variable>
#include <deque>
#include <functional>
#include <future>
#include <mutex>
#include <thread>
#include <vector>

#include "block_queue.h"

typedef std::function<void()> task_type;

class ThreadPool {
public:
    ThreadPool();

    void threadFunc(int qindex);

    void start();
    void stop();

    template <class Func, class... Args> void push_task(Func f, Args&... args)
    {
        int queue_to_push = index_++ % thread_count_;

        task_type task([=] { f(args...); });

        thread_queues_[queue_to_push].push(task);
    }

private:
    int index_ = 0;
    int thread_count_;

    std::vector<std::thread> threads_;
    std::vector<BlockedQueue<task_type>> thread_queues_;
};
