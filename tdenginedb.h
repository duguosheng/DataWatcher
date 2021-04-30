#ifndef TDENGINEDB_H
#define TDENGINEDB_H

#include <QObject>
#include "common.h"

extern "C" {
    //位置信息
    struct position {
        double longitude;  //经度
        double latitude;   //纬度
    };
    //液压系统
    struct hydraulic {
        double pressure[2];  //压力
        double temperature;  //油温
        double oil_volume;   //油量
        double flow[2];      //流量
        int vibration[2];    //振动
    };
    //动力系统
    struct dynamic {
        double pressure;     //压力
        double temperature;  //温度
        int vibration;       //振动
    };
    //发送的参数结构体
    struct truck_crane_data {
        int id;         //设备id
        int groupid;    //设备组id
        char type[32];  //设备型号
        time_t time;    //时间信息
        struct position posi;
        struct hydraulic hydr;
        struct dynamic dyna;
    };

    //TDengine登录信息
    struct TDengineConnectInfo {
        QString ip;         //IP地址
        quint16 port;       //端口号
        QString userName;   //用户名
        QString passwd;     //密码
        QString dbName;     //数据库名
    };

}

extern TAOS *taos;
extern struct TDengineConnectInfo dbinfo;
extern struct truck_crane_data craneData;
extern TAOS_BIND params[16];
TAOS *ConnectDB(const TDengineConnectInfo &dbconf);
bool QueryDB(const QString &cmd);
bool QueryDataFromCrane(const QString &tablename);

#endif // TDENGINEDB_H
