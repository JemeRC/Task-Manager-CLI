#include "procScanner.h"
#include "process/data.h"
#include "process/parser.h"
#include "process/print.h"
#include "stringUtils.h"
#include <stdio.h>
#include <string.h>

int openProc(DIR** procDir){
    *procDir = opendir("/proc");
    if(*procDir == NULL){
        perror("Opening Directory");
        return -1;
    }
    return 0;
}

int readProcess(DIR* procDir, const char suspiciousOnly, const char* targetUser, enum FORMAT formatStyle, FILE* outStream){
    struct dirent* entry;
    ProcessData data;

    while( (entry = readdir(procDir) )){
        if(isdigitS(entry->d_name)){
            initProcessData(&data);

            strcpy(data.pid, entry->d_name);

            char buffer[270];
            
            snprintf(buffer, sizeof(buffer), "/proc/%s/status", entry->d_name);
            readStatusFile(buffer, &data);                
            
            snprintf(buffer, sizeof(buffer), "/proc/%s/cmdline", entry->d_name);
            readCommandLine(buffer, &data);

            snprintf(buffer, sizeof(buffer), "/proc/%s/maps", entry->d_name);
            readMemoryMaps(buffer, &data);

            snprintf(buffer, sizeof(buffer), "/proc/%s/environ", entry->d_name);
            readEnvironFile(buffer, &data);

            char matchesSuspicious = !suspiciousOnly || data.isSuspicious || data.isUnknown;
            char matchesUser = (targetUser == NULL) || (strcmp(data.user, targetUser) == 0);

            if(matchesSuspicious && matchesUser){
                if(formatStyle == FORMAT_TEXT)
                    printProcessText(outStream, &data);
                else if(formatStyle == FORMAT_JSON)
                    printProcessJSON(outStream, &data);
            }

            freeProcessData(&data);
        }
    }

    return 0;
}
