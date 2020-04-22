#include "SystemHandler.h"

void DelayMs(unsigned long ms){
    #ifdef ARDUINO
    Delay(ms);
    #endif

    #ifdef STM32
    HAL_Delay(ms);
    #endif
}

void Reset(){

}

void DebugMessage(char* str){
    #ifdef ARDUINO
    Serial.print(str);
    #endif
}
