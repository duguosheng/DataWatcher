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
    RefreshText();//立即刷新一次

    connect(&fileWatcher, &QFileSystemWatcher::fileChanged,
            this, &ShowReadOnlyTextDialog::RefreshText);
    connect(ui->tBrow_showText, &QTextBrowser::textChanged,
            this, &ShowReadOnlyTextDialog::AutoScroll);

    if (QFile::exists(fileName)) {
        fileWatcher.addPath(fileName);
    } else {
        SaveLog("文件" + fileName + "不存在，无法添加到文件监控器");
    }
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

void ShowReadOnlyTextDialog::RefreshText() {
    if (rLock)
        rLock->lockForRead();

    if (rfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
//        if(rfile.)
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

/**
 * @brief ShowReadOnlyTextDialog::AutoScroll 自动显示最新内容
 */
void ShowReadOnlyTextDialog::AutoScroll() {
    QTextCursor cursor = ui->tBrow_showText->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->tBrow_showText->setTextCursor(cursor);
}
