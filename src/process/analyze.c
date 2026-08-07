#include "process/analyze.h"
#include <string.h>

int isSuspiciousPreload(const char* envVar){

    // Whitelisted, Known Libraries
    if(strstr(envVar, "libmozsandbox.so") != NULL){
        return 0;
    }

    // Suspicios Locations
    if (strstr(envVar, "/tmp") != NULL || 
        strstr(envVar, "/dev/shm") != NULL || 
        strstr(envVar, "/var/tmp") != NULL ||
        strstr(envVar, "./") != NULL) {
        return 1;
    }

    // Unknown Library 
    return 2;
}