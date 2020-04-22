#ifndef I2CHANDLER_H
#define I2CHANDLER_H

#include "SystemHandler.h"

typedef struct I2CHandle
{
	uint8_t number = 0;
	bool initialized = false;
};

bool I2cInitialize(I2CHandle* handle);
void I2cWriteByte(I2CHandle* handle, uint8_t add, uint8_t reg, uint8_t data);
uint8_t I2cReadByte(I2CHandle* handle, uint8_t add, uint8_t reg);
void I2cReadBytes(I2CHandle* handle, uint8_t add, uint8_t reg, uint8_t *data, uint8_t count);
#endif