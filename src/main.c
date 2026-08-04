// System
#include <stdio.h>
#include <dirent.h>

// Char|String Manipulation
#include <ctype.h>
#include <string.h>

int isdigitS(char* s){
    size_t l = strlen(s);
    for(size_t i = 0; i < l; i++)
        if(!isdigit(s[i])) return 0;
    return 1;
}

int listProc(const char *path){
    struct dirent* entry;
    DIR* procDir;
    procDir = opendir(path);
    if(procDir == NULL){
        perror("Opening Directory");
        return -1;
    }
    
    while((entry = readdir(procDir))){
        if(isdigitS(entry->d_name)){
            puts(entry->d_name);
        }
    }
    closedir(procDir);
    procDir = 0;
    entry = 0;
    return 0;
}

int main(){
    listProc("/proc");
    return 0;
}