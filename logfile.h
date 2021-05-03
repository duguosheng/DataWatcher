#ifndef LOGFILE_H
#define LOGFILE_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QReadWriteLock>

class LogFile : public QObject {
    Q_OBJECT
public:
    explicit LogFile(QObject *parent = nullptr);
    explicit LogFile(const QString &dir, QObject *parent = nullptr);
    ~LogFile() = default;
    bool SetLogDir(const QString &dir);
    QString GetLogFileName();
    void SaveLog(const QString &log);

private:
    QString logDir;      //日志存储路径
    QString logFileName; //文件名
    QFile logFile;       //日志文件
    bool OpenLogFile();  //打开日志文件
    void CloseLogFile(); //关闭日志文件

signals:
    void logFailure(const QString &errMsg);//这个信号目前没有关联到任何槽函数

};

#endif // LOGFILE_H
