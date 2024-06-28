#ifndef CLASSEVENT
#define CLASSEVENT
#include <map>
#include <memory>
#include <condition_variable>
#include <cmath>
#include <queue>
#include "node.h"
#include "Task.h"



class TransEvent : public std::enable_shared_from_this<TransEvent>{
private:
    static long long eventCount;
    long long Eventid;
    long long Taskid;
    int fromIndex;
    int toIndex;
    double startTime,endTime;
    TransEvent(long long Taskid,int fIndex,int tIndex,double sTime,double eTime = -1){
        this->Taskid = Taskid;
        this->Eventid = ++eventCount;
        this->fromIndex = fIndex;
        this->toIndex = tIndex;
        this->startTime = sTime;
        this->endTime = eTime;
    }
public:
    static std::map<long long,std::shared_ptr<TransEvent>> eventMap;
    static std::mutex mut;
    static bool needblock = false;
    static std::condition_variable cv;
    static std::priority_queue<std::shared_ptr<TransEvent>,std::vector<std::shared_ptr<TransEvent>>,std::less<std::shared_ptr<TransEvent>>> eventQueue;
    static std::shared_ptr<TransEvent> CreateEvent(long long Taskid,int fIndex, int tIndex, double sTime, double eTime = -1);
public:
    long long getEventid() const;
    std::shared_ptr<Task> getTask();
    void setEndTime(double endTime);

    double getStartTime() const;
    double getEndTime() const;
    int getFromIndex() const;
    int getToIndex() const;

    void finish(double endTime = -1);
}

#endif