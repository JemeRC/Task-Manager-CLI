#define VERSION "v1.0.0"

#include "procScanner.h"
#include <stdio.h>
#include <string.h>

void printHelp(const char* progName){
    printf("Usage: %s [OPTIONS]\n", progName);
    printf("Options:\n");
    printf("    -h, --help          Show this help message\n");
    printf("    -v, --version       Show version information\n");
    printf("    -s, --suspicious    Show only processes with suspicious libraries (/tmp, /dev/shm)\n");
    printf("    -u, --user          Show only processes owned by a specific user\n");
    printf("    -j, --json          Output results in NDJSON format\n");
    printf("    -o, --output        Save output to a specific file instead of the screen\n");
}

int main(int argc, char* argv[]){
    char suspiciousOnly = 0;
    enum FORMAT formatStyle = FORMAT_TEXT;
    const char* targetUser = NULL;
    FILE* outFile = stdout;

    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0){
            printHelp(argv[0]);
            return 0;
        }
        else if(strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0){
            printf("TaskManagerCLI %s\n", VERSION);
            return 0;
        }
        else if(strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--suspicious") == 0){
            suspiciousOnly = 1;
        } 
        else if(strcmp(argv[i], "-u") == 0 || strcmp(argv[i], "--user") == 0){
            if(i + 1 < argc){
                targetUser = argv[++i];
            } else {
                printf("Error: -u requires a user name\n");
                return 1; 
            }
        }
        else if(strcmp(argv[i], "-j") == 0 || strcmp(argv[i], "--json") == 0){
            formatStyle = FORMAT_JSON;
        }
        else if(strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0){
            if(i + 1 < argc){
                outFile = fopen(argv[++i], "w");
                if(outFile == NULL){
                    perror("Output File not Found");
                    return 1;
                }
            } else {
                printf("Error: -o requires a file path\n");
                return 1; 
            }
        } 
        else {
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
        if (outFile != stdout && outFile != NULL)   fclose(outFile);
        return 1;
    }

    programStatus = readProcess(processDirectory, suspiciousOnly, targetUser, formatStyle, outFile);
    if(programStatus == -1){
        perror("Error");
        closedir(processDirectory);
        if (outFile != stdout && outFile != NULL)   fclose(outFile);
        return 1;
    }

    closedir(processDirectory);
    processDirectory = 0;
    if (outFile != stdout && outFile != NULL)   fclose(outFile);
    return 0;
}