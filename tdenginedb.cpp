#include "tdenginedb.h"
#include "common.h"
#include <QDebug>
#include <QTableWidget>
#include <QDateTime>


//数据库连接句柄
TAOS *taos = nullptr;
//数据库连接信息
struct TDengineConnectInfo dbinfo;
//参数绑定
TAOS_BIND params[16];
//存储查询数据
struct truck_crane_data craneData;

void InitTaosBindArray();
/**
 * @brief ConnectDB 连接到TDengine数据库服务端
 * @param dbconf 数据库连接配置信息
 * @return 数据库连接句柄，失败返回nullptr
 */
TAOS *ConnectDB(const TDengineConnectInfo &dbconf) {
//    InitTaosBindArray();
    return taos_connect(dbconf.ip.toStdString().c_str(),
                        dbconf.userName.toStdString().c_str(),
                        dbconf.passwd.toStdString().c_str(),
                        dbconf.dbName.toStdString().c_str(),
                        dbconf.port);
}

/**
 * @brief TDengineDB::QueryDB 执行一句sql,无需结果返回
 * @param cmd 执行的SQL指令
 */
bool QueryDB(const QString &cmd) {
    if (!taos)
        return false;
    int i;
    TAOS_RES *pSql = NULL;
    int32_t code = -1;

    //执行3次失败后退出
    for (i = 0; i < 3; i++) {
        if (NULL != pSql) {
            taos_free_result(pSql);
            pSql = NULL;
        }

        pSql = taos_query(taos, cmd.toStdString().c_str());
        code = taos_errno(pSql);

        if (0 == code) {
            break;
        }
    }

    if (code != 0) {
        SaveLog("指令执行失败: " + cmd + ", 原因是: " + taos_errstr(pSql));
        taos_free_result(pSql);
        taos_close(taos);
        return false;
    }

    taos_free_result(pSql);
    return true;
}


/**
 * @brief ParseRow 解析一行的数据
 * @param row 一行数据
 * @param fields 列属性
 * @param numFields 列名
 * @return 列数据的集合
 */
QStringList ParseRow(const TAOS_ROW &row, TAOS_FIELD *fields, int numFields) {
    if (!row || !fields)
        return QStringList();
    char buf[512];
    //获取字符串格式的数据
    taos_print_row(buf, row, fields, numFields);
    QString rowstr(buf);
    //将结果拆分为字符串列表
    QStringList list = rowstr.split(" ");
    return list;
}

/**
 * @brief ShowDataOnTableWidget 在tablewidget上显示查询结果
 * @param tableWidget 表格视图
 * @param res 查询结果集
 * @param fields 列属性
 * @param numFields 列数
 */
void ShowDataOnTableWidget(QTableWidget *tableWidget, TAOS_RES *res, TAOS_FIELD *fields, int numFields) {
    //删除所有行，否则后续的insertRow将导致行越来越多
    tableWidget->setRowCount(0);
    //设置表格列数
    tableWidget->setColumnCount(numFields);
    //设置表头
    for (int i = 0; i < numFields; ++i) {
        QTableWidgetItem *headerH = new QTableWidgetItem(fields[i].name);
        tableWidget->setHorizontalHeaderItem(i, headerH);
    }
    TAOS_ROW row;
    //逐行解析
    for (int i = 0; (row = taos_fetch_row(res)); ++i) {
        tableWidget->insertRow(i);
        //获取解析结果
        QStringList list = ParseRow(row, fields, numFields);
        auto it = list.begin();
        //向列中写入数据
        for (int j = 0; j < numFields; ++j) {
            //时间戳格式
            if (fields[j].type == TSDB_DATA_TYPE_TIMESTAMP) {
                //由于TDengine模式时间戳精度是毫秒，因此要除以1000
                tableWidget->setItem(i, j, new QTableWidgetItem(QDateTime::fromTime_t(
                                         it->toLongLong() / 1000).toString("yyyy-MM-dd hh:mm:ss")));
            } else {
                tableWidget->setItem(i, j, new QTableWidgetItem(*it));
            }
            if (++it == list.end())
                break;
        }
    }
}

/**
 * @brief GetColumnNames 获取结果集的列名
 * @param fields 列属性
 * @param numFields 列数
 * @return 列名列表
 */
bool GetColumnNames(const QString &dbname, const QString &stable, QStringList &list, bool delTimeStampAndTags) {
    if (!taos) {
        SaveLog("数据库未连接，无法解析超级表");
        return false;
    }
    QString sql = "DESCRIBE " + dbname + "." + stable + ";";
    TAOS_RES *res = taos_query(taos, sql.toStdString().c_str());
    TAOS_FIELD *fields = taos_fetch_fields(res);
    int numFields = taos_num_fields(res);
    if (taos_errno(res) != 0) {
        SaveLog("SQL指令：" + sql + "查询失败,原因：" + taos_errstr(res));
        taos_free_result(res);
        return false;
    }
    TAOS_ROW row;
    while ((row = taos_fetch_row(res))) {
        QStringList stableInfo = ParseRow(row, fields, numFields);
        //执行describe指令后，第0列为field属性名，第1列为type数据类型，第2列为length长度，第3列为note备注
        if (delTimeStampAndTags
                && (stableInfo[1].contains("TIMESTAMP", Qt::CaseInsensitive)
                    || stableInfo[3].contains("TAG", Qt::CaseInsensitive)))
            continue;
        list.append(stableInfo[0]);
    }
    taos_free_result(res);
    return true;
}


///**
// * @brief InitTaosBindArray 绑定参数格式
// */
//void InitTaosBindArray() {
//    params[0].buffer_type = TSDB_DATA_TYPE_TIMESTAMP;
//    params[0].buffer_length = sizeof(craneData.time);
//    params[0].buffer = &craneData.time;
//    params[0].length = &params[0].buffer_length;
//    params[0].is_null = NULL;

//    params[1].buffer_type = TSDB_DATA_TYPE_DOUBLE;
//    params[1].buffer_length = sizeof(craneData.posi.longitude);
//    params[1].buffer = &craneData.posi.longitude;
//    params[1].length = &params[1].buffer_length;
//    params[1].is_null = NULL;

//    params[2].buffer_type = TSDB_DATA_TYPE_DOUBLE;
//    params[2].buffer_length = sizeof(craneData.posi.latitude);
//    params[2].buffer = &craneData.posi.latitude;
//    params[2].length = &params[2].buffer_length;
//    params[2].is_null = NULL;

//    params[3].buffer_type = TSDB_DATA_TYPE_DOUBLE;
//    params[3].buffer_length = sizeof(craneData.hydr.temperature);
//    params[3].buffer = &craneData.hydr.temperature;
//    params[3].length = &params[3].buffer_length;
//    params[3].is_null = NULL;

//    params[4].buffer_type = TSDB_DATA_TYPE_DOUBLE;
//    params[4].buffer_length = sizeof(craneData.hydr.oil_volume);
//    params[4].buffer = &craneData.hydr.oil_volume;
//    params[4].length = &params[4].buffer_length;
//    params[4].is_null = NULL;

//    params[5].buffer_type = TSDB_DATA_TYPE_DOUBLE;
//    params[5].buffer_length = sizeof(craneData.hydr.pressure[0]);
//    params[5].buffer = &craneData.hydr.pressure[0];
//    params[5].length = &params[5].buffer_length;
//    params[5].is_null = NULL;

//    params[6].buffer_type = TSDB_DATA_TYPE_DOUBLE;
//    params[6].buffer_length = sizeof(craneData.hydr.flow[0]);
//    params[6].buffer = &craneData.hydr.flow[0];
//    params[6].length = &params[6].buffer_length;
//    params[6].is_null = NULL;

//    params[7].buffer_type = TSDB_DATA_TYPE_INT;
//    params[7].buffer_length = sizeof(craneData.hydr.vibration[0]);
//    params[7].buffer = &craneData.hydr.vibration[0];
//    params[7].length = &params[7].buffer_length;
//    params[7].is_null = NULL;

//    params[8].buffer_type = TSDB_DATA_TYPE_DOUBLE;
//    params[8].buffer_length = sizeof(craneData.hydr.pressure[1]);
//    params[8].buffer = &craneData.hydr.pressure[1];
//    params[8].length = &params[8].buffer_length;
//    params[8].is_null = NULL;

//    params[9].buffer_type = TSDB_DATA_TYPE_DOUBLE;
//    params[9].buffer_length = sizeof(craneData.hydr.flow[1]);
//    params[9].buffer = &craneData.hydr.flow[1];
//    params[9].length = &params[9].buffer_length;
//    params[9].is_null = NULL;

//    params[10].buffer_type = TSDB_DATA_TYPE_INT;
//    params[10].buffer_length = sizeof(craneData.hydr.vibration[1]);
//    params[10].buffer = &craneData.hydr.vibration[1];
//    params[10].length = &params[10].buffer_length;
//    params[10].is_null = NULL;

//    params[11].buffer_type = TSDB_DATA_TYPE_DOUBLE;
//    params[11].buffer_length = sizeof(craneData.dyna.pressure);
//    params[11].buffer = &craneData.dyna.pressure;
//    params[11].length = &params[11].buffer_length;
//    params[11].is_null = NULL;

//    params[12].buffer_type = TSDB_DATA_TYPE_DOUBLE;
//    params[12].buffer_length = sizeof(craneData.dyna.temperature);
//    params[12].buffer = &craneData.dyna.temperature;
//    params[12].length = &params[12].buffer_length;
//    params[12].is_null = NULL;

//    params[13].buffer_type = TSDB_DATA_TYPE_INT;
//    params[13].buffer_length = sizeof(craneData.dyna.vibration);
//    params[13].buffer = &craneData.dyna.vibration;
//    params[13].length = &params[13].buffer_length;
//    params[13].is_null = NULL;

//    params[14].buffer_type = TSDB_DATA_TYPE_INT;
//    params[14].buffer_length = sizeof(craneData.groupid);
//    params[14].buffer = &craneData.groupid;
//    params[14].length = &params[14].buffer_length;
//    params[14].is_null = NULL;

//    params[15].buffer_type = TSDB_DATA_TYPE_BINARY;
//    params[15].buffer_length = sizeof(craneData.type);
//    params[15].buffer = &craneData.type;
//    params[15].length = &params[15].buffer_length;
//    params[15].is_null = NULL;
//}

///**
// * @brief QueryDataFromCrane 查询超级表Crane中的数据
// * @param tablename 表名
// * @return
// */
//bool QueryDataFromCrane(const QString &tablename) {
//    if (!taos)
//        return false;
//    QString sql = "SELECT LAST_ROW(*) FROM " + tablename;
//    TAOS_STMT *stmt = taos_stmt_init(taos);
//    taos_stmt_prepare(stmt, sql.toStdString().c_str(), 0);
//    taos_stmt_bind_param(stmt, params);

//    if (taos_stmt_execute(stmt) != 0) {
//        SaveLog("执行SQL：" + sql + "失败！原因是" + taos_errstr(taos));
//        return false;
//    }
//    taos_stmt_close(stmt);
//    return true;
//}
