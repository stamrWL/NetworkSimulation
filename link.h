#pragma once
#ifndef CLASSLINK
#define CLASSLINK
#include "intervalTree.h"
#include "LinkTree.h"
#include <memory>
#include <vector>
class LinkTree;

class Link{
private:
    int fromType;
    int fromIndex;
    int ToType;
    int ToIndex;
    double stepTime;
    double needTrans;
    LinkTree* communication;
    std::shared_ptr<std::vector<double>> RateList;
public:
    Link(int fromType,int fromIndex,int ToType,int ToIndex,double InitRate,double stepTime,double left = 0,double windows = 30);
    Link(int fromType,int fromIndex,int ToType,int ToIndex,std::shared_ptr<std::vector<double>> InitRate,double stepTime,double left = 0,double windows = 30);
    double getValue(double Now);
    double getRate(double Now);
    std::shared_ptr<std::vector<double>> getRateList(); 
    void trans(double start,double size);
};
#endif