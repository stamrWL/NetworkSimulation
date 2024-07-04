#pragma once
#ifndef _USST_Enum__
#define _USST_Enum__

#include <cstdint>

enum class UsstMsgTypes : uint32_t
{
    ServerAccept = 0,       /*成功连接到服务器回发*/
    ServerDeny,             /*拒绝连接到服务器回发*/
    ServerPing,             /*Ping*/
    FlashSTK,               
    RestartAll,             /*重置计算模块代码*/ 

    MobilityInterval = 10,  /*获取更新位置信息时间间隔*/
    TaskInterval,
    RouteInterval,

    StartTime = 20,         /*获取开始时间*/
    SynTimestamps,          /*同步时间戳*/
    ObtainSatTLE,           /*获取卫星TLE信息*/
    ObtainSatLLA,           /*获取更新卫星LLA位置*/
    ObtainSatJ2000,         /*获取更新卫星J2000位置*/
    ObtainGSLLA,            /*获取地面节点的经纬度、海拔*/
    

    TransRequest = 30,      /*获取通讯请求*/
    TransReply,             /*通讯请求回执*/

    ObtainAccessDataRate = 40,   /*获取网络链路的状态信息*/ 
    ObtainHasAccess = 42,        // 获取已经存在的链路
    ObtainAccessDataRate2 = 43,
    
    ObtainSatsPerPlane = 50  ,   // 获取星座每个平面的卫星数量
    ObtainNumOfPlanes = 51   ,   // 获取星座平面数量
    ObtainConstellation = 52 ,   // 获取星座属性
    ObtainNumOfSats = 53     ,   // 获取卫星的数量
    ObtainNumOfGS = 55       ,   // 获取地面站的数量
    ObtainNameOfGS = 56      ,   // 获取指定index的地面站名称
    ObtainNameOfAllGS = 57       // 获取所有地面站的名称
};

enum class ComRequester : int16_t{
    satellite = 0,
    groundStation,
};

#endif
