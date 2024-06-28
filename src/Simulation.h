#include "base/common.h"
#include "ThreadPool.h"
#include <condition_variable>   
#include <mutex>
#include <vector>
#include <thread>
#include <map>
#include <omp.h>

class NetWorkSimulation{
private:
    int NodeCount = 0;
    std::map<std::pair<int, int>, int> TyI2I;
    double nextUpdateLinkTime,nextUpdateRouteMap;
    std::condition_variable cv;
    std::mutex UpdatethreadPool;
    ThreadPool* pool;
protected:
    void floydWarshall(std::vector<std::vector<double> >& graph, std::map<int,int>& nextStep);
    void flashEvent();
    int PoolSize;
    int PoolMax;
public:
    NetWorkSimulation(double nextUpdateLinkTime,double nextUpdateRouteMap,double PoolMax = 10){
        this->nextUpdateLinkTime = nextUpdateLinkTime;
        this->nextUpdateRouteMap = nextUpdateRouteMap;
        this->PoolMax = PoolMax;
        this->PoolSize = 0;
        this->pool = new ThreadPool(int(PoolMax*0.7));
        this->pool->setCallback([this]()->void{
            std::unique_lock<std::mutex> lock(UpdatethreadPool);
            this->releaseThread();
        })
    }

    void initThread();
    vpid releaseThread();

    int initNode(int FType,int Findex);
    void initLink(int FType,int Findex,int TType,int Tindex,std::vector<double>& Time,std::vector<double>& RateList,std::vector<double>& LengthList);
    
    void UpdateLink(double now);
    void UpdateRateMap(double now);

    void createTask(long long TaskID, double startTime, double TaskSize, int fromIndex, int ToIndex);
    void NextFinish(long long &TaskID, double &now);
}
