#include "getrealtimedata.h"
#include "common.h"

GetRealTimeData::GetRealTimeData(QObject *parent) :
    QThread(parent),
    tTaos(nullptr), tTSub(nullptr),
    isSubscribe(false) {
    connect(&timer, &QTimer::timeout, this, &GetRealTimeData::Consume);
}

GetRealTimeData::GetRealTimeData(const QString &tablename, int ms, QObject *parent) :
    QThread(parent),
    tableName(tablename), interval(ms),
    tTaos(nullptr), tTSub(nullptr),
    isSubscribe(false) {
    connect(&timer, &QTimer::timeout, this, &GetRealTimeData::Consume);
}

void GetRealTimeData::SetTableName(const QString &tbname) {
    tableName = tbname;
}

void GetRealTimeData::SetInterval(int ms) {
    interval = ms;
}

void GetRealTimeData::run() {
    if (SubscibeTable() == false)
        return;
    timer.start(interval);  //同步查询
}

bool GetRealTimeData::SubscibeTable() {
    if (tableName.isEmpty()) {
        SaveLog("表名为空，订阅失败");
        return false;
    }

    tTaos = ConnectDB(dbinfo);
    if (!tTaos) {
        SaveLog("实时查询模块连接服务器失败");
        return false;
    }
    QString sql = "SELECT * FROM " + tableName + " WHERE ts > now-30s;";
    tTSub = taos_subscribe(tTaos, 0, tableName.toStdString().c_str(),
                           sql.toStdString().c_str(),
                           NULL, NULL, interval);
    if (!tTSub) {
        SaveLog("订阅失败：" + tableName);
        return false;
    }

    isSubscribe = true;
    SaveLog("订阅成功：" + tableName + "，查询周期为" + interval + "ms");
    return true;
}

void GetRealTimeData::Consume() {
    TAOS_RES *res = taos_consume(tTSub);
    if (res == NULL) {
        SaveLog("RealTimeDataWidget::RefreshData  查询出错");
        return;
    }
    TAOS_FIELD *fields = taos_fetch_fields(res); //获取每列的属性
    int num_fields = taos_num_fields(res);       //获取列数
    emit ConsumeOk(res, fields, num_fields);
}

