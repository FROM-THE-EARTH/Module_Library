
#include "I2CHandler.h"
#include "SystemHandler.h"

#define AK8963_ADDRESS 0x0C
#define AK8963_WIM 0x00
#define AK8963_WHI_DEFAULT 0x48
#define AK8963_CNTL1 0x0A
#define AK8963_CNTL1_MODE_SEL_8HZ 0x12
#define AK8963_CNTL1_MODE_SEL_100HZ 0x16
#define AK8963_ST1 0x02
#define AK8963_ASAX 0x10
#define AK8963_ASAY 0x11
#define AK8963_ASAZ 0x12

#define AK8963_HXL 0x03
#define AK8963_HXH 0x04
#define AK8963_HYL 0x05
#define AK8963_HYH 0x06
#define AK8963_HZL 0x07
#define AK8963_HZH 0x08

static const char InitErrMsg[] = "This AK8963 has not been initialized\n";

typedef struct
{
	bool initialized = false;
	I2CHandle i2chandle;
	int MagnetRate = 100;
	float mAdjx;
	float mAdjy;
	float mAdjz;
} AK8963;

bool InitializeAK8963(AK8963 *ak, I2CHandle *i2chandle){
	if(ak->initialized){
		if(I2cInitialize(i2chandle)){
			ak->i2chandle = i2chandle;
			uint8_t tempRegVal;
			tempRegVal = I2cReadByte(ak->i2chandle, AK8963_ADDRESS, AK8963_WIM);
			if(tempRegVal == AK8963_WHI_DEFAULT){
				switch (ak->MagnetRate)
				{
				case 8:
					tempRegVal = AK8963_CNTL1_MODE_SEL_8HZ;
					break;
				default:
					tempRegVal = AK8963_CNTL1_MODE_SEL_100HZ;
					break;
				}

				I2cWriteByte(ak->i2chandle, AK8963_ADDRESS, AK8963_CNTL1, tempRegVal);
				uint8_t asax = I2cReadByte(ak->i2chandle, AK8963_ADDRESS, AK8963_ASAX);
				uint8_t asay = I2cReadByte(ak->i2chandle, AK8963_ADDRESS, AK8963_ASAY);
				uint8_t asaz = I2cReadByte(ak->i2chandle, AK8963_ADDRESS, AK8963_ASAZ);

				ak->mAdjx = ((float)asax - 128.0f) * 0.5f / 128.0f + 1.0f;
				ak->mAdjy = ((float)asay - 128.0f) * 0.5f / 128.0f + 1.0f;
				ak->mAdjz = ((float)asaz - 128.0f) * 0.5f / 128.0f + 1.0f;

				ak->mAdjx *= 4921.0f / 32768.0f;	//計算用の係数に変換
				ak->mAdjy *= 4921.0f / 32768.0f;	//計算用の係数に変換
				ak->mAdjz *= 4921.0f / 32768.0f;	//計算用の係数に変換
			}else{
				DebugMessage("AK8963 WHO_AM_I test is not correct\n");
            }
        }
    }else{
		DebugMessage("AK8963 was initialized twice\n");
    }
    return ak->initialized;
}

bool ReadMagAK8963(AK8963 *ak,float *mx, float *my, float *mz){
	if(ak->initialized){
        uint8_t ST1Bit;
	    ST1Bit = I2cReadByte(ak->i2chandle, AK8963_ADDRESS, AK8963_ST1);
	    if ((ST1Bit & 0x01)){
		    uint8_t magneticData[7];
		    I2cReadBytes(ak->i2chandle, AK8963_ADDRESS, AK8963_HXL, magneticData, 7);
		    *mx = ((int16_t)((magneticData[3] << 8) | magneticData[2])) * ak->mAdjy;
		    *my = ((int16_t)((magneticData[1] << 8) | magneticData[0])) * ak->mAdjx;
		    *mz = -((int16_t)((magneticData[5] << 8) | magneticData[4])) * ak->mAdjz;
		return true;
	}else{
        DebugMessage(InitErrMsg);
    }
    return false;
}