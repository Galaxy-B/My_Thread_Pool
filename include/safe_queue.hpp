#ifndef _SAFE_QUEUE_HPP_
#define _SAFE_QUEUE_HPP_

#include <queue>
#include <mutex>

/********************************************************************

* This header file implements a queue which is thread-safe

*********************************************************************/

template <typename T>
class Safe_Queue
{
private:
    // maintain the order of submitted tasks
    std::queue<T> task_queue;
    
    // use lock to make our visit to the queue thread-safe
    std::mutex lock;

public:
    Safe_Queue() = default;
    
    bool empty()
    {
        // use lock to make it a atomic operation
        std::unique_lock<std::mutex> u_lock(lock);

        return task_queue.empty();
    }

    int size()
    {
        // use lock to make it a atomic operation
        std::unique_lock<std::mutex> u_lock(lock);

        return task_queue.size();
    }

    void enqueue(T& obj)
    {
        // use lock to make it a atomic operation
        std::unique_lock<std::mutex> u_lock(lock);

        task_queue.emplace(obj);
    }

    bool dequeue(T& var)
    {
        // use lock to make it a atomic operation
        std::unique_lock<std::mutex> u_lock(lock);

        if (task_queue.empty())
            return false;
        
        // 'var' will take the 'return value'
        var = std::move(task_queue.front());
        task_queue.pop();
        return true;
    }

    ~Safe_Queue() = default;
};

#endif