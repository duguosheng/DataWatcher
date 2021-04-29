#ifndef COMMON_H
#define COMMON_H

#include <taos.h>
#include <taoserror.h>
#include <QObject>
#include "logfile.h"

extern TAOS *taos;
extern LogFile *log;

const char *QStringToChar(const QString &qstr);
void SaveLog(const QString &logString);
void ProgramExit();

#endif // COMMON_H
