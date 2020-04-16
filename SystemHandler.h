#ifndef SYSTEMHANDLER_H
#define SYSTEMHANDLER_H


#define ARDUINO	//ここにマイコンを追記していく
//#define MBED


void Delayms(unsigned long ms);
void DebugMessage(char* message)

#ifdef ARDUINO
void Delayms(unsigned long ms){
    delay(ms);
}

void DebugMessage(char* message){
    Serial.println(message);
}
#endif

#ifdef STM32
void Delayms(unsigned long ms){
    HAL_Delay(ms);
}

void DebugMessage(char* message){
    //面倒だった
}
#endif

#endif