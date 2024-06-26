#include "LinkTree.h"

void LinkTree::Update(double endTime){
    endTime = int((endTime + stepTime)/stepTime);
    if(LastUpdate > endTime)return;
    while(endTime <= LastUpdate){
        LinkB->changeDefualtValue(endTime,this->link->getRate(endTime));
        endTime += stepTime;
    }
    LastUpdate = endTime;
}


void LinkTree::releasLeft(double now){
    double last = int((now - stepTime)/stepTime);
    if(last >= LastLeft){
        LinkB->releaseLeft(last);
        LastLeft = last;
    }
}

void LinkTree::trans(double start,double end,double value){
    releasLeft(start);
    Update(end);
    LinkB->addValue(start,end,-1*value);
}

void LinkTree::trans(double start,double size){
    releasLeft(start);
    std::vector<double> interval;
    LinkB->ContinuousAllocated(start,size,interval);
    if(interval.size() > 1)
        Update(interval.at(interval.size() - 1));
    LinkB->ContinuousAllocated(start,size,interval);
}