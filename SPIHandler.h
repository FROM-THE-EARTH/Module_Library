#ifndef SPIHANDLER_H
#define SPIHANDLER_H

#include "SystemHandler.h"

typedef struct
{
	uint8_t number = 0;
	bool initialized = false;
	uint8_t pins[10];
	uint8_t pinsnum = 0;
}  SPIHandle;


bool SpiInitialize(SPIHandle *handle);
void SpiAddSlaveSelectPin(SPIHandle *handle, uint8_t pin);
void SpiTransmit(SPIHandle *handle, int pin, uint8_t AddressAndCommand, uint8_t *buffer, uint8_t count);

#endif