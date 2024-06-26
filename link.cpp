#include "link.h"

Link::Link(int fromType,int fromIndex,int ToType,int ToIndex,double InitRate,double stepTime,double left,double windows){
    this->fromType = fromType;
    this->fromIndex = fromIndex;
    this->ToType = ToType;
    this->ToIndex = ToIndex;
    this->RateList = std::make_shared<std::vector<double>>();
    this->RateList->push_back(InitRate);
    this->stepTime = stepTime;
    this->communication = new LinkTree(InitRate,left,windows,stepTime,this);
}

Link::Link(int fromType,int fromIndex,int ToType,int ToIndex,std::shared_ptr<std::vector<double>> InitRate,double stepTime,double left,double windows){
    this->fromType = fromType;
    this->fromIndex = fromIndex;
    this->ToType = ToType;
    this->ToIndex = ToIndex;    
    this->RateList = InitRate;
    this->stepTime = stepTime;
    this->communication = new LinkTree(InitRate->at(std::min(int(left/stepTime),int(this->RateList->size() - 1))),left,windows,stepTime,this);
}


double Link::getValue(double Now){
    return 0;
}

double Link::getRate(double Now){
    int index = int(Now/stepTime);
    return RateList->at(std::min(index,int(RateList->size() - 1)));
}

std::shared_ptr<std::vector<double>> Link::getRateList(){
    return RateList;
}

void Link::trans(double start,double size){
    communication->trans(start,size);
}
