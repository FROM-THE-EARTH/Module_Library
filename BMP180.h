#ifndef INC_BMP180_H_
#define INC_BMP180_H_

#include "stdbool.h"

#define BMP180_ADDR 0x77

//------16bit----------
#define BMP180_CAL_AC1 0xAA
#define BMP180_CAL_AC2 0xAC
#define BMP180_CAL_AC3 0xAE
#define BMP180_CAL_AC4 0xB0
#define BMP180_CAL_AC5 0xB2
#define BMP180_CAL_AC6 0xB4
#define BMP180_CAL_B1 0xB6
#define BMP180_CAL_B2 0xB8
#define BMP180_CAL_MB 0xBA
#define BMP180_CAL_MC 0xBC
#define BMP180_CAL_MD 0xBE
//---------------------

#define BMP180_ID 0xD0
#define BMP180_ID_DEFAULT 0x55
#define BMP180_RESET 0xE0

#define BMP180_OSS 0

#define BMP180_WAITTIME_OSS0 5
#define BMP180_WAITTIME_OSS1 8
#define BMP180_WAITTIME_OSS2 14
#define BMP180_WAITTIME_OSS3 26
#define BMP180_WAITTIME 5

#define BMP180_CTL 0xF4

#define BMP180_OUT_MSB 0xF6
#define BMP180_OUT_LSB 0xF7
#define BMP180_OUT_XLSB 0xF8

bool BMP180_Initialize();
void BMP180_ReadPrs(float *prs);
void BMP180_ReadTmp(float *tmp);

#endif
