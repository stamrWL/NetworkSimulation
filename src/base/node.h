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
    Node(int index);
    // int getNextNode(TransTask& event);
public:
    static std::map<int,Node*> NodeMap;

public:
    static Node* CreateNode(int index);

    void setRoutMap(std::shared_ptr<std::map<int,int>> &_routeMap);

    void acceptTask(std::shared_ptr<Task> task,double time);

    void Trans(std::shared_ptr<TransEvent> event);

};

#endif