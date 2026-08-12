#ifndef PPRINT_H
#define PPRINT_H

#include "process/data.h"
#include <stdio.h>

void printProcessText(FILE* stream, const ProcessData* data);
void printProcessJSON(FILE* stream, const ProcessData* data);


#endif