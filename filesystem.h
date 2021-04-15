#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include <QFile>
#include <QDir>

class FileSystem {
public:
    FileSystem();
};

class LogFile : public QObject {
private:
    QString logDir;    //日志存储路径
    QFile logFile;//日志文件

public:
    LogFile();
    ~LogFile();
    bool OpenLogFile();
    void CloseLogFile();
    bool InitLogDir();
    void SaveLog(const QString &log);

};

#endif // FILESYSTEM_H
