#include "SerialHandler.h"

#include "SystemHandler.h"

#ifdef ARDUINO
    #include <Arduino.h>
#endif

bool SerilInitialize(SerialHandler *handler)
{
    if(!handler->initialized){
        #ifdef ARDUINO
            if(handler->number == 0){
                Serial.begin(115200);
                handler->initialized = true;
            }
        #endif
    }
    return handler->initialized;
}

bool SerialWriteByte(SerialHandler *handler,uint8_t data)
{
    if(handler->initialized){
        #ifdef ARDUINO
            Serial.write(data);
        #endif
        return true;
    }
    return false;
}

uint8_t SerialReadByte(SerialHandler *handler)
{
    if(handler->initialized){
        #ifdef ARDUINO
            return Serial.read();
        #endif
    }
    return 0;
}