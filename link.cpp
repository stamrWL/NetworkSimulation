#include "link.h"

Link::Link(int fromIndex,int ToType,int ToIndex,double InitRate,double stepTime,double left,double windows){

    this->fromIndex = fromIndex;

    this->ToIndex = ToIndex;
    this->RateList = std::make_shared<std::vector<double>>();
    this->RateList->push_back(InitRate);
    this->stepTime = stepTime;
    this->communication = new LinkTree(InitRate,left,windows,stepTime,this);
}

Link::Link(int fromIndex,int ToIndex,std::shared_ptr<std::vector<double>> InitRate,double stepTime,double left,double windows){

    this->fromIndex = fromIndex;

    this->ToIndex = ToIndex;    
    this->RateList = InitRate;
    this->stepTime = stepTime;
    this->communication = new LinkTree(InitRate->at(std::min(int(left/stepTime),int(this->RateList->size() - 1))),left,windows,stepTime,this);
}

static std::shared_ptr<Link> Link::CreateLink(int fromIndex, int ToIndex, double InitRate, double stepTime, double left = 0, double windows = 30){
    auto link = std::make_shared<Link>(fromIndex, ToIndex, InitRate, stepTime, left, windows);
    Link::linkMap[std::pair<int,int>(fromIndex, ToIndex)] = link;
    return link;
}

static std::shared_ptr<Link> Link::CreateLink(int fromIndex, int ToIndex, std::shared_ptr<std::vector<double>> InitRate, double stepTime, double left = 0, double windows = 30){
    auto link = std::make_shared<Link>(fromIndex, ToIndex, InitRate, stepTime, left, windows);
    Link::linkMap[std::pair<int,int>(fromIndex, ToIndex)] = link;
    return link;
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

double Link::trans(double start,double size){
    // 返回结束时间
    auto SE = communication->trans(start,size);
    double end = SE.second;
    return end;
}
