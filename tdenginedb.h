#ifndef TDENGINEDB_H
#define TDENGINEDB_H

#include <QObject>
#include <taos.h>
#include <taoserror.h>

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

    //描述一个超级表，执行describe stable后的结果样式
    struct DescribeStable {
        QString field;
        QString type;
        QString length;
        QString note;
    };
}

extern TAOS *taos;
extern struct TDengineConnectInfo dbinfo;
extern struct truck_crane_data craneData;
extern TAOS_BIND params[16];
TAOS *ConnectDB(const TDengineConnectInfo &dbconf);
bool QueryDB(const QString &cmd);
bool GetColumnNames(const QString &dbname, const QString &stable, QStringList &list, bool delTimeStampAndTags = false);
//bool QueryDataFromCrane(const QString &tablename);
QStringList ParseRow(const TAOS_ROW &row, TAOS_FIELD *fields, int numFields);
class QTableWidget;
void ShowDataOnTableWidget(QTableWidget *tableWidget, TAOS_RES *res, TAOS_FIELD *fields, int numFields);

#endif // TDENGINEDB_H
