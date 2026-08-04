// System
#include <stdio.h>
#include <dirent.h>

// Char|String Manipulation
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int isdigitS(char* s){
    size_t l = strlen(s);
    for(size_t i = 0; i < l; i++)
        if(!isdigit(s[i])) return 0;
    return 1;
}

int openProc(DIR** procDir){
    *procDir = opendir("/proc");
    if(*procDir == NULL){
        perror("Opening Directory");
        return -1;
    }
    return 0;
}

void readStatusFile(char* path){
    char buffer[255];
    
    FILE* statusFile = fopen(path, "r");
    if(statusFile == NULL) return;

    while( fgets(buffer, sizeof(buffer), statusFile) ){
        if(strncmp(buffer, "Name: ", 5) == 0)       printf("Name: %s",buffer+6);
        else if(strncmp(buffer, "State:", 6) == 0)  printf("Status: %s",buffer+7);
        else if(strncmp(buffer, "VmSize:", 7) == 0){
            float MemoryUsed = atoi(buffer+8);
            
            // 0 = KB
            // 1 = MB
            // 2 = GB
            int MemorySize = 0;
            while(MemoryUsed > 1024 && MemorySize < 2){
                MemoryUsed /= 1024; MemorySize++;
            }

            char unit[3];
            
            switch (MemorySize) {
                case 0:     strcpy(unit, "KB"); break;
                case 1:     strcpy(unit, "MB"); break;
                case 2:     strcpy(unit, "GB"); break;
                default:    strcpy(unit, "? ");
            }
            printf("Memory Used: %.2f %s\n",MemoryUsed, unit);
        }
    }
    fclose(statusFile);
}

void readComandLine(char* path){
    char buffer[255];
    
    FILE* commandline = fopen(path, "rb");
    if(commandline == NULL) return;

    printf("CommandLine: ");
    int readBytes; char Empty = 1;
    while( (readBytes = fread(buffer, sizeof(char), 255, commandline) )){
        for(size_t i = 0; i < readBytes; i++){
            Empty = 0;
            if(buffer[i] == '\0') putchar(' ');
            else putchar(buffer[i]);
        }
        if(readBytes != 255) break;
    }

    if(Empty){
        puts("[Kernel Process]");
    }
    

    putchar('\n');
    fclose(commandline);
}

void readMemoryMaps(char* path){
    char* buffer;
    size_t len = 0;

    char lastLib[255] = "";
    int headerPrinted = 0;

    FILE* memoryFile = fopen(path, "r");
    if( memoryFile == NULL ) return;

    while( getline(&buffer, &len, memoryFile) != -1) {
        char* libPath = strchr(buffer, '/');
        if(libPath == NULL) continue;
        
        libPath[strlen(libPath) - 1] = '\0';

        if( strstr(libPath, ".so") != NULL){
            if(strcmp(lastLib, libPath) == 0) continue;

            if(!headerPrinted){
                printf("Libraries:\n");
                headerPrinted = 1;
            }

            int isSuspicios = (strstr(libPath, "/tmp") != NULL || strstr(libPath, "/dev/shm") != NULL);

            printf(" -> %s", libPath);
            if(isSuspicios){
                printf("[WARNING] Suspicious Library Path!");
            }
            putchar('\n');
            
            strncpy(lastLib, libPath, sizeof(lastLib) - 1);
            lastLib[sizeof(lastLib) - 1] = '\0';
        }
    }

    fclose(memoryFile);

}

int readProcess(DIR* procDir){
    struct dirent* entry;

    while( (entry = readdir(procDir) )){
        if(isdigitS(entry->d_name)){

            printf("PID: %s \n", entry->d_name);

            char buffer[255];
            
            snprintf(buffer, sizeof(buffer), "/proc/%s/status", entry->d_name);
            readStatusFile(buffer);                
            
            snprintf(buffer, sizeof(buffer), "/proc/%s/cmdline", entry->d_name);
            readComandLine(buffer);

            snprintf(buffer, sizeof(buffer), "/proc/%s/maps", entry->d_name);
            readMemoryMaps(buffer);

            putchar('\n');
        }
    }

    return 0;
}

int main(){
    int programStatus = 0;
    
    DIR* processDirectory;
    programStatus = openProc(&processDirectory);
    if(programStatus == -1){
        printf("Error");
        return 0;
    }

    programStatus = readProcess(processDirectory);
    if(programStatus == -1){
        printf("Error");
        return 0;
    }

    closedir(processDirectory);
    processDirectory = 0;
    return 0;
}