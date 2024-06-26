#include <vector>
#include <memory>
#include <Exceptions.h>
#include "event.h"

class Event;
class Task{
private:
    long long TaskID;
    double startTime;
    double TaskSize;
    int fromType;
    int fromIndex;
    int ToType;
    int ToIndex;
protected:
    std::vector<std::shared_ptr<Event>> EventList;
public:
    Task(long long TaskID,double startTime,double TaskSize,int fromType,int fromIndex,int ToType,int ToIndex){
        this->TaskID = TaskID;
        this->startTime = startTime;
        this->TaskSize = TaskSize;
        this->fromType = fromType;
        this->fromIndex = fromIndex;
        this->ToType = ToType;
        this->ToIndex = ToIndex;
    }
    int getFromType(){return fromType;}
    int getFromIndex(){return fromIndex;}
    int getToType(){return ToType;}
    int getToIndex(){return ToIndex;}
    void addEvent(std::shared_ptr<Event> Event){
        int LastType = Event->getFromType();
        int LastIndex = Event->getFromIndex();
        if(LastIndex != this->EventList.at(EventList.size() - 1)->getToIndex() || LastType != this->EventList.at(EventList.size() - 1)->getFromType()){
            throw std::exception("nextEvent from != to")
        }
        this->EventList.push_back(Event);
    }
}