#ifndef SHOWREADONLYTEXTDIALOG_H
#define SHOWREADONLYTEXTDIALOG_H

#include <QDialog>
#include <QReadWriteLock>
#include <QFile>

namespace Ui {
class ShowReadOnlyTextDialog;
}

class ShowReadOnlyTextDialog : public QDialog {
    Q_OBJECT

public:
    explicit ShowReadOnlyTextDialog(const QString &filename, const QString &dlgTitle, QWidget *parent = nullptr, const char *codec = "default");
    ~ShowReadOnlyTextDialog();
    void SetReadLock(QReadWriteLock *rwlock);

private slots:
    void on_pBtn_refresh_clicked();

    void on_pBtn_close_clicked();

private:
    Ui::ShowReadOnlyTextDialog *ui;
    QString fileName;
    QFile rfile;
    const char *codeC;
    QReadWriteLock *rLock;

};

#endif // SHOWREADONLYTEXTDIALOG_H
