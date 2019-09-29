#include "ThreadPool.h"

ThreadPool::ThreadPool(int size) : halted(false)
{
    createThreadPool(size);
}

ThreadPool::~ThreadPool()
{
    Shutdown();
}

void ThreadPool::SubmitTask(std::function<void()> task)
{
    std::unique_lock<std::mutex> lock(task_queue_mtx);
    task_queue.push(task);
    cond.notify_one();
}

void ThreadPool::Shutdown()
{
    std::unique_lock<std::mutex> lock(task_queue_mtx);
    if (!halted.load())
    {   
        halted.store(true);
        cond.notify_all();
        lock.unlock();
        for (size_t i = 0; i < workers.size(); ++i)
        {
            workers[i].join();
        }
    }
}

bool ThreadPool::isHalted()
{
    return halted.load();
}

void ThreadPool::createThreadPool(int size)
{
    for (int i = 0; i < size; ++i)
    {
        workers.emplace_back([this] {
            for (;;)
            {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(task_queue_mtx);
                    cond.wait(lock, [this] { return halted.load() || !task_queue.empty(); });
                    if (halted.load() && task_queue.empty())
                        break;
                    task = std::move(task_queue.front());
                    task_queue.pop();
                }
                task();
            }
        });
    }
}