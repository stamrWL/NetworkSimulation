#include "Task.h"
bool CompareTask::operator()(const std::shared_ptr<Task> a,const std::shared_ptr<Task> b)
{
        return a->getEndTime() > b->getEndTime();
};


bool Task::TaskHasFinish = false;
std::map<long long, std::shared_ptr<Task>> Task::TaskMap;
std::mutex Task::mtx;
std::condition_variable Task::cv;
std::priority_queue<std::shared_ptr<Task>,std::vector<std::shared_ptr<Task>>,CompareTask> Task::FinishTask;

Task::Task(long long TaskID, double startTime, double TaskSize, int fromIndex, int ToIndex)
{
        this->TaskID = TaskID;
        this->startTime = startTime;
        this->TaskSize = TaskSize;
        this->fromIndex = fromIndex;
        this->ToIndex = ToIndex;
        this->endTime = -1;
};

std::shared_ptr<Task> Task::CreateTask(long long TaskID, double startTime, double TaskSize, int fromIndex, int ToIndex)
{
        std::shared_ptr<Task> newTask = std::make_shared<Task>(TaskID, startTime, TaskSize, fromIndex, ToIndex);
        Task::TaskMap[TaskID] = newTask;
        auto event = TransEvent::CreateEvent(TaskID, fromIndex, fromIndex, startTime, startTime);
        {
                std::unique_lock<std::mutex> lock(TransEvent::mut);
                TransEvent::eventQueue.push(event);
                TransEvent::cv.notify_all();
        }
        return newTask;
};
