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

double LinkTree::getRangeArea(double start, double end){
    return LinkB->getRangeArea(start, end);
}

std::pair<double, double> LinkTree::trans(double start,double end,double value){
    releasLeft(start);
    Update(end);
    LinkB->addValue(start,end,-1*value);
    return std::pair<double,double>(start, end);
}

std::pair<double, double> LinkTree::trans(double start,double size){
    releasLeft(start);
    std::vector<double> interval;
    LinkB->ContinuousAllocated(start,size,interval);
    if(interval.size() > 1)
        Update(interval.at(interval.size() - 1));
    return LinkB->addContinueValue(interval,size);
}