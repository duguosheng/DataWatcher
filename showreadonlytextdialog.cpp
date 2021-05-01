#include "showreadonlytextdialog.h"
#include "ui_showreadonlytextdialog.h"
#include "common.h"
#include <QFile>
#include <QTextStream>

/**
 * @brief ShowReadOnlyTextDialog::ShowReadOnlyTextDialog
 * @param filename 需要显示的文件
 * @param dlgTitle 窗口标题
 * @param parent 父类
 */
ShowReadOnlyTextDialog::ShowReadOnlyTextDialog(const QString &filename, const QString &dlgTitle, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowReadOnlyTextDialog),
    rfile(new QFile(filename)) {
    ui->setupUi(this);
    setWindowTitle(dlgTitle);
    if (rfile->exists()) {
        rfile->open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream readStream(rfile);
        ui->tBrow_showText->setText(QString(readStream.readAll()));
    } else {
        SaveLog("打开文件" + filename + "失败");
        rfile->deleteLater();
    }
}

ShowReadOnlyTextDialog::~ShowReadOnlyTextDialog() {
    if (rfile)
        rfile->deleteLater();
    delete ui;
}
