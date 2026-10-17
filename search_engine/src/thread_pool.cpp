/**
 *  @file  thread-pool.cpp
 *  @brief iCE && tEA Search Engine module.
 *         Provides a thread pool implementation.
 */

#include "thread_pool.h"

#include <utility>

ThreadPool::ThreadPool()
{
    isRunnung_ = false;
    isStopping_ = false;
}

ThreadPool::~ThreadPool()
{
    // If the wait() call was missed, the threads must be closed gracefully on exit
    if (isRunnung_)
    {
        {
            std::lock_guard<std::mutex> lock(m_);
            tasks_.clear();
        }

        wait();
    }
}

void ThreadPool::run()
{
    if (isRunnung_)
        return;

    unsigned threadCount = std::thread::hardware_concurrency();

    if (!threadCount)
        threadCount = 1;

    for (unsigned i = 0; i < threadCount; ++i)
    {
        threads_.emplace_back(std::thread(&ThreadPool::worker, this));
    }

    isRunnung_ = true;
}

void ThreadPool::enqueue(std::function<void ()>&& task)
{
    if (!isRunnung_)
        return;

    {
        std::lock_guard<std::mutex> lock(m_);

        // rvalue-reference is named, so std::move is still needed
        tasks_.push_back(std::move(task));
    }

    cvTasks_.notify_one();
}

void ThreadPool::wait()
{
    if (!isRunnung_)
        return;

    std::unique_lock<std::mutex> lk(m_);

    while (!tasks_.empty())
    {
        cvWait_.wait(lk);
    }

    isStopping_ = true;

    lk.unlock();

    cvTasks_.notify_all();

    for (auto& thread : threads_)
    {
        if (thread.joinable())
            thread.join();
    }

    isRunnung_ = false;
}

void ThreadPool::worker()
{
    while (true)
    {
        std::unique_lock<std::mutex> lk(m_);

        while (tasks_.empty() && !isStopping_)
        {
            cvTasks_.wait(lk);
        }

        if (isStopping_)
        {
            lk.unlock();

            return;
        }

        std::function<void()> task = std::move(tasks_.front());
        tasks_.pop_front();

        lk.unlock();

        if (task)
            task();

        bool needNotify = false;

        {
            std::lock_guard<std::mutex> lock(m_);

            if (tasks_.empty())
                needNotify = true;
        }

        if (needNotify)
            cvWait_.notify_all();
    }
}
