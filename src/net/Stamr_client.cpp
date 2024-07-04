#include "Stamr_client.h"

bool StamrClient::Muntex_check_has_read()
{
    std::unique_lock<std::mutex> lock(this->is_read);
    return has_read;
}

void StamrClient::Muntex_set_has_read(bool read)
{
    std::unique_lock<std::mutex> lock(this->is_read);
    has_read = read;
}

usst::net::owned_message<UsstMsgTypes> StamrClient::Muntex_check_msg()
{
    while (1)
    {
        std::unique_lock<std::mutex> lock(this->Muntex);
        if (!this->Incoming().empty() && this->Muntex_check_has_read())
        {
            usst::net::owned_message<UsstMsgTypes> msg = this->Incoming().front();
            return msg;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(5));
    }
}

usst::net::message<UsstMsgTypes> StamrClient::Muntex_get_msg()
{
    while (this->Muntex_check_has_read())
        std::this_thread::sleep_for(std::chrono::microseconds(5));
    std::unique_lock<std::mutex> lock(this->Muntex);
    usst::net::message<UsstMsgTypes> msg = this->Incoming().pop_front().msg;
    weakup = -1;
    this->Muntex_set_has_read(true);
    return msg;
}

void StamrClient::get_thread()
{
    for (;;)
    {
        if ((Muntex_check_has_read()) && this->IsConnected() && !this->Incoming().empty())
        {
            auto msg = this->Muntex_check_msg();
            uint32_t thread_id = msg.msg.header.thread_id;
            if (thread_id == 4294967295 || num_thread.count(thread_id) == 0)
            {
                thread_id = num_thread.begin()->first;
            }
            {
                std::unique_lock<std::mutex> lock(Muntex);
                weakup = thread_id;
            }
            Muntex_set_has_read(false);
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::microseconds(50));
        }
        cv.notify_one();
    }
}

StamrClient::StamrClient(std::string &host, uint16_t port)
{
    has_read = true;
    bool has_connect = usst::net::client_interface<UsstMsgTypes>::Connect(host, port);
    if (has_connect)
    {
        std::thread getThread(&StamrClient::get_thread, this);
        getThread.detach();
    }
}

void StamrClient::init_map()
{
    std::unique_lock<std::mutex> lock(this->Muntex);
    num_thread[this->count] = std::this_thread::get_id();
    thread_num[std::this_thread::get_id()] = this->count;
    this->count++;
    if (this->count == -1)
        this->count = 0;
}

usst::net::message<UsstMsgTypes> StamrClient::Wait_function(UsstMsgTypes f_id)
{
    std::unique_lock<std::mutex> ul(thread_Muntex);
    // 等待声明
    while (true)
    {
        cv.wait(ul, [this]
                { return num_thread.count(weakup) != -1 && std::this_thread::get_id() == num_thread[weakup]; });
        auto msg = Muntex_get_msg();
        if (this->IsConnected())
        {
            if (msg.header.id == UsstMsgTypes::ServerAccept)
            {
                std::cout << "Server Accepted Connection\n";
            }
            else if (msg.header.id == f_id)
            {
                return msg;
            }
            else
            {
                throw("msg error");
            }
        }
        else
        {
            std::cout << "Server Down\n";
        }
    }
}

UsstMsgTypes StamrClient::MobilityInterval(){
    usst::net::message<UsstMsgTypes> msg;
    msg.header.id = UsstMsgTypes::MobilityInterval;
    msg.header.thread_id = thread_num[std::this_thread::get_id()];

    Send(msg);

    return UsstMsgTypes::MobilityInterval;
}

UsstMsgTypes StamrClient::ObtainAccessDataRate(int fromType,int fromIndex,int toType,int toIndex)
{
    usst::net::message<UsstMsgTypes> msg;
    msg.header.id = UsstMsgTypes::ObtainAccessDataRate2;
    msg.header.thread_id = thread_num[std::this_thread::get_id()];

    msg << int16_t(toType) << int16_t(toIndex) << int16_t(fromType) << int16_t(fromIndex);

    Send(msg);

    return UsstMsgTypes::ObtainAccessDataRate2;
}

UsstMsgTypes StamrClient::ObtainNumOfSats()
{
    usst::net::message<UsstMsgTypes> msg;
    msg.header.id = UsstMsgTypes::ObtainNumOfSats;
    msg.header.thread_id = thread_num[std::this_thread::get_id()];

    Send(msg);

    return UsstMsgTypes::ObtainNumOfSats;
}

UsstMsgTypes StamrClient::ObtainNumOfGS()
{
    usst::net::message<UsstMsgTypes> msg;
    msg.header.id = UsstMsgTypes::ObtainNumOfGS;
    msg.header.thread_id = thread_num[std::this_thread::get_id()];

    Send(msg);

    return UsstMsgTypes::ObtainNumOfGS;
}

UsstMsgTypes StamrClient::TransRequest(double tsence)
{
    usst::net::message<UsstMsgTypes> msg;
    msg.header.id = UsstMsgTypes::TransRequest;
    msg.header.thread_id = thread_num[std::this_thread::get_id()];

    msg << tsence;

    Send(msg);

    return UsstMsgTypes::TransRequest;
}

UsstMsgTypes StamrClient::TransReply(int32_t EventID, int16_t Tag, int16_t startNodeType, int16_t startNodeIndex, int16_t endNodeType, int16_t endNodeIndex, double Duration,double nowTime)
{
    usst::net::message<UsstMsgTypes> msg;
    msg.header.id = UsstMsgTypes::TransReply;
    msg.header.thread_id = thread_num[std::this_thread::get_id()];

    msg << EventID << Tag << startNodeType << startNodeIndex << endNodeType << endNodeIndex << nowTime;

    Send(msg);

    return UsstMsgTypes::TransReply;
}

UsstMsgTypes StamrClient::ObtainHasAccess(){
    
    usst::net::message<UsstMsgTypes> msg;
    msg.header.id = UsstMsgTypes::ObtainHasAccess;
    msg.header.thread_id = thread_num[std::this_thread::get_id()];
    Send(msg);

    return UsstMsgTypes::ObtainHasAccess;
}
