#ifndef _WORKING_THREAD_HPP_
#define _WORKING_THREAD_HPP_

#include <functional>
#include <mutex>
#include <memory>

/********************************************************************

* This header file implements those threads working in our pool

*********************************************************************/

// forward declaration to avoid cross reference of header files
class Thread_Pool;

class Working_Thread
{
private:
    // the thread pool this working thread belongs to
    Thread_Pool* thread_pool;

public:
    Working_Thread(Thread_Pool* pool) : thread_pool(pool) {}

    void operator()();
};

#endif