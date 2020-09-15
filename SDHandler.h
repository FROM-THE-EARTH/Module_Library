#ifndef INC_SDHANDLER_H_
#define INC_SDHANDLER_H_

#include "stdbool.h"
#include "stdint.h"

typedef struct _SD_Card{
	uint32_t sectors;
	bool initialized;
	bool is_SDHC;
}SD_Card;

bool SD_Initialize();

bool SD_WriteMulti(uint32_t sector, uint8_t *data, uint16_t count);

bool SD_ReadMulti(uint32_t sector, uint8_t *data, uint16_t count);

bool SD_Write(uint32_t sector, uint8_t *data);

bool SD_Read(uint32_t sector, uint8_t *data);

#endif