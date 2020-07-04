/*
 * SD.h
 *
 *  Created on: Jun 12, 2020
 *      Author: oku_d
 */

#ifndef INC_SDHANDLER_H_
#define INC_SDHANDLER_H_

#include "stdbool.h"
#include "stdint.h"
#include "SPIHandler.h"

typedef struct _SD_Card{
	uint32_t sectors;
	bool initialized;
	bool is_SDHC;
}SD_Card;

bool SDInitialize();

uint32_t SDReadOCR();
uint16_t SDReadStatus();
void SDReadCSD(uint8_t *data);
uint32_t SDReadCONF();

bool SDWrites(uint32_t sector, uint8_t *data, uint16_t count);

bool SDReads(uint32_t sector, uint8_t *data, uint16_t count);

bool SDWrite(uint32_t sector, uint8_t *data);

bool SDRead(uint32_t sector, uint8_t *data);

#endif /* INC_SDHANDLER_H_ */
