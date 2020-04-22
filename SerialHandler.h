#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H

#include "SystemHandler.h"

typedef struct
{
    uint8_t number = 0;
    bool initialized = false;
} SerialHandle;

bool SerialInitialize(SerialHandle *handle);
void SerialWriteBytes(SerialHandle *handle, uint8_t *buffer, int count);
void SerialReadBytes(SerialHandle *handle, uint8_t *buffer, int count);
#endif