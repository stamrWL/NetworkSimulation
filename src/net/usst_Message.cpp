#include "usst_Message.h"

usst_Message::usst_Message(usst::net::message<UsstMsgTypes> msg)
{
    this->msg = std::move(msg);
}

void usst_Message::StartTime(int32_t &year, double_t &day)
{
    if (msg.header.id == UsstMsgTypes::StartTime)
        msg >> day >> year;
}
void usst_Message::SynTimestamps(double_t &tsence)
{
    if (msg.header.id == UsstMsgTypes::SynTimestamps)
        msg >> tsence;
}

void usst_Message::ObtainSatTLE(double_t &meanMotion, double_t &raan, double_t &argPerigee, double_t &dragA, double_t &bstarA,
                                double_t &meanAnom, double_t &incl, double_t &ecc, double_t &altitude, double_t &epochD, int32_t &epochY)
{
    if (msg.header.id == UsstMsgTypes::ObtainSatTLE)
        msg >> meanMotion >> raan >> argPerigee >> dragA >> bstarA >> meanAnom >> incl >> ecc >> altitude >> epochD >> epochY;
}

void usst_Message::MobilityInterval(double_t &stepTime){
    if (msg.header.id == UsstMsgTypes::MobilityInterval)
        msg >> stepTime;
}

void usst_Message::Interval(double_t &Interval)
{
    if (msg.header.id == UsstMsgTypes::MobilityInterval || msg.header.id == UsstMsgTypes::TaskInterval || msg.header.id == UsstMsgTypes::RouteInterval)
        msg >> Interval;
}

void usst_Message::ObtainSatLLA(double_t &tsence, double_t &Lat, double_t &Lon, double_t &Alt)
{
    if (msg.header.id == UsstMsgTypes::ObtainSatLLA)
        msg >> Alt >> Lon >> Lat >> tsence;
}

void usst_Message::ObtainSatJ2000(double_t *&t, int32_t &StartIndex, const int32_t Size)
{
    if (msg.header.id == UsstMsgTypes::ObtainSatJ2000)
    {
        msg >> StartIndex;
        std::memcpy(t, msg.body.data(), Size * 7 * sizeof(double_t));
    }
}

void usst_Message::ObtainGSLLA(double_t &Lat, double_t &Lon, double_t &Alt, char *&Name)
{
    if (msg.header.id == UsstMsgTypes::ObtainGSLLA)
    {
        msg.GetString(Name);
        msg >> Alt >> Lon >> Lat;
    }
}

void usst_Message::TransRequest(int16_t* &FromTypes,int16_t* &FromIndexs,int16_t* &ToTypes,int16_t* &ToIndexs,int64_t* &EventIDs,int16_t* &Tags,double * &StartTime,int64_t* &MessageLens,int16_t& len)
{
    if (msg.header.id == UsstMsgTypes::TransRequest)
    {

        msg >> len;

        FromTypes = (int16_t *)malloc(sizeof(int16_t) * len);
        FromIndexs = (int16_t *)malloc(sizeof(int16_t) * len);
        ToTypes = (int16_t *)malloc(sizeof(int16_t) * len);
        ToIndexs = (int16_t *)malloc(sizeof(int16_t) * len);
        EventIDs = (int64_t *)malloc(sizeof(int64_t) * len);
        Tags = (int16_t *)malloc(sizeof(int16_t) * len);
        StartTime = (double *)malloc(sizeof(double) * len);
        MessageLens = (int64_t *)malloc(sizeof(int64_t) * len);

        for (int i = len - 1; i >= 0; i--)
        {
            msg >> MessageLens[i]>> StartTime[i] >> Tags[i] ;
            msg >> EventIDs[i] >> ToIndexs[i] >> ToTypes[i] ;
            msg >> FromIndexs[i] >> FromTypes[i];
        }
    }
}

void usst_Message::TransReply(bool &needRequest)
{
    if (msg.header.id == UsstMsgTypes::TransReply)
    {
        msg >> needRequest;
    }
}

void usst_Message::ObtainAccessDataRate(double* &Times ,double* &Rates,double* &Range,int16_t& len){
    if (msg.header.id == UsstMsgTypes::ObtainAccessDataRate)
    {
        msg >> len;
    
        Range = (double *)malloc(sizeof(double) * len);
        Times = (double *)malloc(sizeof(double) * len);
        Rates = (double *)malloc(sizeof(double) * len);

        for (int i = len - 1; i >= 0; i--)
        {
            msg >> Range[i]>> Rates[i]>> Times[i] ;
        }
    }
}

    
void usst_Message::ObtainAccessDataRate(std::vector<double> &Time,std::vector<double> &Rate,std::vector<double> &Delay){
if (msg.header.id == UsstMsgTypes::ObtainAccessDataRate2)
    {
        int32_t len = 0;
        double_t time_,rate_,delay_;
        msg >> len;

        Time.resize(len,0);
        Rate.resize(len,0);
        Delay.resize(len,0);

        for (int i = len - 1; i >= 0; i--)
        {
            msg >> delay_>> rate_>> time_;
            Time[i] = time_;
            if(rate_ < 0)
                Rate[i] = DBL_MAX;
            else
                Rate[i] = rate_;
            // Rate[i] = rate_ < 0 ? DBL_MAX: rate_;
            if(delay_ < 0)
                Delay[i] = DBL_MAX;
            else
                Delay[i] = delay_;
            // Delay[i] = delay_ < 0 ? DBL_MAX: delay_;
        }
    }
}

void usst_Message::ObtainNumOfSats(int32_t &size){
    if (msg.header.id == UsstMsgTypes::ObtainNumOfSats)
    {
        msg >> size;
    }
}

void usst_Message::ObtainNumOfGS(int32_t &size){
    if (msg.header.id == UsstMsgTypes::ObtainNumOfGS)
    {
        msg >> size;
    }
}


void usst_Message::ObtainHasAccess(std::vector<int16_t> &fromType,std::vector<int16_t> &fromIndex,std::vector<int16_t> &toType,std::vector<int16_t> &toIndex){
    if (msg.header.id == UsstMsgTypes::ObtainHasAccess)
    {
        int32_t size;
        int16_t ft,fi,tt,ti;
        msg >> size;
        fromType.resize(size,0);
        fromIndex.resize(size,0);
        toType.resize(size,0);
        toIndex.resize(size,0);
        for(int i=size - 1;i>=0;i--){
            msg >> ti >> tt >> fi >> ft;
            fromType[i]= ft;
            fromIndex[i]= fi;
            toType[i]= tt;
            toIndex[i]= ti;
        }
    }
}
