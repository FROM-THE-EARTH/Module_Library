#ifndef INC_SPIRAWHANDLER_H_
#define INC_SPIRAWHANDLER_H_

#include "stdint.h"


void SpiRawInitialize();

void SpiRawWrite(uint8_t data);
uint8_t SpiRawRead();
void SpiRawWrites(uint8_t *data,uint16_t count);
void SpiRawReads(uint8_t *data,uint16_t count);

void SpiAsertSS();
void SpiDeAsertSS();

#endif /* INC_SPIRAWHANDLER_H_ */
