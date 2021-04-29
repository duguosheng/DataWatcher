#ifndef TDENGINEDB_H
#define TDENGINEDB_H

#include <QObject>
#include "common.h"
//位置信息
struct position {
    double longitude;  //经度
    double latitude;   //纬度
};
//液压系统
struct hydraulic {
    double pressure;     //压力
    double temperature;  //油温
    double oil_volume;   //油量
    double flow;         //流量
    int vibration;    //振动
};
//动力系统
struct dynamic {
    double pressure;     //压力
    double temperature;  //温度
    int vibration;    //振动
};
//发送的参数结构体
struct truck_crane_data {
    char id[32];  //设备id
    time_t time;  //时间信息
    struct position posi;
    struct hydraulic hydr;
    struct dynamic dyna;
};
bool QueryDB(TAOS *handle, const QString &cmd);

#endif // TDENGINEDB_H
