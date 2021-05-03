#ifndef SHOWREALTIMEDATAWIDGET_H
#define SHOWREALTIMEDATAWIDGET_H

#include <QWidget>
#include <QTimer>
#include "tdenginedb.h"
#include "getrealtimedata.h"

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
    void RefreshData(TAOS_RES *res, TAOS_FIELD *fields, int num_fields);
    void on_pBtn_stopQuery_clicked();
    void SetWidgetToQueryStatus();

private:
    Ui::ShowRealTimeDataWidget *ui;
    GetRealTimeData *queryThread;  //用于更新数据的线程
    bool stopFlag;
};

#endif // REALTIMEDATAWIDGET_H
