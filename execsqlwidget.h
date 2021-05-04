#ifndef EXECSQLWIDGET_H
#define EXECSQLWIDGET_H

#include <QWidget>
#include "highlighter.h"
#include "execsqlthread.h"
#include "tdenginedb.h"

namespace Ui {
class ExecSqlWidget;
}

class ExecSqlWidget : public QWidget {
    Q_OBJECT

public:
    enum WorkMode {EditSqlMode, OpenSqlFileMode};
    explicit ExecSqlWidget(QWidget *parent = nullptr,
                           WorkMode mode = EditSqlMode,
                           const QString &sqlfile = QString());
    ~ExecSqlWidget();

private slots:
    void on_pBtn_showResults_toggled(bool checked);
    void ShowResults(TAOS_RES *res, TAOS_FIELD *fields, int numFields);
    void SaveResults(TAOS_RES *res, TAOS_FIELD *fields, int numFields);
    void SqlFailHandle();
    void on_pBtn_refresh_clicked();
    void on_pBtn_saveToFile_clicked();

    void on_pBtn_saveSqlScript_clicked();

private:
    Ui::ExecSqlWidget *ui;
    Highlighter *highlighter;
    ExecSqlThread *sqlThread;
    WorkMode workmode;
    QString saveDataFileName;
    QString saveSqlFileName;
    QString GetSqlCommdand();
};

#endif // EXECSQLWIDGET_H
