#include "showrealtimedatawidget.h"
#include "ui_showrealtimedatawidget.h"
#include "common.h"
#include "tdenginedb.h"

#include <QDateTime>
#include <QDebug>
#include <QMessageBox>

ShowRealTimeDataWidget::ShowRealTimeDataWidget(QWidget *parent):
    QWidget(parent),
    ui(new Ui::ShowRealTimeDataWidget),
    queryThread(nullptr),
    stopFlag(false) {
    ui->setupUi(this);
    //表格禁用编辑
    ui->tWidget_realtimeData->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //整行选中
    ui->tWidget_realtimeData->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->pBtn_stopQuery->setEnabled(false);
    ui->lineE_interval->setValidator(new QIntValidator(100, 9999999, this));

    queryThread = new GetRealTimeData(this);
    connect(queryThread, &GetRealTimeData::SubscibeSuccess,
            this, &ShowRealTimeDataWidget::SetWidgetToQueryStatus);
    connect(queryThread, &GetRealTimeData::ConsumeOk,
            this, &ShowRealTimeDataWidget::RefreshData);
}

ShowRealTimeDataWidget::~ShowRealTimeDataWidget() {
    stopFlag = true;
    queryThread->deleteLater();
    delete ui;
}

void ShowRealTimeDataWidget::on_pBtn_startQuery_clicked() {
    stopFlag = false;
    //毕设中的表名格式是“d+分组号+设备编号”
    QString tablename = "d" + ui->coBox_gid->currentText() + ui->lineE_id->text();

    //获取设备型号
    char craneType[16];
    QString sql = "SELECT TYPE FROM " + tablename;
    TAOS_RES *res = taos_query(taos, sql.toStdString().c_str());
    if (taos_errno(res) != 0) {
        SaveLog("执行SQL：(" + sql + ")失败, 错误原因:" + QString(taos_errstr(res)));
        QMessageBox::warning(this, "警告", "查询失败，可能原因如下\n"
                             "1.未在C:\\Windows\\System32\\drivers\\etc\\hosts文件中填写(服务器IP 服务器主机名)的映射，"
                             "这是TDengine所要求的，在默认设置下，您需要向该文件中添加"
                             + setting->value("TDengine/ipaddr", "1.15.111.120").toString()
                             + " VM-4-2-centos\n"
                             "2.未连接到互联网\n"
                             "3.数据库中不存在该设备信息");
        taos_free_result(res);
        return;
    }

    TAOS_FIELD *fields = taos_fetch_fields(res); //获取列属性
    int num_fields = taos_num_fields(res);       //获取列数

    taos_print_row(craneType, taos_fetch_row(res), fields, num_fields);
    ui->label_type->setText("当前设备型号：" + QString(craneType));
    taos_free_result(res);

    queryThread->SetTableName(tablename);
    bool ok;
    int interval = ui->lineE_interval->text().toInt(&ok);
    //设置最快刷新频率为100ms
    if (ok && interval > 100)
        queryThread->SetInterval(interval);
    else {
        ui->lineE_interval->setText("100");
        queryThread->SetInterval(100);
    }
    queryThread->start();
    SaveLog("实时查询模块启动，查询表名为" + tablename);
}

/**
 * @brief ShowRealTimeDataWidget::RefreshData 更新tablewidget显示数据
 * @param res 查询结果集
 * @param fields 列属性
 * @param num_fields 列数
 */
void ShowRealTimeDataWidget::RefreshData(TAOS_RES *res, TAOS_FIELD *fields, int num_fields) {
    if (stopFlag)//由于信号触发以后槽的执行可能有延迟，为了保留最后一次查询的数据设置了stopFlag
        return;
    ShowDataOnTableWidget(ui->tWidget_realtimeData, res, fields, num_fields);
}

/**
 * @brief ShowRealTimeDataWidget::on_pBtn_stopQuery_clicked 停止查询
 */
void ShowRealTimeDataWidget::on_pBtn_stopQuery_clicked() {
    stopFlag = true;
    SaveLog("实时查询模块停止");

    if (queryThread->isRunning()) {
        queryThread->Stop();
    }

    ui->coBox_gid->setEnabled(true);
    ui->lineE_id->setEnabled(true);
    ui->lineE_interval->setEnabled(true);
    ui->pBtn_startQuery->setEnabled(true);
    ui->pBtn_stopQuery->setEnabled(false);
}

void ShowRealTimeDataWidget::SetWidgetToQueryStatus() {
    ui->coBox_gid->setEnabled(false);
    ui->lineE_id->setEnabled(false);
    ui->lineE_interval->setEnabled(false);
    ui->pBtn_startQuery->setEnabled(false);
    ui->pBtn_stopQuery->setEnabled(true);
}
