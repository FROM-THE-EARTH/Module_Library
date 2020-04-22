#ifndef I2CHANDLER_H
#define I2CHANDLER_H

#include "SystemHandler.h"

const char InitErrMsg[] = "This I2C number has not been initialized\n";

struct I2CHandle
{
	uint8_t number = 0;
	bool initialized = false;
};

#ifdef ARDUINO
	#include "Wire.h"
#endif

bool I2cInitialize(I2CHandle* handle)
{
	if(handle->initialized == false){
		#ifdef ARDUINO
		if(handle->number == 0){
			Wire.begin();
			Wire.setClock( 400000L );
			handle->initialized = true;
		}else{
			DebugMessage("This I2C number is not available for Arduino\n");
		}
		#endif
	}
	
	return handle->initialized;
}

void I2cWriteByte(I2CHandle* handle, uint8_t add, uint8_t reg, uint8_t data)
{
	if(handle->initialized){
		#ifdef ARDUINO
		Wire.beginTransmission(add);
		Wire.write(reg);
		Wire.write(data);
		Wire.endTransmission();
		#endif
	}else{
		DebugMessage(InitErrMsg);
	}
}

uint8_t I2cReadByte(I2CHandle* handle, uint8_t add, uint8_t reg)
{
	uint8_t data = 0;
	if(handle->initialized){
		#ifdef ARDUINO
		Wire.beginTransmission(add);
		Wire.write(reg);
		Wire.endTransmission(false);
		Wire.requestFrom(add, (uint8_t)1);
		data = Wire.read();
		#endif
	}else{
		DebugMessage(InitErrMsg);
	}
	return data;
}

void I2cReadBytes(I2CHandle* handle, uint8_t add, uint8_t reg, uint8_t *data, uint8_t count)
{
	if(handle->initialized){
		#ifdef ARDUINO
		Wire.beginTransmission(add);
		Wire.write(reg);
		Wire.endTransmission(false);
		Wire.requestFrom(add, count);
		for (int i = 0; i < count; i++){
		data[i] = Wire.read();
		}
		#endif
	}else{
		DebugMessage(InitErrMsg);
	}
}

#endif