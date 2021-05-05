#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_P_triggered();
    void on_action_V_triggered();
    void on_action_Grafana_G_triggered();
    void on_action_AlertManager_M_triggered();
    void on_action_V_2_triggered();
    void on_action_E_triggered();
    void on_action_L_triggered();
    void on_action_U_triggered();
    void on_action_C_triggered();

    void on_action_SQL_E_triggered();

    void on_action_SQL_O_triggered();

    void on_action_C_2_triggered();

private:
    Ui::MainWindow *ui;
    void ClearCentral();
};
#endif // MAINWINDOW_H
