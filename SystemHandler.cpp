#include "SystemHandler.h"

#ifdef ARDUINO
    #include <Arduino.h>
#endif

void DelayMs(unsigned long ms){
    #ifdef ARDUINO
        delay(ms);
    #endif
}

void Reset(){

}

void DebugMessage(char* str){
    #ifdef ARDUINO
        Serial.print(str);
    #endif
}
