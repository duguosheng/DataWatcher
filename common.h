#ifndef COMMON_H
#define COMMON_H

#include <taos.h>
#include <taoserror.h>
#include <QObject>
#include <QSettings>
#include <QReadWriteLock>
#include "logfile.h"

extern TAOS *taos;              //TDengine连接句柄
extern LogFile *runningLog;     //日志文件类指针
extern QReadWriteLock logLock;  //日志文件锁
extern QSettings *setting;      //软件设置

void SaveLog(const QString &logString);
QString GetLogFileName();
void ProgramExit();

#endif // COMMON_H
