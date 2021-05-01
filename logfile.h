#ifndef LOGFILE_H
#define LOGFILE_H

#include <QObject>
#include <QFile>
#include <QDir>

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
    QString logDir;    //日志存储路径
    QString logFileName;//文件名
    QFile logFile;//日志文件
    bool OpenLogFile();
    void CloseLogFile();

signals:
    void logFailure(const QString &errMsg);

};

#endif // LOGFILE_H
