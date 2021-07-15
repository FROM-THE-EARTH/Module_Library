#ifndef I2CHANDLER_H
#define I2CHANDLER_H

#include "stdint.h"
#include "stdbool.h"

void I2cInitialize();

void I2cWriteByte(uint8_t add, uint8_t reg, uint8_t data);

uint8_t I2cReadByte(uint8_t add, uint8_t reg);

void I2cReadBytes(uint8_t add, uint8_t reg, uint8_t *data, uint8_t count);
#endif
