#include "intervalTree.h"
#include "event.h"
#include <map>
#include <memory>
#include <vector>

class Node
{
private:
    std::shared_ptr<IntervalTree> communication;
    std::shared_ptr<std::vector<double>> RateList;
    int type;
    int index;
    double stepTime;
public:
    Node(int type,int index,double InitRate,double stepTime,double left = 0,double windows = 30){
        this->type = type;
        this->index = index;
        this->RateList = std::make_shared<std::vector<double>>();
        this->RateList->push_back(InitRate);
        this->stepTime = stepTime;
        this->communication = std::make_shared<IntervalTree>(InitRate,left,windows);
    }

    Node(int type,int index,std::shared_ptr<std::vector<double>> InitRate,double left = 0,double windows = 30){
        this->type = type;
        this->index = index;
        this->RateList = InitRate;
        this->stepTime = stepTime;
        this->communication = std::make_shared<IntervalTree>(InitRate->at(min(int(left/stepTime),this->RateList->size() - 1)),left,windows);
    }

    void Trans(std::shared_ptr<Event> event);

};