#ifndef SYSTEMHANDLER_H
#define SYSTEMHANDLER_H

#ifndef bool
    #define bool uint8_t
    #define true 1
    #define false 0
#endif

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
    Serial.print(str);
}

#endif