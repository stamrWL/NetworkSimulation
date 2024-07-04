#ifndef CLASSUSSTMSG
#define CLASSUSSTMSG
#include "usst_Enum.h"
#include <cstdint>
#include <math.h>
#include "net_message.h"

enum class UsstMsgTypes : uint32_t;

class usst_Message
{
public:
    usst_Message(usst::net::message<UsstMsgTypes> msg);

    void StartTime(int32_t &year, double_t &day);

    void SynTimestamps(double_t &tsence);

    void ObtainSatTLE(double_t &meanMotion, double_t &raan, double_t &argPerigee, double_t &dragA, double_t &bstarA,
                      double_t &meanAnom, double_t &incl, double_t &ecc, double_t &altitude, double_t &epochD, int32_t &epochY);

    void MobilityInterval(double_t &stepTime);

    void Interval(double_t &Interval);

    void ObtainSatLLA(double_t &tsence,double_t &Lat,double_t &Lon,double_t &Alt);

    void ObtainSatJ2000(double_t*& t,int& StartIndex ,const int Size);
    
    void ObtainGSLLA(double_t &Lat,double_t &Lon,double_t &Alt,char* &Name);

    void TransRequest(int16_t* &FromTypes,int16_t* &FromIndexs,int16_t* &ToTypes,int16_t* &ToIndexs,int64_t* &EventIDs,int16_t* &Tags,double * &StartTime,int64_t* &MessageLens,int16_t& len);

    void TransReply(bool &needRequest);
    
    void ObtainAccessDataRate(double* &Times ,double* &Rates,double* &Range,int16_t& len);
    
    void ObtainAccessDataRate(std::vector<double> &Time,std::vector<double> &Rate,std::vector<double> &Delay);

    void ObtainNumOfSats(int32_t &size);

    void ObtainNumOfGS(int32_t &size);

    void ObtainHasAccess(std::vector<int16_t> &fromType,std::vector<int16_t> &fromIndex,std::vector<int16_t> &to,std::vector<int16_t> &toIndex);

private:
    usst::net::message<UsstMsgTypes> msg;
};

#endif