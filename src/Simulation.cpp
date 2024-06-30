#include "Simulation.h"

void NetWorkSimulation::floydWarshall(std::vector<std::vector<double> >& graph, std::vector<std::vector<int>>& nextStep){
    size_t n = graph.size();
    
    #pragma omp parallel for collapse(1)
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (graph[i][k] != INT32_MAX && graph[k][j] != INT32_MAX && graph[i][k] + graph[k][j] < graph[i][j]) {
                    graph[i][j] = graph[i][k] + graph[k][j];
                    nextStep[i][j] = nextStep[i][k];
                }
            }
        }
    }
}

NetWorkSimulation::NetWorkSimulation(double nextUpdateLinkTime, double nextUpdateRouteMap,double stepTime, int PoolMax)
{
    this->nextUpdateLinkTime = nextUpdateLinkTime;
    this->nextUpdateRouteMap = nextUpdateRouteMap;
    this->PoolMax = PoolMax;
    this->PoolSize = 0;
    this->stepTime = stepTime;
    this->pool = new ThreadPool(int(PoolMax * 0.7));
    this->pool->setCallback([this]() -> void
                            {
        std::unique_lock<std::mutex> lock(UpdatethreadPool);
        this->releaseThread(); 
    });
};



void NetWorkSimulation::flashEvent(){
    double UpdateLinkTime = 0;
    double UpdateRouteTime = 0;
    while(true){
        std::unique_lock<std::mutex> lock(TransEvent::mut);
        TransEvent::cv.wait(lock, []{return !TransEvent::needblock &&TransEvent::eventQueue.size() > 0;});
        std::shared_ptr<TransEvent> event = TransEvent::eventQueue.top();
        if(event->getStartTime() > UpdateLinkTime){
            pool->blockRunNext();
            UpdateLink(event->getStartTime());
            UpdateLinkTime += nextUpdateLinkTime;
            pool->unblockRunNext();
        }
        if(event->getStartTime() > UpdateRouteTime){
            pool->blockRunNext();
            UpdateRateMap(event->getStartTime());
            UpdateRouteTime += nextUpdateRouteMap;
            pool->unblockRunNext();
        }
        TransEvent::eventQueue.pop();
        pool->enqueue([](std::shared_ptr<TransEvent> &event){
            event->finish();
            return;
        },std::ref(event));
    }
}

void NetWorkSimulation::initThread(){
    std::unique_lock<std::mutex> lock(UpdatethreadPool);
    this->cv.wait(lock, [this]{return this->PoolSize < this->PoolMax;});
    PoolSize++;
}

void NetWorkSimulation::releaseThread(){
    std::unique_lock<std::mutex> lock(UpdatethreadPool);
    PoolSize--;
    this->cv.notify_all();
}

int NetWorkSimulation::initNode(int FType,int Findex){
    std::pair<int, int> NodeKey = std::make_pair(FType, Findex);
    if (TyI2I.find({FType,Findex}) == TyI2I.end()){
        TyI2I[{FType,Findex}] = NodeCount++;
        Node(TyI2I[{FType,Findex}]);
    }
    return TyI2I[{FType,Findex}];
}

void NetWorkSimulation::initLink(int FType,int Findex,int TType,int Tindex,std::vector<double>& Time,std::vector<double>& RateList,std::vector<double>& LengthList){
    std::pair<int, int> FNodeKey = std::make_pair(FType, Findex);
    std::pair<int, int> SNodeKey = std::make_pair(FType, Findex);
    int fir,sec;
    if (TyI2I.find(FNodeKey) == TyI2I.end()){
        fir = initNode(FType, Findex);
    }else{
        fir = TyI2I[FNodeKey];
    }

    if (TyI2I.find(SNodeKey) == TyI2I.end()){
        sec = initNode(TType, Tindex);
    }else{
        sec = TyI2I[SNodeKey];
    }
    Link::CreateLink(fir, sec, Time, RateList, LengthList,stepTime);
}

void NetWorkSimulation::UpdateLink(double now){
    std::vector<std::thread> threads;
    for(auto &link:Link::linkMap){
        std::thread t(&Link::Update, link.second, now);
        threads.push_back(std::move(t));
    }
    for(auto &t:threads){
        t.join();
    }
}

void NetWorkSimulation::UpdateRateMap(double now){
    size_t n = Node::NodeMap.size();
    std::vector<std::vector<double>> dist(n, std::vector<double>(n, DBL_MAX));
    std::vector<std::vector<int>> next(n, std::vector<int>(n, INT32_MAX));
    for(int i=0;i<n;i++){
        dist[i][i] = 0;
        next[i][i] = i;
    }
    for(auto &link:Link::linkMap){
        int from = link.second->getFrom();
        int to = link.second->getTo();
        dist[from][to] = link.second->getValue(now);
        dist[to][from] = link.second->getValue(now);
        next[from][to] = to;
        next[to][from] = from;
    }
    floydWarshall(dist, next);
    for(auto &node:Node::NodeMap){
        auto routeMap = std::make_shared<std::map<int,int>>();
        for(int i=0;i<n;i++){
            (*routeMap)[i] = next[node.first][i];
        }
        node.second->setRoutMap(routeMap);
    }
}
long long NetWorkSimulation::createTask(long long TaskID, double startTime, double TaskSize, int FType,int Findex,int TType,int Tindex){
    int fromIndex = TyI2I[std::pair<int,int>(FType,Findex)];
    int ToIndex = TyI2I[std::pair<int,int>(TType, Tindex)];
    
    return Task::CreateTask(TaskID, startTime, TaskSize, fromIndex, ToIndex)->getTaskID();
}

void NetWorkSimulation::NextFinish(long long &TaskID, double &now){
    std::unique_lock<std::mutex> lock(Task::mtx);
    Task::cv.wait(lock, []{return Task::TaskHasFinish;});
    std::shared_ptr<Task> task = Task::FinishTask.top();
    TaskID = task->getTaskID();
    now = task->getEndTime();
    Task::FinishTask.pop();
    if(Task::FinishTask.empty()){
        Task::TaskHasFinish = false;
    }
}

void NetWorkSimulation::start(){
    std::unique_lock<std::mutex> lock(TransEvent::mut);
    TransEvent::needblock = false;
    TransEvent::cv.notify_all();
}