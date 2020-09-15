#ifndef INC_SPIRAWHANDLER_H_
#define INC_SPIRAWHANDLER_H_

#include "stdint.h"

void SpiRawInitialize();

void SpiRawWrite(uint8_t data);
uint8_t SpiRawRead();
void SpiRawWriteMulti(uint8_t *data,uint16_t count);
void SpiRawReadMulti(uint8_t *data,uint16_t count);

void SpiRawAsertSS();
void SpiRawDeAsertSS();

#endif