#include "stringUtils.h"
#include <string.h>
#include <ctype.h>

int isdigitS(char* s){
    size_t l = strlen(s);
    for(size_t i = 0; i < l; i++)
        if(!isdigit(s[i])) return 0;
    return 1;
}
