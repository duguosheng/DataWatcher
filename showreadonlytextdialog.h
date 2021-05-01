#ifndef SHOWREADONLYTEXTDIALOG_H
#define SHOWREADONLYTEXTDIALOG_H

#include <QDialog>

namespace Ui {
class ShowReadOnlyTextDialog;
}
class QFile;
class ShowReadOnlyTextDialog : public QDialog {
    Q_OBJECT

public:
    explicit ShowReadOnlyTextDialog(const QString &filename, const QString &dlgTitle, QWidget *parent = nullptr);
    ~ShowReadOnlyTextDialog();

private:
    Ui::ShowReadOnlyTextDialog *ui;
    QFile *rfile;
};

#endif // SHOWREADONLYTEXTDIALOG_H
