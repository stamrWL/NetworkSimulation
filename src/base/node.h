#ifndef CLASSNODE
#define CLASSNODE
#include "event.h"
#include "link.h"
#include "Task.h"
#include "Event.h"
#include <map>
#include <memory>
#include <vector>
class Task;
class TransEvent;
class Node
{
private:
    int index;
    std::shared_ptr<std::map<int,int>> routeMap;
    // int getNextNode(TransTask& event);
public:
    static std::map<int,Node*> NodeMap;

public:
    Node(int index){
        this->index = index;
        NodeMap[index] = this;
    };

    void setRoutMap(std::shared_ptr<std::map<int,int>> _routeMap){
        this->routeMap = _routeMap;
    };

    void acceptTask(std::shared_ptr<Task> task,double time);

    void Trans(std::shared_ptr<TransEvent> event);

};

#endif