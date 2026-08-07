#ifndef PREADINFO_H
#define PREADINFO_H

#include "process/data.h"

void readStatusFile(const char* path, ProcessData* data);
void readCommandLine(const char* path, ProcessData* data);
void readMemoryMaps(const char* path, ProcessData* data);
void readEnvironFile(const char* path, ProcessData* data);

#endif