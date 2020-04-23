/*

	2019 Oku Dan / oku_dan@yahoo.co.jp

	MPU9250用のI2C版ライブラリです.
	使用する際はデバイスのSDAとSCLをセンサに接続してください.

	1.MPU9250型クラスを作成
	2.Initialize()でセンサ初期化
	3.ReadAccGyr(),ReadMag()で読み出し

	内部DMPを利用してかなり正確なQuaternionを出力してくれる裏ワザ的手法がありますが、かなりの魔境なので追加しません.
	利用したい方は自分で調べてください.
*/
#include "I2CHandler.h"
#include "MPU6050.h"
#include "AK8963.h"

#define MPU9250_ADDRESS 0x68

#define MPU9250_WHO_AM_I 0x75
#define MPU9250_WHO_AM_I_DEFAULT 0x71
#define MPU9250_PWR_MGMT_1 0x6B
#define MPU9250_INT_PIN_CFG 0x37

#define MPU9250_ACCEL_CONFIG 0x1C
#define MPU9250_ACCEL_FS_SEL_2G 0x00
#define MPU9250_ACCEL_FS_SEL_4G 0x08
#define MPU9250_ACCEL_FS_SEL_8G 0x10
#define MPU9250_ACCEL_FS_SEL_16G 0x18

#define MPU9250_GYRO_CONFIG 0x1B
#define MPU9250_GYRO_FS_SEL_250DPS 0x00
#define MPU9250_GYRO_FS_SEL_500DPS 0x08
#define MPU9250_GYRO_FS_SEL_1000DPS 0x10
#define MPU9250_GYRO_FS_SEL_2000DPS 0x18

#define MPU9250_ACCEL_XOUT_H 0x3B
#define MPU9250_ACCEL_XOUT_L 0x3C
#define MPU9250_ACCEL_YOUT_H 0x3D
#define MPU9250_ACCEL_YOUT_L 0x3E
#define MPU9250_ACCEL_ZOUT_H 0x3F
#define MPU9250_ACCEL_ZOUT_L 0x40
#define MPU9250_TEMP_OUT_H 0x41
#define MPU9250_TEMP_OUT_L 0x42
#define MPU9250_GYRO_XOUT_H 0x43
#define MPU9250_GYRO_XOUT_L 0x44
#define MPU9250_GYRO_YOUT_H 0x45
#define MPU9250_GYRO_YOUT_L 0x46
#define MPU9250_GYRO_ZOUT_H 0x47
#define MPU9250_GYRO_ZOUT_L 0x48

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

class MPU9250
{
private:
	bool initialized = false;
	I2CHandler *handler;
	MPU6050 mpu;
	AK8963 ak;
public:
	bool Initialize(I2CHandler *i2chandler);
	bool Initialize(I2CHandler *i2chandler, int AccelScale, int GyroScale, int MagnetRate);
	bool ReadAccGyr(float *ax, float *ay, float *az, float *gx, float *gy, float *gz);
	bool ReadAcc(float *ax, float *ay, float *az);
	bool ReadGyr(float *gx, float *gy, float *gz);
	bool ReadMag(float *mx, float *my, float *mz);
};