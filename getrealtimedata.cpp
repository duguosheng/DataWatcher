#include "getrealtimedata.h"
#include "common.h"
#include <QDebug>

GetRealTimeData::GetRealTimeData(QObject *parent) :
    QThread(parent),
    tTaos(nullptr), tTSub(nullptr), tRes(nullptr) {
    tTaos = ConnectDB(dbinfo);

}

GetRealTimeData::GetRealTimeData(const QString &tablename, int ms, QObject *parent) :
    QThread(parent),
    tableName(tablename), interval(ms),
    tTaos(nullptr), tTSub(nullptr), tRes(nullptr) {
    tTaos = ConnectDB(dbinfo);

}

GetRealTimeData::~GetRealTimeData() {
    if (tRes) {
        taos_stop_query(tRes);
        taos_free_result(tRes);
    }
    if (isRunning()) {
        SaveLog("线程退出");
        exit();
        wait();
    }
    if (tTSub) {
        taos_unsubscribe(tTSub, 0);
    }
    if (tTaos)
        taos_close(tTaos);
}


/**
 * @brief GetRealTimeData::SetTableName 设置所要查询的表名
 * @param tbname
 */
void GetRealTimeData::SetTableName(const QString &tbname) {
    tableName = tbname;
}

/**
 * @brief GetRealTimeData::SetInterval 设置查询周期，时间不宜过短
 * @param ms 毫秒
 */
void GetRealTimeData::SetInterval(int ms) {
    interval = ms;
}

void GetRealTimeData::run() {
    if (SubscibeTable() == false)
        return;
    emit SubscibeSuccess();

    SaveLog("查询子线程启动");
    QTimer consumeTimer;
    //必须使用Qt::DirectConnection建立信号与槽的连接，否则定时器事件仍会在主线程中执行
    connect(&consumeTimer, &QTimer::timeout, this, &GetRealTimeData::Consume, Qt::DirectConnection);
    Consume();  //立即查询一次
    consumeTimer.start(interval);  //同步查询
    exec();  //开启事件循环，否则定时器无法触发
}

/**
 * @brief GetRealTimeData::SubscibeTable 订阅数据库表
 * @return 订阅是否成功
 */
bool GetRealTimeData::SubscibeTable() {
    if (!tTaos) {
        SaveLog("实时查询模块连接服务器失败");
        return false;
    }
    if (tableName.isEmpty()) {
        SaveLog("表名为空，订阅失败");
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

    SaveLog("订阅数据库表" + tableName + "成功，查询周期为" + QString::number(interval) + "ms");
    return true;
}

/**
 * @brief GetRealTimeData::Stop 停止查询，退出线程
 */
void GetRealTimeData::Stop() {
    if (tTSub) {
        //这里上锁是为了防止点击了暂停查询后tTSub无效，而此时又触发了定时器事件
        //tengine可以识别空指针错误并返回，而传入一个未订阅的非空指针将使程序崩溃
        tsubMutex.lock();
        taos_unsubscribe(tTSub, 0);
        tTSub = nullptr;
        tsubMutex.unlock();
        SaveLog("取消订阅：" + tableName);
    }
    if (tRes)
        taos_free_result(tRes);
    if (isRunning()) {
        SaveLog("线程退出");
        exit();
        wait();
    }
}

/**
 * @brief GetRealTimeData::Consume 查询订阅更新
 */
void GetRealTimeData::Consume() {
    tsubMutex.lock();
    if (!tTSub)
        return;
    tRes = taos_consume(tTSub);
    tsubMutex.unlock();

    if (tRes == nullptr || taos_errno(tRes) != 0) {
        //这里不要使用taos_free_result，订阅查询只在最后才释放
        SaveLog(QString("consume查询出错，错误原因：") + taos_errstr(tRes));
        return;
    }
    TAOS_FIELD *fields = taos_fetch_fields(tRes); //获取每列的属性
    int numFields = taos_num_fields(tRes);
    emit ConsumeOk(tRes, fields, numFields);
}


