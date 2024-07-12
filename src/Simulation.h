#pragma once
#ifndef CLASSSIMULATION
#define CLASSSIMULATION

#include "base/common.h"
#include "ThreadPool.h"
#include <condition_variable>
#include <set>  
#include <mutex>
#include <vector>
#include <pthread.h>
#include <map>
#include <omp.h>

class NetWorkSimulation
{
private:
    int NodeCount = 0;
    double stepTime;
    std::map<std::pair<int, int>, int> TyI2I;
#ifdef TEST_DEBUG
    std::map<int,std::pair<int, int>> I2TyI;
#endif
    double nextUpdateLinkTime, nextUpdateRouteMap;
    double initStartTaskTime, nextUpdateTaskInterval;
    std::condition_variable cv;
    std::mutex UpdatethreadPool;
    ThreadPool *pool,*UpdatePool;

protected:
    void floydWarshall(std::vector<std::vector<double>> &graph, std::vector<std::vector<int>> &nextStep);
    void flashEvent();
    void initThread();
    void releaseThread();
    void CreateInitTask();

    int PoolSize;
    int PoolMax;

    double lastTime = 0;
public:
    NetWorkSimulation(double nextUpdateRouteMap,double stepTime, int PoolMax = 10, double nextUpdateTaskInterval = 10);

    int initNode(int FType, int Findex);
    void initLink(int FType, int Findex, int TType, int Tindex, std::vector<double> &Time, std::vector<double> &RateList, std::vector<double> &DelayList);

    void UpdateLink(double now);
    void UpdateRateMap(double now);

    long long createTask(long long TaskID, double startTime, double TaskSize, int FType, int Findex, int TType, int Tindex);
    void NextFinish(long long &TaskID, double &now);

    void startEventFlash();
    void blockAll();

    void start();
    void stop();
};

#endif