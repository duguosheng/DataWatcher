#include "getrealtimedata.h"
#include "common.h"
#include <QDebug>

GetRealTimeData::GetRealTimeData(QObject *parent) :
    QThread(parent),
    tTaos(nullptr), tTSub(nullptr), res(nullptr) {
    tTaos = ConnectDB(dbinfo);

}

GetRealTimeData::GetRealTimeData(const QString &tablename, int ms, QObject *parent) :
    QThread(parent),
    tableName(tablename), interval(ms),
    tTaos(nullptr), tTSub(nullptr), res(nullptr) {
    tTaos = ConnectDB(dbinfo);

}

GetRealTimeData::~GetRealTimeData() {
    qDebug() << "1";
    if (isRunning()) {
        SaveLog("线程退出");
        exit();
        wait();
    }
    qDebug() << "2";
    if (res) {
        taos_stop_query(res);
        taos_free_result(res);
    }
    if (tTSub) {
        taos_unsubscribe(tTSub, 0);
    }
    if (tTaos)
        taos_close(tTaos);
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
    emit SubscibeSuccess();

    SaveLog("查询子线程启动");
    QTimer consumeTimer;
    //必须使用Qt::DirectConnection建立信号与槽的连接，否则定时器事件仍会在主线程中执行
    connect(&consumeTimer, &QTimer::timeout, this, &GetRealTimeData::Consume, Qt::DirectConnection);
    Consume();  //立即查询一次
    consumeTimer.start(interval);  //同步查询
    exec();  //开启事件循环，否则定时器无法触发
}

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
    if (isRunning()) {
        SaveLog("线程退出");
        exit();
        wait();
    }
}

void GetRealTimeData::Consume() {
    tsubMutex.lock();
    if (!tTSub)
        return;
    res = taos_consume(tTSub);
    tsubMutex.unlock();

    if (res == NULL) {
        SaveLog("RealTimeDataWidget::RefreshData  查询出错");
        return;
    }
    TAOS_FIELD *fields = taos_fetch_fields(res); //获取每列的属性
    int num_fields = taos_num_fields(res);
    emit ConsumeOk(res, fields, num_fields);
}


