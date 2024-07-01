#include "node.h"
std::map<int,Node*> Node::NodeMap;

void Node::acceptTask(std::shared_ptr<Task> task,double time){
    int ToIndex = task->getToIndex();
    if(ToIndex != this->index){
        int NextNode = (*this->routeMap)[ToIndex];
        auto event = TransEvent::CreateEvent(task->getTaskID(),this->index,NextNode,time);
        this->Trans(event);
    }else{
        task->setEndTime(time);
        std::unique_lock<std::mutex> lock(Task::mtx);
        Task::FinishTask.push(task);
        Task::TaskHasFinish = true;
        Task::cv.notify_all();
    }
}

void Node::Trans(std::shared_ptr<TransEvent> event){
    auto task = event->getTask();
    double size = task->getTaskSize();
    auto link = Link::linkMap[std::pair<int,int>(event->getFromIndex(),event->getToIndex())];
    double endTime = link->trans(event->getStartTime(),size);
    event->setEndTime(endTime);
    TransEvent::eventQueue.push(event);
}