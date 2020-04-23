/*

	2019 Oku Dan / oku_dan@yahoo.co.jp

	MPU6050用のI2C版ライブラリです.
	使用する際はデバイスのSDAとSCLをセンサに接続してください.

	1.MPU6050型クラスを作成
	2.Initialize()でセンサ初期化
	3.ReadAccGyr()で読み出し

	内部DMPを利用してかなり正確なQuaternionを出力してくれる裏ワザ的手法がありますが、かなりの魔境なので追加しません.
	利用したい方は自分で調べてください.
*/
#pragma once

#include "I2CHandler.h"

#define MPU6050_ADDRESS 0x68

#define MPU6050_WHO_AM_I 0x75
#define MPU6050_WHO_AM_I_DEFAULT 0x68
#define MPU6050_PWR_MGMT_1 0x6B
#define MPU6050_INT_PIN_CFG 0x37

#define MPU6050_ACCEL_CONFIG 0x1C
#define MPU6050_ACCEL_FS_SEL_2G 0x00
#define MPU6050_ACCEL_FS_SEL_4G 0x08
#define MPU6050_ACCEL_FS_SEL_8G 0x10
#define MPU6050_ACCEL_FS_SEL_16G 0x18

#define MPU6050_GYRO_CONFIG 0x1B
#define MPU6050_GYRO_FS_SEL_250DPS 0x00
#define MPU6050_GYRO_FS_SEL_500DPS 0x08
#define MPU6050_GYRO_FS_SEL_1000DPS 0x10
#define MPU6050_GYRO_FS_SEL_2000DPS 0x18

#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_ACCEL_XOUT_L 0x3C
#define MPU6050_ACCEL_YOUT_H 0x3D
#define MPU6050_ACCEL_YOUT_L 0x3E
#define MPU6050_ACCEL_ZOUT_H 0x3F
#define MPU6050_ACCEL_ZOUT_L 0x40
#define MPU6050_TEMP_OUT_H 0x41
#define MPU6050_TEMP_OUT_L 0x42
#define MPU6050_GYRO_XOUT_H 0x43
#define MPU6050_GYRO_XOUT_L 0x44
#define MPU6050_GYRO_YOUT_H 0x45
#define MPU6050_GYRO_YOUT_L 0x46
#define MPU6050_GYRO_ZOUT_H 0x47
#define MPU6050_GYRO_ZOUT_L 0x48


class MPU6050
{
private:
	bool initialized = false;
	I2CHandler *handler;
	float accRange;
	float gyroRange;

public:
	bool Initialize(I2CHandler *i2chandler);
	bool Initialize(I2CHandler *i2chandler,int AccelScale, int GyroScale);
	bool ReadAccGyr(float *ax, float *ay, float *az, float *gx, float *gy, float *gz);
	bool ReadAcc(float *ax, float *ay, float *az);
	bool ReadGyr(float *gx, float *gy, float *gz);
};