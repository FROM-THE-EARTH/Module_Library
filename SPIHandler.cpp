#include "SPIHandler.h"

#ifdef ARDUINO
	#include <SPI.h>
	#define SPI_CLOCK 1000000
	SPISettings spisettings(SPI_CLOCK, MSBFIRST, SPI_MODE0);
#endif

bool SpiInitialize(SPIHandler *handler)
{
	if(!handler->initialized){
		#ifdef ARDUINO
			if(handler->number == 0){
				SPI.begin();
				pinMode(SS,OUTPUT);
				handler->initialized = true;
			}
		#endif
	}
	return handler->initialized;
}

static void SSpinLow(uint8_t pin){
	#ifdef ARDUINO
    digitalWrite(pin, LOW);
	#endif
}

static void SSpinHigh(uint8_t pin){
	#ifdef ARDUINO
    digitalWrite(pin, HIGH);
	#endif
}

void SpiTransmit(SPIHandler *handler, int pin, uint8_t AddressAndCommand, uint8_t *buffer, uint8_t count)
{
	if(handler->initialized){
		SSpinLow(pin);
		#ifdef ARDUINO
		SPI.beginTransaction(spisettings);
		SPI.transfer(AddressAndCommand);
		SPI.transfer(buffer,count);
		SPI.endTransaction();
		#endif
		SSpinHigh(pin);
	}
}