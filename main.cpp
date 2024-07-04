#include "./src/Simulation.h"
#include "./src/net/Stamr_client.h"
#include "./src/net/usst_Message.h"

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

    NetWorkSimulation a(2,3,stepTime,5);

    std::vector<double> timesteps = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<double> rateLists = {10,12,11,12,11,14,13,12,11,12,14};
    std::vector<double> LengthList = {3.12e5,3.12e5,3.12e5,3.12e5,3.12e5,3.12e5,3.12e5,3.12e5,3.12e5,3.12e5,3.12e5};
    std::vector<int16_t> fromType,fromIndex,toType,toIndex;
    
    usst_Message(client.Wait_function(client.ObtainHasAccess())).ObtainHasAccess(fromType,fromIndex,toType,toIndex);
    
    for(int i = 0; i< fromType.size();i++){
        std::vector<double> Rate,Delay,Time;
        UsstMsgTypes f_id = client.ObtainAccessDataRate(fromType[i],fromIndex[i],toType[i],toIndex[i]);
        auto msg = client.Wait_function(f_id);
        auto msg2 = usst_Message(msg);
        a.initLink(fromType[i],fromIndex[i],toType[i],toIndex[i],Time,Rate,Delay);
    }
    
    // long long taskID ;
    // double endTime;

    // a.initLink(0,0,0,1,timesteps,rateLists,LengthList);
    // a.initLink(0,1,0,0,timesteps,rateLists,LengthList);
    // a.initLink(0,0,0,2,timesteps,rateLists,LengthList);
    // a.initLink(0,2,0,0,timesteps,rateLists,LengthList);
    // a.initLink(0,2,0,1,timesteps,rateLists,LengthList);
    // a.initLink(0,1,0,2,timesteps,rateLists,LengthList);

    // taskID = a.createTask(1,0.1,0.3,0,0,0,1);
    // taskID = a.createTask(2,0.101,(rand()%200)/150.0,0,1,0,0);
    // taskID = a.createTask(3,0.102,(rand()%200)/150.0,0,0,0,1);
    // taskID = a.createTask(0,0.2,1.2,0,0,0,1);

    // a.start();

    // a.NextFinish(taskID,endTime);
    // std::cout<<taskID<<":"<<endTime<<std::endl;

    // a.NextFinish(taskID,endTime);
    // std::cout<<taskID<<":"<<endTime<<std::endl;

    // a.NextFinish(taskID,endTime);
    // std::cout<<taskID<<":"<<endTime<<std::endl;

    // a.NextFinish(taskID,endTime);
    // a.startEventFlash();
    // std::cout<<taskID<<":"<<endTime<<std::endl;
    return 0;
}