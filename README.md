# Thaum1el's Thread Pool

An encapsulated but crude **Thread Pool** written in C++.  

This library supports pre-allocating specific amount of threads that will ***work forever*** until the user shuts the **Thread Pool** down, and allows the user submit tasks to the **Thread Pool** in a form like call-back functions.  

## Components

The entirety of the **Thread Pool** are made of three components as follow: 

* `safe_queue.hpp`

As it suggests, this header file implements a task queue on the basis of `std::queue`, guarded by `std::unique_lock` to make it **thread-safe**.

It provides following interfaces whose function I believe is easy to understand.

```C++
bool empty();
int  size();
void enqueue(T& obj);
bool dequeue(T& obj);    // return true if fetched a valid value
```

* `working_thread.hpp`

This component is a wrapped **thread class** which really performs the tasks the user submits. The wrapper itself ought to be **transparent** to users.

Once a working thread is allocated, it just keeps asking the **Thread Pool** it belongs to for a task and then performs it, over and over again. The pseudo code of its implement looks like:

```C++
// keep working until the thread pool is shut down
while (!thread_pool->is_shutdown)
{
    // if there is no task in the queue, wait for notification
    if (thread_pool->task_queue.empty())
    {
        wait();
    }

    // when waked up, try to fetch a task from the queue
    get_task = thread_pool->task_queue.dequeue(task);

    // if fetched successfully, perform this task
    if (get_task)
    {
        task();
    }
}
```

* `thread_pool.hpp`

Obviously, the implement of the **Thread Pool** on the top. It holds a `safe_queue` and a vector of `working_thread` mentioned above.

It provides following interfaces whose function I believe is also easy to understand.

```C++
Thread_Pool(int capacity);    // specify its capacity while creating
void init();
void shutdown();
auto submit(func, args...) -> std::future<(func(args...))>
```

## Usage

Check `src/main.cpp` for detailed usage of the **Thread Pool**.