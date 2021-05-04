#include "execsqlthread.h"
#include "common.h"
/**
 * @brief ExecSqlThread::ExecSqlThread 构造函数，建立数据库连接
 * @param parent 父类
 */
ExecSqlThread::ExecSqlThread(QObject *parent)
    : QThread(parent), tRes(nullptr) {
    tTaos = ConnectDB(dbinfo);
}

ExecSqlThread::~ExecSqlThread() {
    Stop();
    if (tTaos) {
        taos_close(tTaos);
    }
}

void ExecSqlThread::SetSql(const QString &cmd) {
    sql = cmd;
}

QString ExecSqlThread::GetSql() const {
    return sql;
}

void ExecSqlThread::Stop() {
    if (isRunning()) {
        quit();
        wait();
    }
    if (tRes) {
        taos_free_result(tRes);
        tRes = nullptr;
    }
}

void ExecSqlThread::run() {
    //如果上次执行的结果未释放，则释放它
    if (tRes) {
        taos_free_result(tRes);
        tRes = nullptr;
    }
    if (!tTaos) {
        SaveLog("未连接到数据库，ExecSqlThread启动失败");
        emit ExecSqlFail();
        return;
    }
    tRes = taos_query(tTaos, sql.toStdString().c_str());
    if (taos_errno(tRes) != 0) {
        SaveLog("sql: " + sql + "执行失败，原因是:" + taos_errstr(tRes));
        taos_free_result(tRes);
        tRes = nullptr;
        emit ExecSqlFail();
        return;
    }
    TAOS_FIELD *fields = taos_fetch_fields(tRes); //获取每列的属性
    int numFields = taos_num_fields(tRes);
    emit ExecSqlOk(tRes, fields, numFields);
}


