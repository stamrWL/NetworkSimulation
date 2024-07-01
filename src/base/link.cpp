
#include "link.h"

std::map<std::pair<int,int>,std::shared_ptr<Link>> Link::linkMap;

Link::Link(int fromIndex,int ToIndex,std::shared_ptr<std::vector<std::pair<double,double>>> InitRate,std::shared_ptr<std::vector<std::pair<double,double>>> Delay,double stepTime,double windows){
    this->RateDelayList = nullptr;
    this->fromIndex = fromIndex;

    this->ToIndex = ToIndex;  
    std::shared_ptr<std::map<double,std::pair<double,double>>> RDMap = std::make_shared<std::map<double,std::pair<double,double>>>();
    for(int i=0;i<InitRate->size();i++){
        (*RDMap)[InitRate->at(i).first] = std::make_pair(InitRate->at(i).second, Delay->at(i).second);
    }
    this->RateDelayList = RDMap;
    this->stepTime = stepTime;
    this->communication = new LinkTree(InitRate->at(0).second,InitRate->at(0).first,windows,stepTime,this);
}

std::shared_ptr<Link> Link::CreateLink(int fromIndex, int ToIndex, double InitRate, double Length, double stepTime, double left, double right, double windows){
    std::shared_ptr<std::vector<std::pair<double,double>>> InitRateList = std::make_shared<std::vector<std::pair<double,double>>>();
    std::shared_ptr<std::vector<std::pair<double,double>>> DelayList = std::make_shared<std::vector<std::pair<double,double>>>();
    left = int(left / stepTime) * stepTime;
    for(double i = left;i<right;i+=stepTime){
        InitRateList->push_back(std::make_pair(i, InitRate));
        DelayList->push_back(std::make_pair(i, Length/Link::lightRate));
    }
    auto link = std::make_shared<Link>(fromIndex, ToIndex, InitRateList, DelayList, stepTime,  windows);
    Link::linkMap[std::pair<int,int>(fromIndex, ToIndex)] = link;
    return link;
}

std::shared_ptr<Link> Link::CreateLink(int fromIndex, int ToIndex, std::vector<double>& Time,std::vector<double>& RateList,std::vector<double>& LengthList, double stepTime, double windows){
    std::shared_ptr<std::vector<std::pair<double,double>>> InitRate = std::make_shared<std::vector<std::pair<double,double>>>();
    std::shared_ptr<std::vector<std::pair<double,double>>> Delay = std::make_shared<std::vector<std::pair<double,double>>>();
    for(int i=0;i<Time.size();i++){
        double time = int(Time[i]/stepTime)*stepTime;
        InitRate->push_back(std::make_pair(time, RateList[i]));
        Delay->push_back(std::make_pair(time, LengthList[i]/Link::lightRate));
    }
    auto link = std::make_shared<Link>(fromIndex, ToIndex, InitRate, Delay, stepTime,  windows);
    Link::linkMap[std::pair<int,int>(fromIndex, ToIndex)] = link;
    return link;
}

double Link::getValue(double Now){
    // һ�����ڹ���·�ɱ��Ĳ���
    // �˴�ʹ����һ��steptime�ڵĴ������ʱ��
    double time = int(Now/stepTime)*stepTime;
    double Area = communication->getRangeArea(time,time + stepTime);
    double Rate = getRate(time);
    return Rate>0?(Rate * stepTime - Area)/Rate:DBL_MAX;
}

double Link::getRate(double Now){
    auto upper = this->RateDelayList->upper_bound(Now);
    if(upper == this->RateDelayList->begin()){
        return 0;
    }else{
        upper--;
    }
    if(std::abs(Now - upper->first) > stepTime){
        return 0;
    }
    return upper->second.first;
}

double Link::getDelay(double Now){
    auto upper = this->RateDelayList->upper_bound(Now);
    if(upper == this->RateDelayList->begin()){
        return 0;
    }else{
        upper--;
    }
    if(std::abs(Now - upper->first) > stepTime){
        return 0;
    }
    return upper->second.second;
}

void Link::Update(double Now){
    communication->Update(Now);
}

double Link::trans(double start,double size){
    // ���ؽ���ʱ��
    auto SE = communication->trans(start,size);
    double end = SE.second;
    return end;
}
