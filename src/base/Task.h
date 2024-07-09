#ifndef CLASSTASK
#define CLASSTASK
#include <vector>
#include <queue>
#include <map>
#include <memory>
#include <mutex>
#include <condition_variable>
#include "event.h"
#ifdef TEST_DEBUG
#include <sstream>
#include <string>
#endif

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
    std::mutex EListMTX;
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
#ifdef TEST_DEBUG
    std::shared_ptr<TransEvent> getLastEvent()const{
        return this->EventList.at(this->EventList.size()>=2?this->EventList.size()-2:0);
    }

    std::string viewEvent();
#endif

    void addEvent(std::shared_ptr<TransEvent> Event){
        std::unique_lock<std::mutex> lock(EListMTX);
        this->EventList.push_back(std::move(Event));
    };
};

#endif