#ifndef PDATA_H
#define PDATA_H

typedef struct{
    char pid[32];
    char ppid[32];
    char user[64];
    
    char name[256];
    char status[64];
    
    
    float memoryUsed;
    char memoryUnit[4];
    
    char* commandLine;

    char isSuspicious;
    char* suspiciousLibs;

    char isUnknown;
    char* unknownLibs;
} ProcessData;

void initProcessData(ProcessData* data);
void freeProcessData(ProcessData* data);
void appendLib(char** targetBuf, char* flag, const char* libPath);

#endif