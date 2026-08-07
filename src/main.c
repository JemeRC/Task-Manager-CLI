// Modules
#include "procScanner.h"

// Standard Libraries
#include <stdio.h>
#include <string.h>

void printHelp(const char* progName){
    printf("Usage: %s [OPTIONS]\n", progName);
    printf("Options:\n");
    printf("    -h, --help          Show this help message\n");
    printf("    -s, --suspicious    Display only processes with suspicious libraries mapped (/tmp, /dev/shm)\n");
}

int main(int argc, char* argv[]){
    int suspiciousOnly = 0;

    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--suspicious") == 0){
            suspiciousOnly = 1;
        } else if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0){
            printHelp(argv[0]);
            return 0;
        } else {
            printf("Unknown option: %s\n", argv[i]);
            printHelp(argv[0]);
            return 1;
        }
    }
    
    int programStatus = 0;

    DIR* processDirectory;
    programStatus = openProc(&processDirectory);
    if(programStatus == -1){
        perror("Error");
        return 1;
    }

    programStatus = readProcess(processDirectory, suspiciousOnly);
    if(programStatus == -1){
        perror("Error");
        return 1;
    }

    closedir(processDirectory);
    processDirectory = 0;
    return 0;
}