#ifndef SPIHANDLER_H
#define SPIHANDLER_H

#include "SystemHandler.h"

class SPIHandler{
	private:
	public:
		bool initialized = false;
		uint8_t number = 0;
};

bool SpiInitialize(SPIHandler *handler);

void SpiTransmit(SPIHandler *handler, int pin, uint8_t AddressAndCommand, uint8_t *buffer, uint8_t count);

#endif
