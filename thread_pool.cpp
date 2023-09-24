#include "thread_pool.h"

ThreadPool::ThreadPool()
    : thread_count_(std::thread::hardware_concurrency() != 0
            ? std::thread::hardware_concurrency()
            : 4)
    , thread_queues_(thread_count_)
{
}

void ThreadPool::start()
{
    for (int i = 0; i < thread_count_; i++) {
        threads_.emplace_back(std::thread(&ThreadPool::threadFunc, this, i));
    }
}

void ThreadPool::stop()
{
    for (int i = 0; i < thread_count_; i++) {
        task_type empty_task;
        thread_queues_[i].push(empty_task);
    }

    for (auto& t : threads_) {
        t.join();
    }
}

void ThreadPool::threadFunc(int qindex)
{
    while (true) {
        task_type task_to_do;
        bool res;
        int i = 0;
        for (; i < thread_count_; ++i) {
            res = thread_queues_[(qindex + i) % thread_count_].fast_pop(
                task_to_do);
            if (res)
                break;
        }

        if (!res) {
            thread_queues_[qindex].pop(task_to_do);
        } else if (!task_to_do) {
            thread_queues_[(qindex + i) % thread_count_].push(task_to_do);
        }

        if (!task_to_do) {
            return;
        }
        task_to_do();
    }
}
