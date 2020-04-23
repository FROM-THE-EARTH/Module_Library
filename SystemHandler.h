#define ARDUINO

#ifndef SYSTEMHANDLER_H
#define SYSTEMHANDLER_H

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

#endif