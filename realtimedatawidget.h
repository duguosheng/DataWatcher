#ifndef REALTIMEDATAWIDGET_H
#define REALTIMEDATAWIDGET_H

#include <QWidget>
#include <QTimer>
#include "tdenginedb.h"

namespace Ui {
class RealTimeDataWidget;
}

class RealTimeDataWidget : public QWidget {
    Q_OBJECT

public:
    explicit RealTimeDataWidget(QWidget *parent = nullptr);
    ~RealTimeDataWidget();

private slots:
    void on_pBtn_startQuery_clicked();
    void RefreshData();

    void on_pBtn_stopQuery_clicked();

private:
    Ui::RealTimeDataWidget *ui;
    TAOS_SUB *tsub;
    QTimer *consumeTimer;//数据订阅查询周期
};

#endif // REALTIMEDATAWIDGET_H
