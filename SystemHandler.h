#define ARDUINO
//#define MBED
//#define STM32

#ifndef SYSTEMHANDLER_H
#define SYSTEMHANDLER_H


#ifndef uint8_t
    #define uint8_t char
#endif

#ifndef bool
    #define bool uint8_t
    #define true 1
    #define false 0
#endif


void DelayMs(unsigned long ms);
void Reset();
void DebugMessage(char* str);

#endif