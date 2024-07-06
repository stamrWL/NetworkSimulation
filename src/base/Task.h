#ifndef CLASSTASK
#define CLASSTASK
#include <vector>
#include <queue>
#include <map>
#include <memory>
#include <mutex>
#include <condition_variable>
#include "event.h"
class TransEvent;
class Task;

class CompareTask{
public:
    bool operator()(const std::shared_ptr<Task> a,const std::shared_ptr<Task> b);
};


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
public:
    static std::map<long long, std::shared_ptr<Task>> TaskMap;
    static std::mutex mtx;
    static std::condition_variable cv;
    static bool TaskHasFinish ;
    static std::priority_queue<std::shared_ptr<Task>,std::vector<std::shared_ptr<Task>>,CompareTask> FinishTask;
public:
    Task(long long TaskID,double startTime,double TaskSize,int fromIndex,int ToIndex);
    static std::shared_ptr<Task> CreateTask(long long TaskID, double startTime, double TaskSize, int fromIndex, int ToIndex);
    bool operator<(const Task& rhs)const{
        return this->endTime < rhs.endTime;
    };
    long long getTaskID()const{return TaskID;};
    int getFromIndex()const{return fromIndex;};
    int getToIndex()const{return ToIndex;};
    double getStartTime()const{return startTime;};
    double getEndTime()const{return endTime;};
    double getTaskSize()const{return TaskSize;};
    void setEndTime(double endTime){this->endTime = endTime;};
    void addEvent(std::shared_ptr<TransEvent> Event){
        this->EventList.push_back(std::move(Event));
    };
};

#endif