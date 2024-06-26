#include "node.h"


void Node::acceptTask(std::shared_ptr<Task> task,double time){
    int ToIndex = task->getToIndex();
    int NextNode = this->routeMap[ToIndex];
    auto event = Event::CreateEvent(task->getTaskID(),this->index,NextNode,time);
    this->Trans(event);
}


void Node::Trans(std::shared_ptr<Event> event){
    auto task = event->getTask();
    double size = task->getTaskSize();
    auto link = Link::linkMap[std::pair<int,int>(event->getFromIndex(),event->getToIndex())];
    double endTime = link->trans(event->getStartTime(),size);
    event->setEndTime(endTime);
    Event::EventQueue.push(event);
}