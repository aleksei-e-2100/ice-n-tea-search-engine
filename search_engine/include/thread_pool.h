/**
 *  @file  thread-pool.h
 *  @brief iCE && tEA Search Engine module.
 *         Provides a thread pool implementation.
 */

#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <thread>
#include <condition_variable>
#include <mutex>
#include <deque>
#include <functional>
#include <vector>

/**
 *  @class ThreadPool
 *  @brief Implements a thread pool
 */
class ThreadPool
{
public:
    /**
     *  @brief Constructor
     */
    ThreadPool();

    /**
     *  @brief Destructor
     */
    ~ThreadPool();

    // Assignment and transfer are prohibited
    ThreadPool(const ThreadPool& other) = delete;
    ThreadPool& operator=(const ThreadPool& other) = delete;
    ThreadPool(ThreadPool&& other) = delete;
    ThreadPool& operator=(ThreadPool&& other) = delete;

    /**
     *  @brief Starts the thread pool
     */
    void run();

    /**
     *  @brief Enqueues a task for execution (the first available thread will take it)
     *  @param task - the task
     */
    void enqueue(std::function<void ()> &&task);

    /**
     *  @brief Waits until all tasks enqueued in the thread pool are completed
     */
    void wait();

private:
    /**
     *  @brief Implements a worker thread
     */
    void worker();

private:
    std::vector<std::thread> threads_;
    std::deque<std::function<void()>> tasks_;

    std::condition_variable cvWait_;
    std::condition_variable cvTasks_;

    std::mutex m_;

    bool isRunnung_;
    bool isStopping_;
};

#endif // THREAD_POOL_H
