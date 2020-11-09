#include "BMP180.h"
#include "I2C2Handler.h"

const uint8_t BMP180_CTL_CMD1 = 0x2E;
const uint8_t BMP180_CTL_CMD2 = 0x34 + (BMP180_OSS << 6);
uint8_t BMP180_WAITTIME_OSS;

int16_t ac1, ac2, ac3, b1, b2, mb, mc, md;
uint16_t ac4, ac5, ac6;

static void Delay(uint32_t ms){
	HAL_Delay(ms);
}

bool BMP180_Initialize(){
	uint8_t tempRegVal;
	I2c2Initialize();
	tempRegVal = I2c2ReadByte(BMP180_ADDR,BMP180_ID);
	if(tempRegVal != BMP180_ID_DEFAULT)return false;

	ac1 = I2c2ReadByte(BMP180_ADDR,BMP180_CAL_AC1) << 8 | I2c2ReadByte(BMP180_ADDR,BMP180_CAL_AC1 + 1);
	ac2 = I2c2ReadByte(BMP180_ADDR,BMP180_CAL_AC2) << 8 | I2c2ReadByte(BMP180_ADDR,BMP180_CAL_AC2 + 1);
	ac3 = I2c2ReadByte(BMP180_ADDR,BMP180_CAL_AC3) << 8 | I2c2ReadByte(BMP180_ADDR,BMP180_CAL_AC3 + 1);
	ac4 = I2c2ReadByte(BMP180_ADDR,BMP180_CAL_AC4) << 8 | I2c2ReadByte(BMP180_ADDR,BMP180_CAL_AC4 + 1);
	ac5 = I2c2ReadByte(BMP180_ADDR,BMP180_CAL_AC5) << 8 | I2c2ReadByte(BMP180_ADDR,BMP180_CAL_AC5 + 1);
	ac6 = I2c2ReadByte(BMP180_ADDR,BMP180_CAL_AC6) << 8 | I2c2ReadByte(BMP180_ADDR,BMP180_CAL_AC6 + 1);
	b1 = I2c2ReadByte(BMP180_ADDR,BMP180_CAL_B1) << 8 | I2c2ReadByte(BMP180_ADDR,BMP180_CAL_B1 + 1);
	b2 = I2c2ReadByte(BMP180_ADDR,BMP180_CAL_B2) << 8 | I2c2ReadByte(BMP180_ADDR,BMP180_CAL_B2 + 1);
	mb = I2c2ReadByte(BMP180_ADDR,BMP180_CAL_MB) << 8 | I2c2ReadByte(BMP180_ADDR,BMP180_CAL_MB + 1);
	mc = I2c2ReadByte(BMP180_ADDR,BMP180_CAL_MC) << 8 | I2c2ReadByte(BMP180_ADDR,BMP180_CAL_MC + 1);
	md = I2c2ReadByte(BMP180_ADDR,BMP180_CAL_MD) << 8 | I2c2ReadByte(BMP180_ADDR,BMP180_CAL_MD + 1);

	switch(BMP180_OSS){
	case 0:
		BMP180_WAITTIME_OSS = BMP180_WAITTIME_OSS0;
		break;
	case 1:
		BMP180_WAITTIME_OSS = BMP180_WAITTIME_OSS1;
		break;
	case 2:
		BMP180_WAITTIME_OSS = BMP180_WAITTIME_OSS2;
		break;
	case 3:
		BMP180_WAITTIME_OSS = BMP180_WAITTIME_OSS3;
		break;
	}
	return true;
}

static uint16_t ReadUT(){
	uint8_t ut[2];
	uint16_t UT;
	I2c2WriteByte(BMP180_ADDR,BMP180_CTL,BMP180_CTL_CMD1);
	Delay(BMP180_WAITTIME);
	I2c2ReadBytes(BMP180_ADDR,BMP180_OUT_MSB,ut,2);
	UT = ut[0] << 8 | ut[1];
	return UT;
}

static uint32_t ReadUP(){
	uint8_t up[3];
	uint32_t UP;
	I2c2WriteByte(BMP180_ADDR,BMP180_CTL,BMP180_CTL_CMD2);
	Delay(BMP180_WAITTIME_OSS);
	I2c2ReadBytes(BMP180_ADDR,BMP180_OUT_MSB,up,3);
	UP = (uint32_t)up[0] << 16 | (uint16_t)up[1] << 8 | up[2];
	UP = UP >> (8 - BMP180_OSS);
	return UP;
}

static int32_t calcB5(uint16_t ut){
	int32_t x1 = (ut - (int32_t)ac6) * ((int32_t)ac5) >> 15;
	int32_t x2 = ((int32_t)mc << 11) / (x1 + (int32_t)md);
	return x1 + x2;
}

static int32_t calcT(uint32_t ut){
	int32_t b5 = calcB5(ut);
	return (b5 + 8) >> 4;
}

static int32_t calcP(uint16_t ut,uint32_t up){
	int32_t p;
	int32_t b5 = calcB5(ut);
	int32_t b6 = b5 - 4000;
	int32_t x1 = ((int32_t)b2 * ((b6 * b6) >> 12)) >> 11;
	int32_t x2 = ((int32_t)ac2 * b6) >> 11;
	int32_t x3 = x1 + x2;
	int32_t b3 = ((((int32_t)ac1 * 4 + x3) << BMP180_OSS) + 2) / 4;

	x1 = ((int32_t)ac3 * b6) >> 13;
	x2 = ((int32_t)b1 * ((b6 * b6) >> 12)) >> 16;
	x3 = ((x1 + x2) + 2) >> 2;
	uint32_t b4 = ((uint32_t)ac4 * (uint32_t)(x3 + 32768)) >> 15;
	uint32_t b7 = ((uint32_t)up - b3) * (uint32_t)(50000UL >> BMP180_OSS);

	if (b7 < 0x80000000) {
		p = (b7 * 2) / b4;
	} else {
		p = (b7 / b4) * 2;
	}
	x1 = (p >> 8) * (p >> 8);
	x1 = (x1 * 3038) >> 16;
	x2 = (-7357 * p) >> 16;

	p = p + ((x1 + x2 + (int32_t)3791) >> 4);
	return p;
}

void BMP180_ReadPrs(float *prs){
	uint16_t ut;
	uint32_t up;
	ut = ReadUT();
	up = ReadUP();
	*prs = (float)calcP(ut,up) / 100;
}

void BMP180_ReadTmp(float *tmp){
	uint16_t ut;
	ut = ReadUT();
	*tmp = (float)calcT(ut) / 10;
}
