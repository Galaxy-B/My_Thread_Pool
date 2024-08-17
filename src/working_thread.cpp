#include "thread_pool.hpp"

// overload '()' operator to make this class an executed function for the thread
void Working_Thread::operator()()
{
    std::function<void()> task;

    // mark whether this thread received a task from the thread pool
    bool get_task = false;

    // keep working until the thread pool is shut down
    while (!thread_pool->is_shutdown)
    {
        // use unique lock in cooperation with conditional variable to manage all threads
        {
            std::unique_lock<std::mutex> u_lock(thread_pool->lock);

            // if there is no task in the queue, wait for notification
            if (thread_pool->task_queue.empty())
            {
                thread_pool->cv.wait(u_lock);
            }

            // otherwise try to fetch a task from the queue
            get_task = thread_pool->task_queue.dequeue(task);
        }

        // if fetched successfully, perform this task
        if (get_task)
        {
            task();
        }
    }
}