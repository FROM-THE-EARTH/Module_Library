#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#include "stdint.h"
#include "stdbool.h"

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

bool MPU6050_Initialize(int AccelScale, int GyroScale);

void MPU6050_ReadAccGyr(float *ax, float *ay, float *az, float *gx, float *gy, float *gz);

void MPU6050_ReadAcc(float *ax, float *ay, float *az);

void MPU6050_ReadGyr(float *gx, float *gy, float *gz);

#endif