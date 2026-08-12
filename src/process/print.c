#include "process/print.h"
#include <stdio.h>

void printProcessText(FILE* stream, const ProcessData* data){
    fprintf(stream, "Pid:            %s\nPPid:           %s\nName:           %s\nUser:           %s\nStatus:         %s\n",
                        data->pid, data->ppid, data->name, data->user, data->status);

                if(data->memoryUsed < 0.0f){
                    fprintf(stream, "Memory:         N/A \n");
                } else {
                    fprintf(stream, "Memory:         %.2f %s\n", data->memoryUsed, data->memoryUnit);
                }
                fprintf(stream, "Command Line:   %s\n", data->commandLine ? data->commandLine : "N/A");



                if (data->isSuspicious && data->suspiciousLibs){
                    fprintf(stream, "[WARNING] Suspicious Libraries Loaded:\n\t%s", data->suspiciousLibs);
                }

                if(data->isUnknown && data->unknownLibs){
                    fprintf(stream, "[INFO] Unknown Preloaded Libraries (Needs Review):\n\t%s", data->unknownLibs);
                }

                fputc('\n', stream);
}



static void printEscapedString(FILE* stream, const char* str) {
    if (!str) return;
    
    while (*str) {
        switch (*str) {
            case '"':  fprintf(stream, "\\\""); break;
            case '\\': fprintf(stream, "\\\\"); break;
            case '\n': fprintf(stream, "\\n");  break;
            case '\t': fprintf(stream, "\\t");  break;
            default:   fputc(*str, stream);  break;
        }
        str++;
    }
}

static void charToJSON(FILE* stream, const char* name, const char* data){
    fprintf(stream, "\"%s\": \"", name);
    printEscapedString(stream, data ? data : "");
    fprintf(stream, "\"");
}

static void floatToJSON(FILE* stream, const char* name, const float data){
    fprintf(stream, "\"%s\": %.2f", name, data);
}

static void boolToJSON(FILE* stream, const char* name, const char data){
    if( data > 0 ) fprintf(stream, "\"%s\": true", name);
    else           fprintf(stream, "\"%s\": false", name);
}

static void nullToJSON(FILE* stream, const char* name){
    fprintf(stream, "\"%s\": null", name);
}

static void char_vectorToJSON(FILE* stream, const char* name, const char* data){
    fprintf(stream, "\"%s\": [", name);
    
    if (data != NULL && *data != '\0') {
        fprintf(stream, "\"");
        const char* pointer = data;
        while(*pointer != '\0'){
            if(*pointer == '\n'){
                pointer++;
                if(*pointer == '\t') pointer++;
                if(*pointer != '\0') fprintf(stream, "\", \"");
            }
            else{
                switch (*pointer) {
                case '"':  fprintf(stream, "\\\""); break;
                case '\\': fprintf(stream, "\\\\"); break;
                default:   fputc(*pointer, stream); break;
                }
                pointer++;
            }
        }
        fprintf(stream, "\"");
    }

    fprintf(stream, "]");
}

void printProcessJSON(FILE* stream, const ProcessData *data){
    fprintf(stream, "{");
    charToJSON(stream, "pid", data->pid);
    fputc(',', stream);   charToJSON(stream, "ppid", data->ppid);
    fputc(',', stream);   charToJSON(stream, "name", data->name);
    fputc(',', stream);   charToJSON(stream, "user", data->user);
    fputc(',', stream);   charToJSON(stream, "status", data->status);
    
    if( data->memoryUsed == -1.0f){
        fputc(',', stream);   nullToJSON(stream, "memory_used");
        fputc(',', stream);   charToJSON(stream, "memory_unit", "N/A");
    }
    else{
        fputc(',', stream);   floatToJSON(stream, "memory_used", data->memoryUsed);
        fputc(',', stream);   charToJSON(stream, "memory_unit", data->memoryUnit);
    }
    
    fputc(',', stream);   charToJSON(stream, "command_line", data->commandLine);

    fputc(',', stream);   boolToJSON(stream, "is_suspicious", data->isSuspicious);
    fputc(',', stream);   char_vectorToJSON(stream, "suspicious_libs", data->suspiciousLibs);

    fputc(',', stream);   boolToJSON(stream, "is_unknown", data->isUnknown);
    fputc(',', stream);   char_vectorToJSON(stream, "unknown_libs", data->unknownLibs);

    fprintf(stream, "}\n");
}