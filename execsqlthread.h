#ifndef EXECSQLTHREAD_H
#define EXECSQLTHREAD_H

#include <QThread>
#include "tdenginedb.h"

class ExecSqlThread : public QThread {
    Q_OBJECT
public:
    explicit ExecSqlThread(QObject *parent = nullptr);
    ~ExecSqlThread();
    void SetSql(const QString &cmd);
    QString GetSql() const;
    void Stop();

protected:
    void run() override;

private:
    QString sql;
    TAOS *tTaos;
    TAOS_RES *tRes;

signals:
    void ExecSqlFail();
    void ExecSqlOk(TAOS *res, TAOS_FIELD *fields, int numFields);
};

#endif // EXECSQLTHREAD_H
