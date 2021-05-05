#include "execsqlwidget.h"
#include "ui_execsqlwidget.h"
#include "common.h"

#include <QDateTime>
#include <QRegularExpression>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>

/**
 * @brief ExecSqlWidget::ExecSqlWidget 构造函数
 * @param parent 父类
 * @param mode 工作模式：直接编辑SQL指令还是打开SQL文件
 * @param sqlfile 如果工作模式为打开SQL文件，应当设置文件名
 */
ExecSqlWidget::ExecSqlWidget(QWidget *parent, WorkMode mode, const QString &sqlfile) :
    QWidget(parent),
    ui(new Ui::ExecSqlWidget),
    sqlThread(nullptr),
    workmode(mode),
    saveSqlFileName(sqlfile) {
    ui->setupUi(this);
    ui->tWidget_showResults->setVisible(false);
    ui->pBtn_refresh->setVisible(false);
    //表格禁用编辑
    ui->tWidget_showResults->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //设置字体
    QFont font;
    font.setFamily("Consolas");
    font.setPointSize(14);
    font.setFixedPitch(true);
    ui->textE_sqlScript->setFont(font);
    highlighter = new Highlighter(ui->textE_sqlScript->document());

    if (workmode == OpenSqlFileMode) {
        //如果文件名为空，改为编辑模式
        if (saveSqlFileName.isEmpty()) {
            workmode = EditSqlMode;
            return;
        }
        QFile saveSqlFile(saveSqlFileName);
        if (saveSqlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&saveSqlFile);
            ui->textE_sqlScript->setText(in.readAll());
            saveSqlFile.close();
        } else {
            SaveLog("打开文件" + saveSqlFileName + "失败");
        }
    } else {
        ui->textE_sqlScript->setText("-- example\n"
                                     "SELECT * FROM d1001 WHERE ts > now - 10s");
    }

    sqlThread = new ExecSqlThread(this);
    connect(sqlThread, &ExecSqlThread::ExecSqlOk, this, &ExecSqlWidget::ShowResults);
    connect(sqlThread, &ExecSqlThread::ExecSqlFail, this, &ExecSqlWidget::SqlFailHandle);
}

ExecSqlWidget::~ExecSqlWidget() {
    delete ui;
}

/**
 * @brief ExecSqlWidget::on_pBtn_showResults_toggled 切换显示面板，
 * @param checked 按键状态
 */
void ExecSqlWidget::on_pBtn_showResults_toggled(bool checked) {
    ui->tWidget_showResults->setVisible(checked);
    ui->pBtn_refresh->setVisible(checked);
    if (checked) {
        sqlThread->SetSql(ui->textE_sqlScript->toPlainText());
        sqlThread->start();
        ui->pBtn_showResults->setText(tr("关闭显示"));
    } else {
        sqlThread->Stop();
        ui->pBtn_showResults->setText(tr("显示结果"));
    }
}

/**
 * @brief ExecSqlWidget::ShowResults 显示执行结果
 * @param res 查询结果集
 * @param fields 列属性
 * @param numFields 列数
 */
void ExecSqlWidget::ShowResults(TAOS_RES *res, TAOS_FIELD *fields, int numFields) {
    ShowDataOnTableWidget(ui->tWidget_showResults, res, fields, numFields);
}

/**
 * @brief ExecSqlWidget::SaveResults 将结果保存到文件
 * @param res 查询结果集
 * @param fields 列属性
 * @param numFields 列数
 */
void ExecSqlWidget::SaveResults(TAOS_RES *res, TAOS_FIELD *fields, int numFields) {
    QFile saveDataFile(saveDataFileName);
    if (saveDataFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&saveDataFile);
        TAOS_ROW row;
        char buf[1024];
        while ((row = taos_fetch_row(res))) {
            taos_print_row(buf, row, fields, numFields);
            QString csvData(buf);
            csvData.replace(' ', ',');
            out << csvData;
            out << endl;
        }
        saveDataFile.close();
        SaveLog("sql执行结果存至" + saveDataFileName);
    } else {
        SaveLog("打开文件" + saveDataFileName + "失败");
    }

    disconnect(sqlThread, &ExecSqlThread::ExecSqlOk, this, &ExecSqlWidget::SaveResults);
    connect(sqlThread, &ExecSqlThread::ExecSqlOk, this, &ExecSqlWidget::SaveResults);
}

/**
 * @brief ExecSqlWidget::SqlFailHandle 查询出错处理
 * 执行出错后恢复原始的信号与槽连接
 * 失败时如果处于保存到文件模式，那么显示模式的信号与槽就无法在SaveResults中重新建立连接
 */
void ExecSqlWidget::SqlFailHandle() {
    disconnect(sqlThread, &ExecSqlThread::ExecSqlOk, this, &ExecSqlWidget::SaveResults);
    connect(sqlThread, &ExecSqlThread::ExecSqlOk, this, &ExecSqlWidget::ShowResults);
}

QString ExecSqlWidget::GetSqlCommdand() {
    return QString();
}

/**
 * @brief ExecSqlWidget::on_pBtn_refresh_clicked 刷新数据
 */
void ExecSqlWidget::on_pBtn_refresh_clicked() {
    sqlThread->Stop();
    sqlThread->SetSql(ui->textE_sqlScript->toPlainText());
    sqlThread->start();
}

/**
 * @brief ExecSqlWidget::on_pBtn_saveToFile_clicked 数据保存到文件
 * 这里只是建立信号与槽的连接，并启动查询线程，真正写入文件在槽函数完成
 */
void ExecSqlWidget::on_pBtn_saveToFile_clicked() {
    saveDataFileName.clear();
    saveDataFileName = QFileDialog::getSaveFileName(
                           this, "查询结果另存为", "D:", tr("CSV文件(*.csv)"));

    if (saveDataFileName.isEmpty())
        return;
    //断开信号连接是必须的，因为显示面板中逐行读取，将改变res的状态
    //taos_fetch_row(res)将无法获取想要的行数据
    disconnect(sqlThread, &ExecSqlThread::ExecSqlOk, this, &ExecSqlWidget::ShowResults);
    connect(sqlThread, &ExecSqlThread::ExecSqlOk, this, &ExecSqlWidget::SaveResults);
    sqlThread->Stop();
    sqlThread->SetSql(ui->textE_sqlScript->toPlainText());
    sqlThread->start();
}

/**
 * @brief ExecSqlWidget::on_pBtn_saveSqlScript_clicked 保存SQL脚本
 */
void ExecSqlWidget::on_pBtn_saveSqlScript_clicked() {
    if (workmode == EditSqlMode) {
        saveSqlFileName = QFileDialog::getSaveFileName(
                              this, "SQL脚本另存为", "D:", tr("SQL文件(*.sql)"));

        if (saveSqlFileName.isEmpty())
            return;
        SaveLog("SQL脚本另存为" + saveSqlFileName);
    }

    QFile saveSqlFile(saveSqlFileName);
    if (saveSqlFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&saveSqlFile);
        out << ui->textE_sqlScript->toPlainText();
        saveSqlFile.close();
    } else {
        SaveLog("打开文件" + saveSqlFileName + "失败");
    }

}
