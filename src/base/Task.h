#include <vector>
#include <queue>
#include <map>
#include <memory>
#include <mutex>
#include <Exceptions.h>
#include <condition_variable>
#include "event.h"

class TransEvent;
class Task{
private:
    long long TaskID;
    double startTime;
    double endTime;
    double TaskSize;
    int fromIndex;
    int ToIndex;
protected:
    std::vector<std::shared_ptr<TransEvent>> EventList;
    Task(long long TaskID,double startTime,double TaskSize,int fromIndex,int ToIndex){
        this->TaskID = TaskID;
        this->startTime = startTime;
        this->TaskSize = TaskSize;
        this->fromIndex = fromIndex;
        this->ToIndex = ToIndex;
        this->endTime = -1;
    }
public:
    static std::map<long long, std::shared_ptr<Task>> TaskMap;
    static std::mutex mtx;
    static std::condition_variable cv;
    static bool TaskHasFinish = false;
    static std::priority_queue<std::shared_ptr<Task>,std::vector<std::shared_ptr<Task>>,std::less<std::shared_ptr<Task>>> FinishTask;
public:
    std::shared_ptr<Task> CreateTask(long long TaskID, double startTime, double TaskSize, int fromIndex, int ToIndex){
        std::shared_ptr<Task> newTask = std::make_shared<Task>(TaskID, startTime, TaskSize, fromIndex,  ToIndex);
        TaskMap[TaskID] = newTask;
        auto event = TransEvent::CreateEvent(this->TaskID,this->fromIndex,this->ToIndex,startTime,startTime);
        TransEvent::eventQueue.push(event);
        return newTask;
    }
    bool operator<(const Task& rhs)const{
        return this->endTime < rhs.endTime;
    }
    long long getTaskID()const{return TaskID;}
    int getFromIndex()const{return fromIndex;}
    int getToIndex()const{return ToIndex;}
    double getStartTime()const{return startTime;}
    double getEndTime()const{return endTime;}
    double getTaskSize()const{return TaskSize;}
    void setEndTime(double endTime){this->endTime = endTime;}
    void addEvent(std::shared_ptr<TransEvent> Event){
        this->EventList.push_back(Event);
    }
}