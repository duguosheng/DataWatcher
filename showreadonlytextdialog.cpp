#include "showreadonlytextdialog.h"
#include "ui_showreadonlytextdialog.h"
#include "common.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
/**
 * @brief ShowReadOnlyTextDialog::ShowReadOnlyTextDialog
 * @param filename 需要显示的文件
 * @param dlgTitle 窗口标题
 * @param parent 父类
 */
ShowReadOnlyTextDialog::ShowReadOnlyTextDialog(const QString &filename, const QString &dlgTitle, QWidget *parent, const char *codec) :
    QDialog(parent),
    ui(new Ui::ShowReadOnlyTextDialog),
    rfile(new QFile(filename)) {
    ui->setupUi(this);
    setWindowTitle(dlgTitle);
    rfile->open(QIODevice::ReadOnly | QIODevice::Text);
    if (rfile->isOpen()) {
        QTextStream readStream(rfile);
        if (strcmp(codec, "default"))
            readStream.setCodec(codec);
        ui->tBrow_showText->setText(QString(readStream.readAll()));
    } else {
        SaveLog("读取文件" + filename + "失败");
        delete rfile;
        rfile = nullptr;
    }

}

ShowReadOnlyTextDialog::~ShowReadOnlyTextDialog() {
    if (rfile)
        rfile->deleteLater();
    delete ui;
}
