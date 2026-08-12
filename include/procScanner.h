#ifndef PROCSCANNER_H
#define PROCSCANNER_H

#include <stdio.h>
#include <dirent.h>

enum FORMAT {
    FORMAT_TEXT = 0,
    FORMAT_JSON = 1
};

int openProc(DIR** procDir);
int readProcess(DIR* procDir, const char suspiciousOnly, const char* targetUser, enum FORMAT formatStyle, FILE* outStream);

#endif