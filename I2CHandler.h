#ifndef I2CHANDLER_H
#define I2CHANDLER_H

#include "SystemHandler.h"

class I2CHandler
{
	private:
	public:
		bool initialized = false;
		uint8_t number = 0;
};

bool I2cInitialize(I2CHandler *handler);

void I2cWriteByte(I2CHandler *handler, uint8_t add, uint8_t reg, uint8_t data);

uint8_t I2cReadByte(I2CHandler *handler, uint8_t add, uint8_t reg);

void I2cReadBytes(I2CHandler *handler, uint8_t add, uint8_t reg, uint8_t *data, uint8_t count);

#endif