#include "process/parser.h"
#include "process/analyze.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>


void readStatusFile(const char* path, ProcessData* data){
    FILE* statusFile = fopen(path, "r");
    if(statusFile == NULL) return;
    
    char buffer[255];

    while( fgets(buffer, sizeof(buffer), statusFile) ){
        if(strncmp(buffer, "Name: ", 5) == 0){
            strcpy(data->name, buffer+6);
            data->name[strcspn(data->name, "\r\n")] = '\0';
        }
        else if(strncmp(buffer, "State:", 6) == 0){
            strcpy(data->status, buffer+7);
            data->status[strcspn(data->status, "\r\n")] = '\0';
        }
        else if(strncmp(buffer, "Uid:",4) == 0){
            uid_t uid;
            if(sscanf(buffer+4, "%u", &uid) == 1){
                struct passwd* pw = getpwuid(uid);
                if(pw != NULL){ 
                    strncpy(data->user, pw->pw_name, sizeof(data->user) - 1);
                    data->user[sizeof(data->user) - 1] = '\0';
                }
                else
                    snprintf(data->user, sizeof(data->user), "%u", uid);
            }
        }
        else if(strncmp(buffer, "PPid:", 5) == 0){
            sscanf(buffer+5, "%d", &(data->ppid));
        }
        else if(strncmp(buffer, "VmRSS:", 6) == 0){
            unsigned long long memoryKB = 0;
            if(sscanf(buffer+6, "%llu", &memoryKB) == 1){
                float MemoryUsed = (float)memoryKB;
                
                int MemorySize = 0; // 0 = KB, 1 = MB, 2 = GB
                while(MemoryUsed > 1024.0f && MemorySize < 2){
                    MemoryUsed /= 1024.0f; MemorySize++;
                }

                char unit[3];
                switch (MemorySize) {
                    case 0:     strcpy(unit, "KB"); break;
                    case 1:     strcpy(unit, "MB"); break;
                    case 2:     strcpy(unit, "GB"); break;
                    default:    strcpy(unit, "? ");
                }

                data->memoryUsed = MemoryUsed;
                strcpy(data->memoryUnit, unit);
            }
        }
    }
    fclose(statusFile);
}
void readCommandLine(const char* path, ProcessData* data){
    FILE* commandline = fopen(path, "rb");
    if(commandline == NULL) return;

    size_t capacity = 256;
    size_t len = 0;
    char* buffer = malloc(capacity);
    
    if(!buffer){
        fclose(commandline);
        return;
    }

    char chunk[255];
    size_t readBytes; 
    char empty = 1;

    while( (readBytes = fread(chunk, sizeof(char), sizeof(chunk), commandline) )){
        empty = 0;
        
        if(len + readBytes + 1 > capacity){
            capacity = (len + readBytes + 1) * 2;
            char* newBuffer = realloc(buffer, capacity);
            if(!newBuffer){
                free(buffer);
                fclose(commandline);
                return;
            }
            buffer = newBuffer;
        }

        for(size_t i = 0; i < readBytes; i++){
            buffer[len++] = (chunk[i] == '\0') ? ' ' : chunk[i];
        }
    }

    if(empty){
        free(buffer);
        data->commandLine = strdup("[Kernel Process]");
    } else{
        buffer[len] = '\0';
        data->commandLine = buffer;
    }

    fclose(commandline);
}
void readMemoryMaps(const char* path, ProcessData* data){
    char* buffer = NULL;
    size_t len = 0;

    char lastLib[255] = "";

    FILE* memoryFile = fopen(path, "r");
    if(memoryFile == NULL) return;

    while( getline(&buffer, &len, memoryFile) != -1) {
        char* libPath = strchr(buffer, '/');
        if(libPath == NULL) continue;
        
        libPath[strcspn(libPath, "\r\n")] = '\0';

        if( strstr(libPath, ".so") != NULL){
            if(strcmp(lastLib, libPath) == 0) continue;

            int isSuspicious = (strstr(libPath, "/tmp") != NULL || strstr(libPath, "/dev/shm") != NULL);

            if(isSuspicious){
                appendLib(&(data->suspiciousLibs), &(data->isSuspicious), libPath);
            }
            
            strncpy(lastLib, libPath, sizeof(lastLib) - 1);
            lastLib[sizeof(lastLib) - 1] = '\0';
        }
    }

    free(buffer);
    fclose(memoryFile);

}
void readEnvironFile(const char* path, ProcessData* data){
    FILE* environFile = fopen(path, "rb");
    if(environFile == NULL) return;

    char* buffer = NULL;
    size_t len = 0;

    while(getdelim(&buffer, &len, '\0', environFile) != -1){
        if(strncmp(buffer, "LD_PRELOAD=", 11) == 0)
            switch(isSuspiciousPreload(buffer)){
                case 1: appendLib(&(data->suspiciousLibs), &(data->isSuspicious), buffer);
                        break;
                case 2: appendLib(&(data->unknownLibs), &(data->isUnknown), buffer);
                        break;
                default:;
            }
    }

    free(buffer);
    fclose(environFile);
}