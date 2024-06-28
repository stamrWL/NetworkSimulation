#include "Simulation.h"

void NetWorkSimulation::floydWarshall(std::vector<std::vector<double> >& graph, std::vector<vector<int>>& nextStep){
    int n = graph.size();
    
    #pragma omp parallel for collapse(1)
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (graph[i][k] != INF && graph[k][j] != INF && graph[i][k] + graph[k][j] < graph[i][j]) {
                    graph[i][j] = graph[i][k] + graph[k][j];
                    nextStep[i][j] = nextStep[i][k];
                }
            }
        }
    }
}

void NetWorkSimulation::flashEvent(){
    while(true){
        std::unique_lock<std::mutex> lock(TransEvent::mut);
        TransEvent::cv.wait(lock, []{return !TransEvent::needblock &&TransEvent::eventQueue.size() > 0;});
        std::shared_ptr<TransEvent> event = TransEvent::eventQueue.top();
        TransEvent::eventQueue.pop();
        std::thread Th(Event::finish,event,-1);
        Th.detach();
    }
}

void NetWorkSimulation::initThread(){
    std::unique_lock<std::mutex> lock(UpdatethreadPool);
    this->cv.wait(lock, [this]{return this->PoolSize < this->PoolMax;});
    PoolSize++;
}

vpid NetWorkSimulation::releaseThread(){
    std::unique_lock<std::mutex> lock(UpdatethreadPool);
    PoolSize--;
    this->cv.notify_all();
}

int NetWorkSimulation::initNode(int FType,int Findex){
    std::pair<int, int> NodeKey = std::make_pair(FType, Findex);
    if (TyI2I.find(NodeKey) == TyI2I.end()){
        TyI2I[NodeKey] = NodeCount++;
        Node(TyI2I[NodeKey]);
    }
    return TyI2I[NodeKey];
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
    Link::CreateLink(fir, sec, Time, RateList, LengthList);
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
    int n = Node::NodeMap.size();
    std::vector<std::vector<double>> dist(n, vector<int>(n, DBL_MAX));
    std::vector<std::vector<int>> next(n, vector<int>(n, -1));
    for(int i=0;i<n;i++){
        dist[i][i] = 0;
        next[i][i] = i;
    }
    for(auto &link:Link::linkMap){
        int from = link.second->from;
        int to = link.second->to;
        dist[from][to] = link.second->getValue();
        dist[to][from] = link.second->getValue();
        next[from][to] = to;
        next[to][from] = from;
    }
    floydWarshall(dist, next);
    for(auto &node:Node::NodeMap){
        auto routeMap = std::make_shared<std::map<int,int>>();
        for(int i=0;i<n;i++){
            routeMap[i] = next[node.first][i];
        }
        node.second->setRoutMap(routeMap);
    }
}
void NetWorkSimulation::createTask(long long TaskID, double startTime, double TaskSize, int fromIndex, int ToIndex){
    Task::CreateTask(TaskID, startTime, TaskSize, fromIndex, ToIndex);
}

void NetWorkSimulation::NextFinish(long long &TaskID, double &now){
    std::unique_lock<std::mutex> lock(NetWorkSimulation::mtx);
    Task::cv.wait(lock, []{return Task::TaskHasFinish;});
    std::shared_ptr<Task> task = Task::FinishTask.top();
    TaskID = task->getTaskID();
    now = task->getEndTime();
    Task::FinishTask.pop();
    if(Task::FinishTask.empty()){
        Task::TaskHasFinish = false;
    }
}