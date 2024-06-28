#ifndef CLASSLINKTREE
#define CLASSLINKTREE
#include "intervalTree.h"
#include "link.h"
#include <cfloat>
#include <vector>
class Link;

class LinkTree{
private:
    IntervalTree* LinkB;
    Link* link;
    double LastUpdate;
    double LastLeft;
    double stepTime;
protected:
    void Update(double endTime);
    void releasLeft(double now); 
public:
    LinkTree(double initValue,double left,double windows,double stepTime,Link* link){
        LastUpdate = initValue;
        LastLeft = 0;
        this->stepTime = stepTime;
        this->link = link;
        LinkB = new IntervalTree(initValue,left,windows);
    }
    double getRangeArea(double start, double end);
    std::pair<double, double> trans(double start,double end,double value);
    std::pair<double, double> trans(double start,double size);
};
#endif