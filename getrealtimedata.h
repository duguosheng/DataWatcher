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
    void run();

private:
    QString tableName;
    int interval;
    //TDengine建议为每一个线程建立单独的连接，
    TAOS *tTaos;
    TAOS_SUB *tTSub;
    TAOS_RES *res;

    QMutex tsubMutex;

    bool SubscibeTable();
    bool SubscibeTable(const QString &tablename, int interval);

private slots:
    void Consume();

signals:
    void SubscibeSuccess();
    void ConsumeOk(TAOS_RES *res, TAOS_FIELD *fields, int num_fields);
};

#endif // GETREALTIMEDATA_H
