#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H

#include <stdint.h>

class SerialHandler
{
    private:
    public:
        bool initialized = false;
        uint8_t number = 0;
};

bool SerilInitialize(SerialHandler *handler);

bool SerialWriteByte(SerialHandler *handler,uint8_t data);

uint8_t SerialReadByte(SerialHandler *handler);

#endif