#ifndef PROCSCANNER_H
#define PROCSCANNER_H

#include <dirent.h>

int openProc(DIR** procDir);
int readProcess(DIR* procDir, const int suspiciousOnly);

#endif