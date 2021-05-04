#ifndef GETREALTIMEDATA_H
#define GETREALTIMEDATA_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QMutex>
#include "tdenginedb.h"

class GetRealTimeData : public QThread {
    Q_OBJECT
public:
    explicit GetRealTimeData(QObject *parent = nullptr);
    explicit GetRealTimeData(const QString &tablename, int interval, QObject *parent = nullptr);
    ~GetRealTimeData();
    void SetTableName(const QString &tbname);
    void SetInterval(int ms);
    void Stop();

protected:
    void run() override;

private:
    QString tableName;  //表名
    int interval;       //订阅查询时间间隔，单位ms
    TAOS *tTaos;        //TDengine建议为每一个线程建立单独的连接，
    TAOS_SUB *tTSub;    //订阅数据库返回的句柄
    TAOS_RES *tRes;     //查询结果集，只在解除订阅后才释放结果集，否则会导致应用崩溃
    QMutex tsubMutex;   //同步tSub

    bool SubscibeTable();
    bool SubscibeTable(const QString &tablename, int interval);

private slots:
    void Consume();

signals:
    void SubscibeSuccess();
    void ConsumeOk(TAOS_RES *res, TAOS_FIELD *fields, int num_fields);
};

#endif // GETREALTIMEDATA_H
