#include "Simulation.h"

void NetWorkSimulation::floydWarshall(std::vector<std::vector<double> >& graph, std::vector<std::vector<int>>& nextStep){
    size_t n = graph.size();
    
    #pragma omp parallel for collapse(1)
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (graph[i][k] != DBL_MAX && graph[k][j] != DBL_MAX && graph[i][k] + graph[k][j] < graph[i][j]) {
                    graph[i][j] = graph[i][k] + graph[k][j];
                    nextStep[i][j] = nextStep[i][k];
                    // 最短路径合理？
                    // 会出现回环
                }
            }
        }
    }
}

NetWorkSimulation::NetWorkSimulation( double nextUpdateRouteMap,double stepTime, int PoolMax)
{
    this->nextUpdateLinkTime = stepTime;
    this->nextUpdateRouteMap = nextUpdateRouteMap;
    this->PoolMax = PoolMax;
    this->PoolSize = 0;
    this->stepTime = stepTime;
    this->pool = new ThreadPool(int(PoolMax * 0.7));
    this->pool->setCallback([this]() -> void
                            {
        this->releaseThread(); 
    });
};

void NetWorkSimulation::flashEvent(){
    #if __cplusplus >= 201703L
        std::cout << "C++17" << std::endl;
    #elif __cplusplus >= 201402L
        std::cout << "C++14" << std::endl;
    #elif __cplusplus >= 201103L
        std::cout << "C++11" << std::endl;
    #else
        std::cout << "Pre-C++11" << std::endl;
    #endif

    #ifdef TEST_DEBUG
    std::set<std::tuple<long long,int,int>> test;
    #endif


    double UpdateLinkTime = 0;
    double UpdateRouteTime = 0;
    std::shared_ptr<TransEvent> event;
    while(true){
        {
            std::unique_lock<std::mutex> lock(TransEvent::mut);
            TransEvent::cv.wait(lock, []{return !TransEvent::needblock &&TransEvent::eventQueue.size() > 0 ;});
            event = TransEvent::eventQueue.top();
            TransEvent::eventQueue.pop();
        }
        {
            std::unique_lock<std::mutex> lock(Task::mtx);
            Task::cv.wait(lock,[]{
                return !Task::TaskHasFinish;
            });
        }
        #ifdef TEST_DEBUG
        long long Tid = event->getTask()->getTaskID();
        int FromIndex = event->getFromIndex();
        int ToIndex = event->getToIndex();
        if(test.find({Tid,FromIndex,ToIndex}) != test.end()){
            std::cout << "Event ID: " << Tid << std::endl;
            // assert(false);
        }  
        test.insert({Tid,FromIndex,ToIndex}); 

        // if(event->getStartTime()>= 21){
        //     printf("%lld\n", Tid);
        // }

        // if(event->getTask()->getLastEvent()->getEventid() != event->getEventid()&&event->getTask()->getLastEvent()->getEndTime() - event->getStartTime() > 0.001){
        //     printf(event->getTask()->viewEvent().c_str());
        // }
        #endif  

        
        while(event->getEndTime() + 2*stepTime >= UpdateLinkTime){
            pool->blockRunNext();
            UpdateLink(event->getEndTime() + stepTime);
            UpdateLinkTime += nextUpdateLinkTime;
            pool->unblockRunNext();
        }
        if(event->getEndTime() >= UpdateRouteTime){
            pool->blockRunNext();
            UpdateRateMap(event->getEndTime());
            UpdateRouteTime += nextUpdateRouteMap;
            pool->unblockRunNext();
        }
        // event->finish();
        initThread();
        pool->enqueue([](std::shared_ptr<TransEvent> event){
            event->finish();
            return;
        },event);
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
    TransEvent::cv.notify_all();
}

int NetWorkSimulation::initNode(int FType,int Findex){
    std::pair<int, int> NodeKey = std::make_pair(FType, Findex);
    if (TyI2I.find({FType,Findex}) == TyI2I.end()){
        TyI2I[{FType,Findex}] = NodeCount++;
#ifdef TEST_DEBUG
        I2TyI[TyI2I[{FType,Findex}]] = {FType,Findex};
#endif
        Node::CreateNode(TyI2I[{FType,Findex}]);
    }
    return TyI2I[{FType,Findex}];
}

void NetWorkSimulation::initLink(int FType,int Findex,int TType,int Tindex,std::vector<double>& Time,std::vector<double>& RateList,std::vector<double>& LengthList){
    std::pair<int, int> FNodeKey = std::make_pair(FType, Findex);
    std::pair<int, int> SNodeKey = std::make_pair(TType, Tindex);
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
    for(auto &link_:Link::linkMap){
        for(auto &link:link_.second){
            // link.second->Update(now);
            threads.emplace_back(&Link::Update, link.second, now);
        }
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
    for(auto& link_:Link::linkMap){
        for(auto& link:link_.second){
            int from = link.second->getFrom();
            int to = link.second->getTo();
            dist[from][to] = link.second->getValue(now);
#ifdef TEST_DEBUG
            if(dist[from][to] < 0){
                printf(" ");
                dist[from][to] = link.second->getValue(now);
            }
#endif
            next[from][to] = dist[from][to] != DBL_MAX?to:INT32_MAX;
        }
    }
    // if(now>=21){
    //     printf(" ");
    // }
    floydWarshall(dist, next);
    for(auto &node:Node::NodeMap){
        auto routeMap = std::make_shared<std::map<int,int>>();
        for(int i=0;i<n;i++){
#ifdef TEST_DEBUG
            if(next[node.first][i] == INT32_MAX){
                // 为什么存在未链接的节点
                std::cout<<I2TyI[node.first].first<<","<<I2TyI[node.first].second<<std::endl;
                std::cout<<I2TyI[i].first<<","<<I2TyI[i].second<<std::endl;
            }
#endif
            (*routeMap)[i] = next[node.first][i];
        }
        node.second->setRoutMap(routeMap);
    }
}

long long NetWorkSimulation::createTask(long long TaskID, double startTime, double TaskSize, int FType,int Findex,int TType,int Tindex){
    int fromIndex = TyI2I[std::pair<int,int>(FType,Findex)];
    int ToIndex = TyI2I[std::pair<int,int>(TType, Tindex)];
    auto task = Task::CreateTask(TaskID, startTime, TaskSize, fromIndex, ToIndex);
    Task::cv.notify_all();
    return task->getTaskID();
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
        Task::cv.notify_all();
    }
}

void NetWorkSimulation::startEventFlash(){   
    std::unique_lock<std::mutex> lock(TransEvent::mut);
    TransEvent::needblock = false;
    TransEvent::cv.notify_all();
}

void NetWorkSimulation::start(){
    std::thread FlashThread(&NetWorkSimulation::flashEvent,this);
    FlashThread.detach();
    {
        std::unique_lock<std::mutex> lock(TransEvent::mut);
        TransEvent::needblock = false;
        TransEvent::cv.notify_all();
    }
}

void NetWorkSimulation::blockAll(){
    {
        std::unique_lock<std::mutex> lock(TransEvent::mut);
        TransEvent::needblock = true;
    }
}