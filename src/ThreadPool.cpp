#include "ThreadPool.h"


size_t ThreadPool::getThread()
{
    std::unique_lock<std::mutex> lock(this->runNextmut);
    return runningThread;
}

void ThreadPool::setCallback(std::function<void()> callback)
{
    std::unique_lock<std::mutex> lock(changeCallback);
    this->task_finished_callback = std::move(callback);
}

void ThreadPool::blockRunNext()
{
    std::unique_lock<std::mutex> lock(runNextmut);
    this->runNext = false;
    conditionRun.wait(lock, [this]() -> bool
                      { return this->runNext || this->runningThread == 0; });
}

void ThreadPool::unblockRunNext()
{
    std::unique_lock<std::mutex> lock(runNextmut);
    this->runNext = true;
    conditionRun.notify_all();
}

void ThreadPool::start()
{
    unblockRunNext();
}

// the constructor just launches some amount of workers
ThreadPool::ThreadPool(size_t threads)
    : stop(false), runNext(false), runningThread(0)
{
    {
        std::unique_lock<std::mutex> lock(changeCallback);
        task_finished_callback = std::function<void()>([]() -> void {});
    }
    for (size_t i = 0; i < threads; ++i)
        workers.emplace_back(
            [this]
            {
                for (;;)
                {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        this->condition.wait(lock,
                                             [this]
                                             { return this->stop || ((!this->tasks.empty())); });
                        if (this->stop && this->tasks.empty())
                            return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }

                    {
                        std::unique_lock<std::mutex> lock(runNextmut);
                        conditionRun.wait(lock, [this]() -> bool
                                          { return this->runNext; });
                        runningThread++;
                    }

                    task();

                    {
                        std::unique_lock<std::mutex> lock(runNextmut);
                        runningThread--;
                        conditionRun.notify_all();
                        //
                    }

                    this->task_finished_callback();
                }
            });
}
// the destructor joins all threads
ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread &worker : workers)
        worker.join();
}