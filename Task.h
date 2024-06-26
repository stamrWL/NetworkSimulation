#include <vector>
#include <map>
#include <memory>
#include <Exceptions.h>
#include "event.h"

class Event;
class Task{
private:
    long long TaskID;
    double startTime;
    double TaskSize;
    int fromIndex;
    int ToIndex;
protected:
    std::vector<std::shared_ptr<Event>> EventList;
    Task(long long TaskID,double startTime,double TaskSize,int fromIndex,int ToIndex){
        this->TaskID = TaskID;
        this->startTime = startTime;
        this->TaskSize = TaskSize;
        this->fromIndex = fromIndex;
        this->ToIndex = ToIndex;
    }
public:
    static std::map<long long, std::shared_ptr<Task>> TaskMap;
public:
    std::shared_ptr<Task> CreateTask(long long TaskID, double startTime, double TaskSize, int fromIndex, int ToIndex){
        std::shared_ptr<Task> newTask = std::make_shared<Task>(TaskID, startTime, TaskSize, fromIndex,  ToIndex);
        TaskMap[TaskID] = newTask;
        return newTask;
    }
    long long getTaskID()const{return TaskID;}
    int getFromIndex()const{return fromIndex;}
    int getToIndex()const{return ToIndex;}
    double getStartTime()const{return startTime;}
    double getTaskSize()const{return TaskSize;}
    void addEvent(std::shared_ptr<Event> Event){
        this->EventList.push_back(Event);
    }
}