#include "thread_pool.hpp"

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <random>

// initialize random number generator
std::random_device rd;
std::mt19937 mt(rd());
std::uniform_int_distribution<int> model(-1000, 1000);

// i_rand() will generate a randome integar between -1000 and 1000
auto i_rand = std::bind(model, mt);

// make the thread randomly sleep so as to simulate complex task
void sleep_randomly();

// perform multiplication and print the result
void multiply(int a, int b);

// perform multiplication and store the result in 'res'
void multiply_store(int a, int b, int& res);

// perform multiplication and return the result
int multiply_return(int a, int b);

int main()
{
    Thread_Pool thread_pool(3);

    thread_pool.init();

    for (int i = 1; i <= 3; i++)
    {
        for (int j = 1; j <= 10; j++)
        {
            thread_pool.submit(multiply, i, j);
        }
    }

    int result1;
    auto future1 = thread_pool.submit(multiply_store, std::ref(result1), 10, 10);

    future1.get();
    std::cout << "result1 is: " << result1 << std::endl;

    auto future2 = thread_pool.submit(multiply_return, 20, 20);

    int result2 = future2.get();
    std::cout << "result2 is: " << result2 << std::endl;

    thread_pool.shutdown();

    system("pause");

    return 0;
}

void sleep_randomly()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(2000 + i_rand()));
}

void multiply(int a, int b)
{
    sleep_randomly();

    std::cout << a << " * " << b << " = " << a * b << std::endl;
}

void multiply_store(int a, int b, int& res)
{
    sleep_randomly();

    res = a * b;
    std::cout << a << " * " << b << " = " << res << std::endl;
}

int multiply_return(int a, int b)
{
    sleep_randomly();

    std::cout << a << " * " << b << " = " << a * b << std::endl;
    return a * b;
}