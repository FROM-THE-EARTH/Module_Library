#pragma once

#include "I2CHandler.h"

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

class AK8963
{
    private:
        bool initialized = false;
        I2CHandler *handler;
    	float mAdjx;
	    float mAdjy;
	    float mAdjz;
    public:
    	bool Initialize(I2CHandler *i2chandler,int MagnetRate);
        bool ReadMag(float *mx, float *my, float *mz);
};


bool AK8963::Initialize(I2CHandler *i2chandler,int MagnetRate){
    if(initialized){
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
                mAdjz *= 4921.0f / 32768.0f;	//計算用の係数に変換			 //スリープモードを解除
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