#pragma once
#ifndef CLASSLINK
#define CLASSLINK
#include "intervalTree.h"
#include "LinkTree.h"
#include <memory>
#include <vector>
#include <mutex>
#include <map>
class LinkTree;

class Link{
private:
#ifdef TEST_DEBUG
public:
#endif
    int fromIndex;
    int ToIndex;
    double stepTime;
    LinkTree* communication;
    std::shared_ptr<std::map<double,std::pair<double,double>>> RateDelayList;
public:
static std::mutex SLMTX;
static std::map<int,std::map<int,std::shared_ptr<Link>>> linkMap;
static constexpr double lightRate = 299792458;
static void insertLinkMap(std::pair<int,int>&,std::shared_ptr<Link>&);
static void insertLinkMap(int,int,std::shared_ptr<Link>&);
static std::shared_ptr<Link> getLinkMap(std::pair<int,int>&);
static std::shared_ptr<Link> getLinkMap(int,int);
    std::mutex uniMTX;
public:
    Link(int fromIndex,int ToIndex,std::shared_ptr<std::vector<std::pair<double,double>>> InitRate,std::shared_ptr<std::vector<std::pair<double,double>>> Delay,double stepTime);
    static std::shared_ptr<Link> CreateLink(int fromIndex, int ToIndex, double InitRate, double Length, double stepTime, double left = 0, double right = 1800);
    static std::shared_ptr<Link> CreateLink(int fromIndex, int ToIndex, std::vector<double>& Time,std::vector<double>& RateList,std::vector<double>& LengthList, double stepTime);
    double getValue(double Now);
    double getRate(double Now);
    double getDelay(double Now);
    int getFrom()const{return fromIndex;};
    int getTo()const{return ToIndex;};
    bool Update(double Now);
    // std::map<double,double> getRateList(); 
    double trans(double start,double size);
};
#endif