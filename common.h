#ifndef COMMON_H
#define COMMON_H

#include <taos.h>
#include <taoserror.h>
#include <QObject>
#include "filesystem.h"

extern TAOS *taos;
extern LogFile *log;

const char *QStringToChar(const QString &qstr);
void SaveLog(const QString &logString) ;

#endif // COMMON_H
