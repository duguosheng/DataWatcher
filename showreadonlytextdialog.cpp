#include "showreadonlytextdialog.h"
#include "ui_showreadonlytextdialog.h"
#include "common.h"
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
    fileName(filename),
    rfile(filename),
    codeC(codec),
    rLock(nullptr) {
    ui->setupUi(this);
    setWindowTitle(dlgTitle);
    on_pBtn_refresh_clicked();  //立即刷新一次
}

ShowReadOnlyTextDialog::~ShowReadOnlyTextDialog() {
    delete ui;
}

/**
 * @brief ShowReadOnlyTextDialog::SetReadLock 设置锁
 * @param rwlock 读写锁
 */
void ShowReadOnlyTextDialog::SetReadLock(QReadWriteLock *rwlock) {
    rLock = rwlock;
}

void ShowReadOnlyTextDialog::on_pBtn_refresh_clicked() {
    if (rLock)
        rLock->lockForRead();

    if (rfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream readStream(&rfile);
        //如果用户没有设置编码则保持默认
        if (strcmp(codeC, "default"))
            readStream.setCodec(codeC);
        ui->tBrow_showText->setText(QString(readStream.readAll()));
        rfile.close();

        if (rLock)
            rLock->unlock();
    } else {
        if (rLock)
            rLock->unlock();

        SaveLog("读取文件" + fileName + "失败");
    }

}

void ShowReadOnlyTextDialog::on_pBtn_close_clicked() {
    close();
}
