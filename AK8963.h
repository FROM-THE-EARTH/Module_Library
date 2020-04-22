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

typedef struct
{
	bool initialized = false;
	I2CHandle i2chandle;
	int MagnetRate = 100;
	float mAdjx;
	float mAdjy;
	float mAdjz;
} AK8963;

bool InitializeAK8963(AK8963* ak, I2CHandle* i2chandle);
bool ReadMagAK8963(AK8963 *ak,float *mx, float *my, float *mz);