#ifndef I2CHANDLER_H
#define I2CHANDLER_H

class I2CHandler
{
	private:
	public:
		bool initialized = false;
		uint8_t number = 0;
};

#ifdef ARDUINO
	#include "Wire.h"
#endif

bool I2cInitialize(I2CHandler *handler){
	if(!handler->initialized){
		#ifdef ARDUINO
			if(handler->number == 0){
				Wire.begin();
				Wire.setClock( 400000L );
				handler->initialized = true;
			}
		#endif
	}
	return handler->initialized;
}

void I2cWriteByte(I2CHandler *handler, uint8_t add, uint8_t reg, uint8_t data)
{
	if(handler->initialized){
		#ifdef ARDUINO
			Wire.beginTransmission(add);
			Wire.write(reg);
			Wire.write(data);
			Wire.endTransmission();
		#endif
	}
}

uint8_t I2cReadByte(I2CHandler *handler, uint8_t add, uint8_t reg)
{
	uint8_t data = 0;	
	if(handler->initialized){
		#ifdef ARDUINO
			Wire.beginTransmission(add);
			Wire.write(reg);
			Wire.endTransmission(false);
			Wire.requestFrom(add, (uint8_t)1);
			data = Wire.read();
		#endif
	}
	return data;
}

void I2cReadBytes(I2CHandler *handler, uint8_t add, uint8_t reg, uint8_t *data, uint8_t count)
{
	if(handler->initialized){
		#ifdef ARDUINO
			Wire.beginTransmission(add);
			Wire.write(reg);
			Wire.endTransmission(false);
			Wire.requestFrom(add, count);
			for (int i = 0; i < count; i++)
			{
				data[i] = Wire.read();
			}
		#endif
	}
}

#endif