#ifndef SPIHANDLER_H
#define SPIHANDLER_H

#include "SystemHandler.h"

struct SPIHandle
{
	uint8_t number = 0;
	bool initialized = false;
	uint8_t pins[10];
	uint8_t pinsnum = 0;
};

#ifdef ARDUINO
	#include <SPI.h>
	SPISettings spisettings(1000000, MSBFIRST, SPI_MODE0);
#endif

bool SpiInitialize(SPIHandle *handle){
	if(handle->initialized == false){
		#ifdef ARDUINO
		if(handle->number == 0){
			SPI.begin();
			pinMode(SS,OUTPUT);
			pinsnum = 1;
			pins[pinsnum - 1] = SS;
			handle->initialized = true;
		}else{
			DebugMessage("This SPI number is not available for Arduino\n");
		}
		#endif
	}

	return handle->initialized;
}

bool pinCheck(SPIHandle *handle,uint8_t pin){
	for(uint8_t i = 0;i < handle->pinsnum;i++){
		if(handle->pins[i] == pin){
			return true;
		}
	}
	return false;
}

void SpiAddSlaveSelectPin(SPIHandle *handle, uint8_t pin){
	if(pinCheck(handle,pin) == false){
		pinsnum++;
		pins[pinsnum - 1] = pin;
		#ifdef ARDUINO
		pinMode(pin,OUTPUT);
		#endif
	}else{
		DebugMessage("This pin has been added as SlaveSelectPin\n");
	}
}

void SSpinLow(uint8_t pin){
	#ifdef ARDUINO
    digitalWrite(pin, LOW);
	#endif
}

void SSpinHigh(uint8_t pin){
	#ifdef ARDUINO
    digitalWrite(pin, HIGH);
	#endif
}

void SpiTransmit(SPIHandle *handle, int pin, uint8_t AddressAndCommand, uint8_t *buffer, uint8_t count)
{
	if(handle->initialized == true){
		if(pinCheck(handle,pin) == true){
			SSpinLow(pin);

			#ifdef ARDUINO
			SPI.beginTransaction(spisettings);
			SPI.transfer(add);
			SPI.transfer(buffer,count);
			SPI.endTransaction();
			#endif

			SSpinHigh(pin);
		}else{
			DebugMessage("This pin has not been added\n");
		}
	}else{
		DebugMessage("This SPI number has no been initialized\n");
	}
}

#endif