#pragma once
#ifndef CLASSLINK
#define CLASSLINK
#include "intervalTree.h"
#include "LinkTree.h"
#include <memory>
#include <vector>
#include <map>
class LinkTree;

class Link{
private:
    int fromIndex;
    int ToIndex;
    double stepTime;
    LinkTree* communication;
    std::shared_ptr<std::map<double,std::pair<double,double>>> RateDelayList;
    Link(int fromIndex,int ToIndex,std::shared_ptr<std::vector<std::pair<double,double>>> InitRate,std::shared_ptr<std::vector<std::pair<double,double>>> Delay,double stepTime,double windows = 30);
public:
static std::map<std::pair<int,int>,std::shared_ptr<Link>> linkMap;
static double lightRate = 299792458;
public:
    static std::shared_ptr<Link> CreateLink(int fromIndex, int ToIndex, double InitRate, double Length, double stepTime, double left = 0, double right = 1800, double windows = 30);
    static std::shared_ptr<Link> CreateLink(int fromIndex, int ToIndex, std::vector<double>& Time,std::vector<double>& RateList,std::vector<double>& LengthList, double stepTime, double windows = 30);
    double getValue(double Now);
    double getRate(double Now);
    void Update(double Now);
    // std::map<double,double> getRateList(); 
    double trans(double start,double size);
};
#endif