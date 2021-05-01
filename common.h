#ifndef COMMON_H
#define COMMON_H

#include <taos.h>
#include <taoserror.h>
#include <QObject>
#include <QSettings>
#include "logfile.h"

extern TAOS *taos;
extern LogFile *runningLog;
extern QSettings *setting;

void SaveLog(const QString &logString);
QString GetLogFileName();
void ProgramExit();

#endif // COMMON_H
