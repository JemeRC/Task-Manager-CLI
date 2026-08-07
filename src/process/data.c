#include "process/data.h"
#include <string.h>
#include <stdlib.h>

void initProcessData(ProcessData *data){
    memset(data, 0, sizeof(ProcessData));
    data->commandLine = NULL;
    data->suspiciousLibs = NULL;
    data->memoryUsed = -1.0f;
    strcpy(data->memoryUnit, "N/A");
}

void freeProcessData(ProcessData *data){
    if(data->commandLine){
        free(data->commandLine);
        data->commandLine = NULL;
    }

    if(data->suspiciousLibs){
        free(data->suspiciousLibs);
        data->suspiciousLibs = NULL;
    }

    if (data->unknownLibs) {
        free(data->unknownLibs);
        data->unknownLibs = NULL;
    }
}

void appendLib(char** targetBuf, char* flag, const char* libPath) {
    size_t currentLen = *targetBuf ? strlen(*targetBuf) : 0;
    size_t neededLen = currentLen + strlen(libPath) + 3;

    char* newBuffer = realloc(*targetBuf, neededLen);
    if (!newBuffer) return;

    if (currentLen == 0) {
        newBuffer[0] = '\0';
    }

    *targetBuf = newBuffer;
    strcat(*targetBuf, libPath);
    strcat(*targetBuf, "\n\t");
    *flag = 1;
}