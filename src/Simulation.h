#pragma once
#ifndef CLASSSIMULATION
#define CLASSSIMULATION

#include "base/common.h"
#include "ThreadPool.h"
#include <condition_variable>
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
    double nextUpdateLinkTime, nextUpdateRouteMap;
    std::condition_variable cv;
    std::mutex UpdatethreadPool;
    ThreadPool *pool;

protected:
    void floydWarshall(std::vector<std::vector<double>> &graph, std::vector<std::vector<int>> &nextStep);
    void flashEvent();
    int PoolSize;
    int PoolMax;

public:
    NetWorkSimulation(double nextUpdateLinkTime, double nextUpdateRouteMap,double stepTime, int PoolMax = 10);

    void initThread();
    void releaseThread();

    int initNode(int FType, int Findex);
    void initLink(int FType, int Findex, int TType, int Tindex, std::vector<double> &Time, std::vector<double> &RateList, std::vector<double> &LengthList);

    void UpdateLink(double now);
    void UpdateRateMap(double now);

    long long createTask(long long TaskID, double startTime, double TaskSize, int FType, int Findex, int TType, int Tindex);
    void NextFinish(long long &TaskID, double &now);

    void start();
};

#endif