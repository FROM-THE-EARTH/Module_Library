#include "SystemHandler.h"
#include "SerialHandler.h"

const char InitErrMsg[] = "This serial number has not been initialized\n";

bool SerialInitialize(SerialHandle *handle){
    if(handle->initialized == false){
        #ifdef ARDUINO
        if(handle->number == 0){
            Serial.begin(115200);
        }else{
            DebugMessage("This serial number is not available for Arduino\n");
        }
        #endif
    }
    return handle->initialized;
}

void SerialWriteBytes(SerialHandle *handle, uint8_t *buffer, int count){
    if(handle->initialized){
        #ifdef ARDUINO
        Serial.write(data,count);
        #endif
    }else{
        DebugMessage(InitErrMsg);
    }
}

void SerialReadBytes(SerialHandle *handle, uint8_t *buffer, int count){
    if(handle->initialized){
        #ifdef ARDUINO
        if(Serial.available() >= count){
            for(int i = 0;i < count;i++){
                buffer[i] = Serial.read();
            }
        }else{
            DebugMessage("Received datas on serial is not available to read\n");
        }
        #endif
    }else{
        DebugMessage(InitErrMsg);
    }
}
