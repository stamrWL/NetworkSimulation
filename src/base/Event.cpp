#include "Event.h"

long long TransEvent::eventCount;
std::map<long long,std::shared_ptr<TransEvent>> TransEvent::eventMap;
std::mutex TransEvent::mut;
bool TransEvent::needblock = true;
std::condition_variable TransEvent::cv;

std::priority_queue<std::shared_ptr<TransEvent>,std::vector<std::shared_ptr<TransEvent>>,std::less<std::shared_ptr<TransEvent>>> TransEvent::eventQueue;

TransEvent::TransEvent(long long Taskid,int fIndex,int tIndex,double sTime,double eTime){
    this->Taskid = Taskid;
    this->Eventid = ++eventCount;
    this->fromIndex = fIndex;
    this->toIndex = tIndex;
    this->startTime = sTime;
    this->endTime = eTime;
}

std::shared_ptr<TransEvent>  TransEvent::CreateEvent(long long Taskid,int fIndex, int tIndex, double sTime, double eTime){
    auto event = std::make_shared<TransEvent>(Taskid,fIndex, tIndex, sTime, eTime);
    TransEvent::eventMap[event->getEventid()] = event;
    if(Task::TaskMap.find(Taskid) != Task::TaskMap.end())
        Task::TaskMap[Taskid]->addEvent(event);
    return event;
}

long long TransEvent::getEventid() const{
    return this->Eventid;
}

std::shared_ptr<Task>  TransEvent::getTask(){
    return Task::TaskMap[this->Taskid];
}

void TransEvent::setEndTime(double endTime) {
    this->endTime = endTime;
}

double TransEvent::getStartTime() const{
    return this->startTime;
}
double TransEvent::getEndTime() const{
    return this->endTime;
}

int TransEvent::getFromIndex() const{
    return this->fromIndex;
}

int TransEvent::getToIndex() const{
    return this->toIndex;
}

void TransEvent::finish(double endTime){
    if(endTime >=0 && std::abs(endTime - this->endTime) > 0.002){
        throw std::runtime_error("Event finish error");
    }
    if(endTime >= 0)
        this->endTime = endTime;

    Node::NodeMap[this->toIndex]->acceptTask(Task::TaskMap[this->Taskid],this->endTime);  
}