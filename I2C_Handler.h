#ifndef I2C_HANDLER_H
#define I2C_HANDLER_H

#include <Arduino.h>

void I2C_Write(uint8_t, uint8_t, uint8_t*);
void I2C_Write(uint8_t, uint8_t, uint8_t*, unsigned int);

void I2C_Read(uint8_t, uint8_t, uint8_t*, unsigned int);

void Action_CMD(uint8_t, uint8_t);

#endif
