#include "MPU9250.h"

bool MPU9250::Initialize(I2CHandler *i2chandler, int AccelScale, int GyroScale, int MagnetRate){
	if(initialized){
		if(I2cInitialize(i2chandler)){
			handler = i2chandler;
			if(I2cReadByte(handler,MPU9250_ADDRESS, MPU9250_WHO_AM_I) == MPU9250_WHO_AM_I_DEFAULT){
				if(mpu.Initialize(handler,AccelScale,GyroScale)){
					I2cWriteByte(handler, MPU9250_ADDRESS, MPU9250_INT_PIN_CFG, 0x02);				 //bypass mode(磁気センサが使用出来るようになる)
					if(ak.Initialize(handler,MagnetRate)){
						initialized = true;
					}
				}
			}
		}
	}
	return initialized;
}

bool MPU9250::Initialize(I2CHandler *handler)
{
	return Initialize(handler, 16, 2000, 100);
}

bool MPU9250::ReadAccGyr(float *ax, float *ay, float *az, float *gx, float *gy, float *gz){
	return mpu.ReadAccGyr(ax,ay,az,gx,gy,gz);
}

bool MPU9250::ReadAcc(float *ax, float *ay, float *az){
	return mpu.ReadAcc(ax,ay,az);
}
bool MPU9250::ReadGyr(float *gx, float *gy, float *gz){
	return mpu.ReadGyr(gx,gy,gz);
}

bool MPU9250::ReadMag(float *mx, float *my, float *mz){
	return ak.ReadMag(mx,my,mz);
}