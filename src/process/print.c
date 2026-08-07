#include "process/print.h"
#include <stdio.h>

void printProcessText(const ProcessData* data){
    printf("Pid:            %s\nPPid:           %d\nName:           %s\nUser:           %s\nStatus:         %s\n",
                        data->pid, data->ppid, data->name, data->user, data->status);

                if(data->memoryUsed < 0.0f){
                    printf("Memory:         N/A \n");
                } else {
                    printf("Memory:         %.2f %s\n", data->memoryUsed, data->memoryUnit);
                }
                printf("Command Line:   %s\n", data->commandLine);



                if (data->isSuspicious && data->suspiciousLibs){
                    printf("[WARNING] Suspicious Libraries Loaded:\n\t%s", data->suspiciousLibs);
                }

                if(data->isUnknown && data->unknownLibs){
                    printf("[INFO] Unknown Preloaded Libraries (Needs Review):\n\t%s", data->unknownLibs);
                }

                putchar('\n');
}