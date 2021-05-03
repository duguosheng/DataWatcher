#include "showrealtimedatawidget.h"
#include "ui_showrealtimedatawidget.h"
#include "common.h"
#include "tdenginedb.h"

#include <QDateTime>
#include <QDebug>
#include <QMessageBox>

ShowRealTimeDataWidget::ShowRealTimeDataWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShowRealTimeDataWidget),
    tsub(nullptr) {
    ui->setupUi(this);
    //禁用编辑
    ui->tWidget_realtimeData->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //整行选中
    ui->tWidget_realtimeData->setSelectionBehavior(QAbstractItemView::SelectRows);
    //调整列宽
    ui->tWidget_realtimeData->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    for (int i = 1; i <= 13; ++i) {
        ui->tWidget_realtimeData->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    }
    ui->pBtn_stopQuery->setEnabled(false);
    //创建定时器对象
    consumeTimer = new QTimer(this);
    connect(consumeTimer, &QTimer::timeout, this, &ShowRealTimeDataWidget::RefreshData);
}

ShowRealTimeDataWidget::~ShowRealTimeDataWidget() {
    //取消订阅，不保留订阅信息
    if (tsub) {
        qDebug() << "tsub1";
        taos_unsubscribe(tsub, 0);
        tsub = nullptr;
    }
    delete ui;
}

void ShowRealTimeDataWidget::on_pBtn_startQuery_clicked() {
    QString tablename = "d" + ui->coBox_gid->currentText() + ui->lineE_id->text();
    SaveLog("实时查询模块启动，查询表名为" + tablename);

    //获取设备型号
    char craneType[16];
    QString sql = "SELECT TYPE FROM " + tablename;
    TAOS_RES *res = taos_query(taos, sql.toStdString().c_str());
    if (res == NULL || taos_errno(res) != 0) {
        SaveLog("执行SQL：(" + sql + ")失败, 错误原因:" + QString(taos_errstr(res)));
        QMessageBox::warning(this, "警告", "查询失败，可能原因如下\n"
                             "1.未在C:\\Windows\\System32\\drivers\\etc\\hosts文件中填写(服务器IP 服务器主机名)的映射，"
                             "这是TDengine所要求的，在默认设置下，您需要向该文件中添加"
                             + setting->value("TDengine/ipaddr", "1.15.111.120").toString()
                             + " VM-4-2-centos\n"
                             "2.未连接到互联网\n"
                             "3.数据库中不存在该设备信息");
        // taos_free_result(res);
        return;
    }

    TAOS_FIELD *fields = taos_fetch_fields(res); //获取列属性
    int num_fields = taos_num_fields(res);       //获取列数
    taos_print_row(craneType, taos_fetch_row(res), fields, num_fields);
    ui->label_type->setText("当前设备型号：" + QString(craneType));
    taos_free_result(res);

    sql = "SELECT * FROM " + tablename + " WHERE ts > now-30s;";
    //订阅数据库表，周期为1000ms
    tsub = taos_subscribe(taos, 0, tablename.toStdString().c_str(),
                          sql.toStdString().c_str(),
                          NULL, NULL, 5000);
    if (tsub == NULL) {
        SaveLog("执行SQL：(" + sql + ")失败, 错误原因:" + QString(taos_errstr(res)));
        QMessageBox::warning(this, "警告", "查询失败，可能原因如下\n"
                             "1.未在C:\\Windows\\System32\\drivers\\etc\\hosts文件中填写(服务器IP 服务器主机名)的映射，"
                             "这是TDengine所要求的，在默认设置下，您需要向该文件中添加"
                             + setting->value("TDengine/ipaddr", "1.15.111.120").toString()
                             + " VM-4-2-centos\n"
                             "2.未连接到互联网\n"
                             "3.数据库中不存在该设备信息");
        return;
    }
    //开启定时器，默认为5000ms
    bool ok;
    uint interval = ui->lineE_interval->text().toUInt(&ok);
    if (ok && interval > 5000)
        consumeTimer->start(interval);
    else {
        ui->lineE_interval->setText("5000");
        consumeTimer->start(5000);
    }

    ui->coBox_gid->setEnabled(false);
    ui->lineE_id->setEnabled(false);
    ui->lineE_interval->setEnabled(false);
    ui->pBtn_startQuery->setEnabled(false);
    ui->pBtn_stopQuery->setEnabled(true);

    //立即先触发一次
    RefreshData();
}

void ShowRealTimeDataWidget::RefreshData() {
    TAOS_RES *res = taos_consume(tsub);
    if (res == NULL) {
        SaveLog("RealTimeDataWidget::RefreshData  查询出错");
        return;
    }
    TAOS_FIELD *fields = taos_fetch_fields(res); //获取每列的属性
    int num_fields = taos_num_fields(res);       //获取列数
    int rowNum = ui->tWidget_realtimeData->rowCount(); //表格的最大行数
    int columnNum = ui->tWidget_realtimeData->columnCount(); //表格的最大列数
    TAOS_ROW row;
    //逐行解析
    for (int i = 0; i < rowNum && (row = taos_fetch_row(res)); ++i) {
        char buf[512];
        //获取字符串格式的数据
        taos_print_row(buf, row, fields, num_fields);
        QString rowstr(buf);
        //将结果拆分为字符串列表
        QStringList list = rowstr.split(" ", QString::SkipEmptyParts);
        auto it = list.begin();
        //向列中写入数据
        for (int j = 0; j < columnNum; ++j) {
            //时间戳格式
            if (j == 0) {
                //由于TDengine模式时间戳精度是毫秒，因此要除以1000
                ui->tWidget_realtimeData->setItem(i, j, new QTableWidgetItem(QDateTime::fromTime_t(
                                                      it->toLongLong() / 1000).toString("yyyy-MM-dd hh:mm:ss")));
            } else {
                ui->tWidget_realtimeData->setItem(i, j, new QTableWidgetItem(*it));
            }
            if (++it == list.end())
                break;
        }
    }
}


void ShowRealTimeDataWidget::on_pBtn_stopQuery_clicked() {
    SaveLog("实时查询模块停止");
    //停止定时器
    if (consumeTimer->isActive())
        consumeTimer->stop();

    ui->coBox_gid->setEnabled(true);
    ui->lineE_id->setEnabled(true);
    ui->lineE_interval->setEnabled(true);
    ui->pBtn_startQuery->setEnabled(true);
    ui->pBtn_stopQuery->setEnabled(false);

    //取消订阅，不保留订阅信息
    if (tsub) {
        taos_unsubscribe(tsub, 0);
        tsub = nullptr;
    }
}