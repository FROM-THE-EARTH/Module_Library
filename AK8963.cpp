#include "AK8963.h"

bool AK8963::Initialize(I2CHandler *i2chandler,int MagnetRate){
    if(!initialized){
		if(I2cInitialize(i2chandler)){
			handler = i2chandler;
			if(I2cReadByte(handler, AK8963_ADDRESS, AK8963_WIM) == AK8963_WHI_DEFAULT){
				uint8_t tempRegVal;
                switch (MagnetRate)
                {
                case 8:
                    tempRegVal = AK8963_CNTL1_MODE_SEL_8HZ;
                    break;
                default:
                    tempRegVal = AK8963_CNTL1_MODE_SEL_100HZ;
                    break;
                }
                I2cWriteByte(handler, AK8963_ADDRESS, AK8963_CNTL1, tempRegVal);
                uint8_t asax = I2cReadByte(handler, AK8963_ADDRESS, AK8963_ASAX);
                uint8_t asay = I2cReadByte(handler, AK8963_ADDRESS, AK8963_ASAY);
                uint8_t asaz = I2cReadByte(handler, AK8963_ADDRESS, AK8963_ASAZ);

                mAdjx = ((float)asax - 128.0f) * 0.5f / 128.0f + 1.0f;
                mAdjy = ((float)asay - 128.0f) * 0.5f / 128.0f + 1.0f;
                mAdjz = ((float)asaz - 128.0f) * 0.5f / 128.0f + 1.0f;

                mAdjx *= 4921.0f / 32768.0f;	//計算用の係数に変換
                mAdjy *= 4921.0f / 32768.0f;	//計算用の係数に変換
                mAdjz *= 4921.0f / 32768.0f;	//計算用の係数に変換
				initialized = true;
			}
		}
	}
	return initialized;
}

bool AK8963::ReadMag(float *mx, float *my, float *mz){
    if(initialized){
        uint8_t ST1Bit;
        ST1Bit = I2cReadByte(handler,AK8963_ADDRESS, AK8963_ST1);
        if ((ST1Bit & 0x01)){
            uint8_t magneticData[7];
            I2cReadBytes(handler,AK8963_ADDRESS, AK8963_HXL, magneticData, 7);
            *mx = ((int16_t)((magneticData[3] << 8) | magneticData[2])) * mAdjy;
            *my = ((int16_t)((magneticData[1] << 8) | magneticData[0])) * mAdjx;
            *mz = -((int16_t)((magneticData[5] << 8) | magneticData[4])) * mAdjz;
            return true;
        }
    }
    return false;
}