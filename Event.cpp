#include "Event.h"


static std::shared_ptr<Event>  Event::CreateEvent(long long Taskid,int fIndex, int tIndex, double sTime, double eTime){
    auto event = std::make_shared<Event>(Taskid,fIndex, tIndex, sTime, eTime);
    Event::eventMap[event->getEventid()] = event;
    if(Task::TaskMap.find(Taskid) != Task::TaskMap.end())
        Task::TaskMap[Taskid]->addEvent(event);
    return event;
}

bool Event::operator<(const Event& e) const{
    if(this->endTime == e.endTime)
        return this->Eventid < e.Eventid;
    else
        return this->endTime < e.endTime;
}

long long Event::getEventid() const{
    return this->Eventid;
}

std::shared_ptr<Task>  Event::getTask(){
    return Task::TaskMap[this->Taskid];
}

void Event::setEndTime(double endTime) {
    this->endTime = endTime;
}

double Event::getStartTime() const{
    return this->startTime;
}
double Event::getEndTime() const{
    return this->endTime;
}

int Event::getFromIndex() const{
    return this->fromIndex;
}

int Event::getToIndex() const{
    return this->toIndex;
}

void Event::finish(double endTime){
    if(endTime >=0 && std::abs(endTime - this->endTime) > 0.002){
        throw std::runtime_error("Event finish error");
    }
    if(endTime >= 0)
        this->endTime = endTime;

    Node::NodeMap[this->toIndex]->acceptTask(Task::TaskMap[this->Taskid],endTime);  
}