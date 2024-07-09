#include "node.h"
std::map<int, Node *> Node::NodeMap;

Node::Node(int index){
    this->index = index;
    // NodeMap[index] = this;
    this->routeMap = nullptr;
};

Node* Node::CreateNode(int index){
    #if __cplusplus >= 201703L
        // std::cout<<"C++17"<<std::endl;
    #endif
    auto a = new Node(index);
    Node::NodeMap[index] = a;
    return a;
}

void Node::setRoutMap(std::shared_ptr<std::map<int, int>> &_routeMap)
{
    this->routeMap = nullptr;
    this->routeMap = _routeMap;
};

void Node::acceptTask(std::shared_ptr<Task> task, double time)
{
    int ToIndex = task->getToIndex();
    if (ToIndex != this->index)
    {
        int NextNode = (*this->routeMap)[ToIndex];
        auto event = TransEvent::CreateEvent(task->getTaskID(), this->index, NextNode, time);
        this->Trans(event);
    }
    else
    {
        task->setEndTime(time);
        std::unique_lock<std::mutex> lock(Task::mtx);
        Task::FinishTask.push(task);
        Task::TaskHasFinish = true;
        Task::cv.notify_all();
    }
}

void Node::Trans(std::shared_ptr<TransEvent> event)
{
    auto task = event->getTask();
    double size = task->getTaskSize();
    double endTime = -1;
    {
        auto link = Link::getLinkMap(event->getFromIndex(), event->getToIndex());
        std::unique_lock<std::mutex> lock(link->uniMTX);
        double startTime = event->getStartTime();
        endTime = link->trans(startTime, size);
    }

    event->setEndTime(endTime);
    {
        std::unique_lock<std::mutex> lock(TransEvent::mut);
        TransEvent::eventQueue.push(event);
        TransEvent::cv.notify_all();
    }
}