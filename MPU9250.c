#include "I2CHandler.h"
#include "SystemHandler.h"
#include "MPU6050.h"
#include "AK8963.h"
#include "MPU9250.h"

static const char InitlzErrMsg[] = "MPU9250 failed to initialize\n";
static const char InitErrMsg[] = "This MPU9250 has not been initialized\n";

bool InitializeMPU9250(MPU9250 *mpu, I2CHandle *i2chandle){
	if(mpu->initialized){
		if(I2cInitialize(i2chandle)){
			mpu->i2chandle = i2chandle;
			uint8_t tempRegVal;
			tempRegVal = I2cReadByte(mpu->i2chandle, MPU9250_ADDRESS, MPU9250_WHO_AM_I);
			if(tempRegVal == MPU9250_WHO_AM_I_DEFAULT){
				mpu6050->AccelScale = mpu->AccelScale;
				mpu6050->GyroScale = mpu->GyroScale;
				if(InitializeMPU6050(&mpu6050,mpu->i2chandle) == false){
					DebugMessage(InitlzErrMsg);
					return false;
				}
			}

			I2cWriteByte(mpu->i2chandle, MPU9250_ADDRESS, MPU9250_INT_PIN_CFG, 0x02);				 //bypass mode(磁気センサが使用出来るようになる)
			for(unsigned int i = 0;i < 0xFFFF;i++);		//待機
			tempRegVal = I2cReadByte(mpu->i2chandle, AK8963_ADDRESS, AK8963_WIM);
			if(tempRegVal == AK8963_WHI_DEFAULT){
				ak8963->MagnetScale = mpu->MagnetScale;
				if(InitializeAK8963(&ak8963,mpu->i2chandle) == false){
					DebugMessage(InitlzErrMsg);
					return false;
				}
			}
		}
	}else{
		DebugMessage("MPU9250 was initialized twice\n");
	}
}

void ReadAccGyrMPU9250(MPU9250 *mpu, float *ax, float *ay, float *az, float *gx, float *gy, float *gz){
	ReadAccGyrMPU6050(&mpu->mpu6050, float *ax, float *ay, float *az, float *gx, float *gy, float *gz);
}

void ReadAccMPU9250(MPU9250 *mpu, float *ax, float *ay, float *az){
	ReadAccMPU6050(&mpu->mpu6050, float *ax, float *ay, float *az);
}

void ReadGyrMPU9250(MPU9250 *mpu, float *gx, float *gy, float *gz){
	ReadGyrMPU6050(&mpu->mpu6050, float *gx, float *gy, float *gz);
}

bool ReadMagMPU9250(MPU9250 *mpu, float *mx, float *my, float *mz){
	return ReadMagAK8963(&mpu->ak8963, float *mx, float *my, float *mz);
}