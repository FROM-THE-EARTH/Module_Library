#include "I2CHandler.h"
#include "SystemHandler.h"
#include "MPU6050.h"


static const char InitErrMsg[] = "This MPU6050 has not been initialized\n";

bool InitializeMPU6050(MPU6050 *mpu, I2CHandle *i2chandle){
	if(mpu->initialized){
		if(I2cInitialize(i2chandle)){
			mpu->i2chandle = i2chandle;
			uint8_t tempRegVal = 0x00;
			tempRegVal = I2cReadByte(mpu->i2chandle, MPU6050_ADDRESS, MPU6050_WHO_AM_I);
			//if(tempRegVal == MPU6050_WHO_AM_I_DEFAULT){
			if(tempRegVal != 0x00){		//MPU9250で使いまわすようにこんな実装にした許して
				I2cWriteByte(mpu->i2chandle, MPU6050_ADDRESS, MPU6050_PWR_MGMT_1, 0);				 //スリープモードを解除
				switch (mpu->AccelScale)
				{
				case 2:
					tempRegVal = MPU6050_ACCEL_FS_SEL_2G;
					mpu->accRange = 2.0;												 //計算で使用するので，選択したレンジを入力する
					break;
				case 4:
					tempRegVal = MPU6050_ACCEL_FS_SEL_4G;
					mpu->accRange = 4.0;
					break;
				case 8:
					tempRegVal = MPU6050_ACCEL_FS_SEL_8G;
					mpu->accRange = 8.0;
					break;	
				default:
					tempRegVal = MPU6050_ACCEL_FS_SEL_16G;
					mpu->accRange = 16.0;
					break;
				}
				I2cWriteByte(mpu->i2chandle, MPU6050_ADDRESS, MPU6050_ACCEL_CONFIG, tempRegVal);		 //加速度センサの測定レンジの設定

				switch (mpu->GyroScale)
				{
				case 250:
					tempRegVal = MPU6050_GYRO_FS_SEL_250DPS;
					mpu->gyroRange = 250.0;												 //計算で使用するので，選択したレンジを入力する
					break;
				case 500:
					tempRegVal = MPU6050_GYRO_FS_SEL_500DPS;
					mpu->gyroRange = 500.0;
					break;
				case 1000:
					tempRegVal = MPU6050_GYRO_FS_SEL_1000DPS;
					mpu->gyroRange = 1000.0;
					break;
				default:
					tempRegVal = MPU6050_GYRO_FS_SEL_2000DPS;
					mpu->gyroRange = 2000.0;
					break;
				}
				I2cWriteByte(mpu->i2chandle, MPU6050_ADDRESS, MPU6050_GYRO_CONFIG, tempRegVal); //ジャイロセンサの測定レンジの設定

				mpu->accRange /= 32768.0;	//計算用の係数に変換
				mpu->gyroRange /= 32768.0;
				mpu->initialized = true;
			}else{
				DebugMessage("MPU6050 WHO_AM_I test is not correct\n");
			}
		}
	}else{
		DebugMessage("MPU6050 was initialized twice\n");
	}
	return mpu->initialized;
}

void ReadAccGyrMPU6050(MPU6050 *mpu, float *ax, float *ay, float *az, float *gx, float *gy, float *gz){
	if(mpu->initialized){
		uint8_t AccGyroTemp[14];
		I2cReadBytes(mpu->i2chandle, MPU6050_ADDRESS, MPU6050_ACCEL_XOUT_H, AccGyroTemp, 14);
		*ax = (int16_t)(AccGyroTemp[0] << 8 | AccGyroTemp[1]) * mpu->accRange;
		*ay = (int16_t)(AccGyroTemp[2] << 8 | AccGyroTemp[3]) * mpu->accRange;
		*az = (int16_t)(AccGyroTemp[4] << 8 | AccGyroTemp[5]) * mpu->accRange;

		*gx = (int16_t)(AccGyroTemp[8] << 8 | AccGyroTemp[9]) * mpu->gyroRange;
		*gy = (int16_t)(AccGyroTemp[10] << 8 | AccGyroTemp[11]) * mpu->gyroRange;
		*gz = (int16_t)(AccGyroTemp[12] << 8 | AccGyroTemp[13]) * mpu->gyroRange;
	}else{
		DebugMessage(InitErrMsg);
	}
}

void ReadAccMPU6050(float *ax, float *ay, float *az){
	if(mpu->initialized){
		uint8_t AccTemp[6];
		I2cReadBytes(mpu->i2chandle, MPU6050_ADDRESS, MPU6050_ACCEL_XOUT_H, AccTemp, 6);
		*ax = (int16_t)(AccTemp[0] << 8 | AccTemp[1]) * mpu->accRange;
		*ay = (int16_t)(AccTemp[2] << 8 | AccTemp[3]) * mpu->accRange;
		*az = (int16_t)(AccTemp[4] << 8 | AccTemp[5]) * mpu->accRange;
	}else{
		DebugMessage(InitErrMsg);
	}
}

void ReadGyrMPU6050(float *gx, float *gy, float *gz){
	if(mpu->initialized){
		uint8_t GyroTemp[6];
		I2cReadBytes(mpu->i2chandle, MPU6050_ADDRESS, MPU6050_GYRO_XOUT_H, GyroTemp, 6);
		*gx = (int16_t)(GyroTemp[0] << 8 | GyroTemp[1]) * mpu->gyroRange;
		*gy = (int16_t)(GyroTemp[2] << 8 | GyroTemp[3]) * mpu->gyroRange;
		*gz = (int16_t)(GyroTemp[4] << 8 | GyroTemp[5]) * mpu->gyroRange;
	}else{
		DebugMessage(InitErrMsg);
	}
}