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