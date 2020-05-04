#include "MPU6050.h"

bool MPU6050::Initialize(I2CHandler *i2chandler, int AccelScale, int GyroScale){
	if(!initialized){
		if(I2cInitialize(i2chandler)){
			handler = i2chandler;
			//if(I2cReadByte(handler, MPU6050_ADDRESS, MPU6050_WHO_AM_I) == MPU6050_WHO_AM_I_DEFAULT){
			if(I2cReadByte(handler, MPU6050_ADDRESS, MPU6050_WHO_AM_I) != 0){
				uint8_t tempRegVal;

				switch (AccelScale)
				{
				case 2:
					tempRegVal = MPU6050_ACCEL_FS_SEL_2G;
					accRange = 2.0;												 //計算で使用するので，選択したレンジを入力する
					break;
				case 4:
					tempRegVal = MPU6050_ACCEL_FS_SEL_4G;
					accRange = 4.0;
					break;
				case 8:
					tempRegVal = MPU6050_ACCEL_FS_SEL_8G;
					accRange = 8.0;
					break;	
				default:
					tempRegVal = MPU6050_ACCEL_FS_SEL_16G;
					accRange = 16.0;
					break;
				}
				I2cWriteByte(handler, MPU6050_ADDRESS, MPU6050_ACCEL_CONFIG, tempRegVal);		 //加速度センサの測定レンジの設定

				switch (GyroScale)
				{
				case 250:
					tempRegVal = MPU6050_GYRO_FS_SEL_250DPS;
					gyroRange = 250.0;												 //計算で使用するので，選択したレンジを入力する
					break;
				case 500:
					tempRegVal = MPU6050_GYRO_FS_SEL_500DPS;
					gyroRange = 500.0;
					break;
				case 1000:
					tempRegVal = MPU6050_GYRO_FS_SEL_1000DPS;
					gyroRange = 1000.0;
					break;
				default:
					tempRegVal = MPU6050_GYRO_FS_SEL_2000DPS;
					gyroRange = 2000.0;
					break;
				}
				I2cWriteByte(handler, MPU6050_ADDRESS, MPU6050_GYRO_CONFIG, tempRegVal); //ジャイロセンサの測定レンジの設定

				accRange /= 32768.0;	//計算用の係数に変換
				gyroRange /= 32768.0;

				I2cWriteByte(handler, MPU6050_ADDRESS, MPU6050_PWR_MGMT_1, 0);				 //スリープモードを解除
				initialized = true;
			}
		}
	}
	return initialized;
}

bool MPU6050::Initialize(I2CHandler *i2chandler)
{
	return Initialize(i2chandler,16, 2000);
}

bool MPU6050::ReadAccGyr(float *ax, float *ay, float *az, float *gx, float *gy, float *gz){
	if(initialized){
		uint8_t AccGyroTemp[14];
		I2cReadBytes(handler, MPU6050_ADDRESS, MPU6050_ACCEL_XOUT_H, AccGyroTemp, 14);
		*ax = (int16_t)(AccGyroTemp[0] << 8 | AccGyroTemp[1]) * accRange;
		*ay = (int16_t)(AccGyroTemp[2] << 8 | AccGyroTemp[3]) * accRange;
		*az = (int16_t)(AccGyroTemp[4] << 8 | AccGyroTemp[5]) * accRange;

		*gx = (int16_t)(AccGyroTemp[8] << 8 | AccGyroTemp[9]) * gyroRange;
		*gy = (int16_t)(AccGyroTemp[10] << 8 | AccGyroTemp[11]) * gyroRange;
		*gz = (int16_t)(AccGyroTemp[12] << 8 | AccGyroTemp[13]) * gyroRange;
		return true;
	}
	return false;
}

bool MPU6050::ReadAcc(float *ax, float *ay, float *az){
	if(initialized){
		uint8_t AccTemp[6];
		I2cReadBytes(handler, MPU6050_ADDRESS, MPU6050_ACCEL_XOUT_H, AccTemp, 6);
		*ax = (int16_t)(AccTemp[0] << 8 | AccTemp[1]) * accRange;
		*ay = (int16_t)(AccTemp[2] << 8 | AccTemp[3]) * accRange;
		*az = (int16_t)(AccTemp[4] << 8 | AccTemp[5]) * accRange;
		return true;
	}
	return false;
}

bool MPU6050::ReadGyr(float *gx, float *gy, float *gz){
	if(initialized){
		uint8_t GyroTemp[6];
		I2cReadBytes(handler, MPU6050_ADDRESS, MPU6050_GYRO_XOUT_H, GyroTemp, 6);
		*gx = (int16_t)(GyroTemp[0] << 8 | GyroTemp[1]) * gyroRange;
		*gy = (int16_t)(GyroTemp[2] << 8 | GyroTemp[3]) * gyroRange;
		*gz = (int16_t)(GyroTemp[4] << 8 | GyroTemp[5]) * gyroRange;
		return true;
	}
	return false;
}