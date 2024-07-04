#ifndef CLASSSTAMRCLIENT
#define CLASSSTAMRCLIENT
#include "net_client.h"
#include "usst_Enum.h"
#include "net_message.h"
#include "usst_Message.h"
#include <vector>
#include <condition_variable>
#include <mutex>
#include <map>
#include <string>
#include <iostream>

class StamrClient :public usst::net::client_interface<UsstMsgTypes>{
private:
    std::mutex is_read;
    std::mutex Muntex;
    std::mutex thread_Muntex;
    std::mutex C;
    std::condition_variable cv;
    std::map<uint32_t, std::thread::id> num_thread;
    std::map<std::thread::id, uint32_t> thread_num;
    uint32_t weakup = -1;

    uint32_t count = 0;
    bool has_read = true;
    
private:
    bool Muntex_check_has_read();
    void Muntex_set_has_read(bool read);
    usst::net::owned_message<UsstMsgTypes> Muntex_check_msg();
    usst::net::message<UsstMsgTypes> Muntex_get_msg();

    void get_thread();
public:
    StamrClient(std::string& host,uint16_t port);
    void init_map();
    usst::net::message<UsstMsgTypes> Wait_function(UsstMsgTypes f_id);
    UsstMsgTypes MobilityInterval();
    UsstMsgTypes ObtainAccessDataRate(int fromType,int fromIndex,int toType,int toIndex);
    UsstMsgTypes ObtainNumOfSats();
    UsstMsgTypes ObtainNumOfGS();
    UsstMsgTypes TransRequest(double tsence);
    UsstMsgTypes TransReply(int32_t EventID,int16_t Tag,int16_t startNodeType,int16_t startNodeIndex,int16_t endNodeType,int16_t endNodeIndex,double Duration,double now);
    UsstMsgTypes ObtainHasAccess();
};

#endif