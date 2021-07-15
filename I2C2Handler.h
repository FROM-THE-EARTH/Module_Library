#ifndef I2C2HANDLER_H
#define I2C2HANDLER_H

#include "stdint.h"
#include "stdbool.h"

void I2c2Initialize();

void I2c2WriteByte(uint8_t add, uint8_t reg, uint8_t data);

uint8_t I2c2ReadByte(uint8_t add, uint8_t reg);

void I2c2ReadBytes(uint8_t add, uint8_t reg, uint8_t *data, uint8_t count);
#endif
