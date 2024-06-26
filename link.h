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
    double needTrans;
    LinkTree* communication;
    std::shared_ptr<std::vector<double>> RateList;
    Link(int fromIndex,int ToIndex,double InitRate,double stepTime,double left = 0,double windows = 30);
    Link(int fromIndex,int ToIndex,std::shared_ptr<std::vector<double>> InitRate,double stepTime,double left = 0,double windows = 30);
public:
static std::map<std::pair<int,int>,std::shared_ptr<Link>> linkMap;
public:
    static std::shared_ptr<Link> CreateLink(int fromIndex, int ToIndex, double InitRate, double stepTime, double left = 0, double windows = 30);
    static std::shared_ptr<Link> CreateLink(int fromIndex, int ToIndex, std::shared_ptr<std::vector<double>> InitRate, double stepTime, double left = 0, double windows = 30);
    double getValue(double Now);
    double getRate(double Now);
    std::shared_ptr<std::vector<double>> getRateList(); 
    double trans(double start,double size);
};
#endif