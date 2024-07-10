#include "./src/Simulation.h"
#include "./src/net/Stamr_client.h"
#include "./src/net/usst_Message.h"
#include <thread>
#include <vector>
#include <set>

int main(){
    // std::cout<<"hello world"<<std::endl;
    std::string host = "192.168.100.164";
    StamrClient client(host,60000);
    client.init_map();
    int32_t GSsize,SatSize;
    double_t stepTime;

    usst_Message(client.Wait_function(client.MobilityInterval())).MobilityInterval(stepTime);
    usst_Message(client.Wait_function(client.ObtainNumOfGS())).ObtainNumOfGS(GSsize);
    usst_Message(client.Wait_function(client.ObtainNumOfSats())).ObtainNumOfSats(SatSize);

    NetWorkSimulation a(3,stepTime,10);

    // std::vector<double> timesteps = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    // std::vector<double> rateLists = {10,12,11,12,11,14,13,12,11,12,14};
    // std::vector<double> LengthList = {3.12e5,3.12e5,3.12e5,3.12e5,3.12e5,3.12e5,3.12e5,3.12e5,3.12e5,3.12e5,3.12e5};
    std::vector<int16_t> fromType,fromIndex,toType,toIndex;
    
    usst_Message(client.Wait_function(client.ObtainHasAccess())).ObtainHasAccess(fromType,fromIndex,toType,toIndex);
    
    {
        ThreadPool pool(20);
        std::set<std::pair<int,int>> Set;
        std::mutex mtx;

        for(int i = 0; i< fromType.size();i++){
            pool.enqueue([](int i,std::vector<int16_t>& fromType,std::vector<int16_t>& fromIndex, std::vector<int16_t>& toType, std::vector<int16_t>& toIndex,StamrClient& client,NetWorkSimulation& a,std::set<std::pair<int,int>> &Set,std::mutex * mtx){
                std::vector<double> Rate,Delay,Time;
                client.init_map();
                UsstMsgTypes f_id = client.ObtainAccessDataRate(fromType[i],fromIndex[i],toType[i],toIndex[i]);
                auto msg = client.Wait_function(f_id);
                auto msg2 = usst_Message(msg);
                msg2.ObtainAccessDataRate(Time,Rate,Delay);
                {
                    std::unique_lock<std::mutex> lock(*mtx);  
                    int fo = fromType[i]*144 + fromIndex[i];
                    int to = toType[i]*144 + toIndex[i];
                    if(fo < 0||to < 0||fo > 21600||to > 21600){
                        printf("error\n");
                    }

                    if(Set.find({fo,to}) == Set.end()){
                        Set.insert({fo,to});
                        a.initLink(fromType[i],fromIndex[i],toType[i],toIndex[i],Time,Rate,Delay);
                    }
                }
            },i,std::ref(fromType),std::ref(fromIndex),std::ref(toType),std::ref(toIndex),std::ref(client),std::ref(a),std::ref(Set),&mtx);
        }
        pool.start();
        Sleep(1);
        pool.waitAllFinish();
    }

    // for(double time =0 ;time<120;time+=0.2){
    //     a.createTask(int(time/0.2),time,0.1,1,1,1,10);
    // }
    for(int taskid = 0;taskid < 120*int(1/0.2);taskid++){
        a.createTask(taskid,double(taskid)*0.2,0.1,1,9,1,14);
    }

    a.start();
    long long taskID ;
    double endTime = 0;
    std::set<int> Set;
    for(;endTime<119;)
    {
        a.NextFinish(taskID,endTime);
        std::cout<<taskID<<","<<Task::TaskMap[taskID]->getStartTime()<<","<< endTime <<std::endl;

        Set.emplace(taskID);
    }

    // a.NextFinish(taskID,endTime);
    // std::cout<<taskID<<":"<<endTime<<std::endl;

    // a.NextFinish(taskID,endTime);
    // std::cout<<taskID<<":"<<endTime<<std::endl;

    // a.NextFinish(taskID,endTime);
    // a.startEventFlash();
    // std::cout<<taskID<<":"<<endTime<<std::endl;
    return 0;
}