#define ARDUINO

#ifndef SYSTEMHANDLER_H
#define SYSTEMHANDLER_H

#include <stdint.h>

void DelayMs(unsigned long ms);

void Reset();

void DebugMessage(char* str);

#endif