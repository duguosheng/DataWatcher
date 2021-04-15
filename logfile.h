#ifndef LOGFILE_H
#define LOGFILE_H

#include <QObject>
#include <QFile>
#include <QDir>

class LogFile : public QObject {
    Q_OBJECT
public:
    explicit LogFile(QObject *parent = nullptr);
    ~LogFile() = default;
    bool OpenLogFile();
    void CloseLogFile();
    void SaveLog(const QString &log);


private:
    QString logDir;    //日志存储路径
    QString logFileName;//文件名
    QFile logFile;//日志文件

    bool InitLogSystem();

signals:
    void logFailure(const QString &errMsg);

};

#endif // LOGFILE_H
