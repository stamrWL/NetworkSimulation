#ifndef CLASSEVENT
#define CLASSEVENT
#include <map>
#include <memory>
#include <cmath>
#include <queue>
#include "node.h"
#include "Task.h"
class Event : public std::enable_shared_from_this<Event>{
private:
    static long long eventCount;
    long long Eventid;
    long long Taskid;
    int fromIndex;
    int toIndex;
    double startTime,endTime;
    Event(long long Taskid,int fIndex,int tIndex,double sTime,double eTime = -1){
        this->Taskid = Taskid;
        this->Eventid = ++eventCount;
        this->fromIndex = fIndex;
        this->toIndex = tIndex;
        this->startTime = sTime;
        this->endTime = eTime;
    }
public:
    static std::map<long long,std::shared_ptr<Event>> eventMap;
    static std::priority_queue<std::shared_ptr<Event>,std::vector<std::shared_ptr<Event>>,std::less<std::shared_ptr<Event>>> eventQueue;
    static std::shared_ptr<Event> CreateEvent(long long Taskid,int fIndex, int tIndex, double sTime, double eTime = -1);
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