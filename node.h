#ifndef CLASSNODE
#define CLASSNODE
#include "event.h"
#include "link.h"
#include <map>
#include <memory>
#include <vector>

class Node
{
private:
    int index;
    double stepTime;
    std::shared_ptr<std::map<int,int>> routeMap;
    std::shared_ptr<std::map<int,Link>> routeMap;
    int getNextNode(TransTask& event);
public:
    static std::map<int,std::shared_ptr<Node>> NodeMap;
public:
    Node(int index,double stepTime){
        this->index = index;
        this->stepTime = stepTime;
    }

    void setRoutMap(std::shared_ptr<std::map<int,int>> _routeMap){
        this->routeMap = _routeMap;
    }

    void acceptTask(std::shared_ptr<Task> task,double time);

    void Trans(std::shared_ptr<Event> event);

};

#endif