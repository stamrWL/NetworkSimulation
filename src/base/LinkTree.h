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
    void UpdateTree(double time);

    void releasLeft(double now); 
public:
    LinkTree(double initValue,double left,double stepTime,Link* link){
        LastUpdate = 0;
        LastLeft = 0;
        this->stepTime = stepTime;
        this->link = link;
        LinkB = new IntervalTree(initValue,left,stepTime);
    }
    bool Update(double endTime);
    double getRangeArea(double start, double end);
    std::pair<double, double> trans(double start,double end,double value);
    std::pair<double, double> trans(double start,double size);
};
#endif