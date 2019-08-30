#pragma once

#include<queue>
#include<functional>
#include<mutex>
#include<condition_variable>
#include<thread>
#include<atomic>

class ThreadPool {
public:
    /// Creates the thread pool with provided size.
    ThreadPool(int size);
    ~ThreadPool();

    /// Submits task to thread pool.
    void SubmitTask(std::function<void()> task);

    /// Shutdown thread pool. When there are some tasks on the task queue the will be completed firts.
    void Shutdown();
private:

std::vector<std::thread> workers;
std::queue<std::function<void()>> task_queue;
std::mutex task_queue_mtx;
std::condition_variable cond;
std::atomic<bool> halted;

void createThreadPool(int size);

};