#pragma once

#include "thread_pool.h"

class RequestHandler {
public:
    RequestHandler() { tpool_.start(); }
    ~RequestHandler() { tpool_.stop(); }

    template <class Func, class... Args> void push_tasks(Func f, Args&... args)
    {
        tpool_.push_task(f, args...);
    }

private:
    ThreadPool tpool_;
};
