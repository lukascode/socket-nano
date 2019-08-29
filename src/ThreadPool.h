#pragma once

#include<queue>
#include<functional>
#include<mutex>
#include<condition_variable>
#include<thread>
#include<atomic>

class ThreadPool {
public:
    ThreadPool(int size);
    ~ThreadPool();
    void SubmitTask(std::function<void()> task);
    void Shutdown();
private:

std::vector<std::thread> workers;
std::queue<std::function<void()>> task_queue;
std::mutex task_queue_mtx;
std::condition_variable cond;
std::atomic<bool> halted;

void createThreadPool(int size);

};