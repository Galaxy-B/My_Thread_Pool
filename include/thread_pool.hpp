#ifndef _THREAD_POOL_HPP_
#define _THREAD_POOL_HPP_

#include "safe_queue.hpp"
#include "working_thread.hpp"

#include <functional>
#include <mutex>
#include <vector>
#include <thread>
#include <condition_variable>
#include <future>

/********************************************************************

* This header file implements the thread pool

*********************************************************************/

class Thread_Pool
{
public:
    // mark whether the thread pool has been off
    bool is_shutdown;

    // queue of submitted tasks
    Safe_Queue<std::function<void()>> task_queue;

    // pre-allocated working threads
    std::vector<std::thread> working_threads;

    // mutex for unique lock
    std::mutex lock;

    // conditional variable for notifying available working thread
    std::condition_variable cv;

public:
    Thread_Pool(int capacity) : working_threads(std::vector<std::thread>(capacity)), is_shutdown(false) {}

    void init()
    {
        // instantiate all threads to set them work
        for (auto& working_thread : working_threads)
        {
            working_thread = std::thread(Working_Thread(this));
        }
    }

    void shutdown()
    {
        is_shutdown = true;
        // wake up all working threads
        cv.notify_all();

        // wait for all working threads to finish their task
        for (auto& working_thread : working_threads)
        {
            if (working_thread.joinable())
            {
                working_thread.join();
            }
        }
    }

    template <typename F, typename... Args>
    auto submit(F&& func, Args&&... args) -> std::future<decltype(func(args...))>
    {
        // bind the function with parameters into a task
        std::function<decltype(func(args...))()> task = std::bind(std::forward<F>(func), std::forward<Args>(args)...);

        // encapsulate the task with a pointer so it can be copy constructed effectively
        auto task_ptr = std::make_shared<std::packaged_task<decltype(func(args...))()>>(task);

        // wrap the task with a void function shell
        std::function<void()> wrapped_task = [task_ptr]()
        {
            (*task_ptr)();
        };

        // push the wrapped task into the queue
        task_queue.enqueue(wrapped_task);

        // wake up a sleeping thread if there is one to perform this task
        cv.notify_one();

        // return the future class of the submitted task
        return task_ptr->get_future();
    }
};

#endif