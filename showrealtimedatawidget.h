#ifndef SHOWREALTIMEDATAWIDGET_H
#define SHOWREALTIMEDATAWIDGET_H

#include <QWidget>
#include <QTimer>
#include "tdenginedb.h"

namespace Ui {
class ShowRealTimeDataWidget;
}

class ShowRealTimeDataWidget : public QWidget {
    Q_OBJECT

public:
    explicit ShowRealTimeDataWidget(QWidget *parent = nullptr);
    ~ShowRealTimeDataWidget();

private slots:
    void on_pBtn_startQuery_clicked();
    void RefreshData();

    void on_pBtn_stopQuery_clicked();

private:
    Ui::ShowRealTimeDataWidget *ui;
    TAOS_SUB *tsub;
    QTimer *consumeTimer;//数据订阅查询周期
};

#endif // REALTIMEDATAWIDGET_H
