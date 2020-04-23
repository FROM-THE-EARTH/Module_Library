/*

	2019 Oku Dan / oku_dan@yahoo.co.jp

	Adafruit社製LSM9DS1用のI2C版ライブラリです.
	使用する際はデバイスのSDAとSCLをセンサに接続してください.

	1.LSM_9DS1型クラスを作成
	2.Initialize()でセンサの初期化
	3.availableFIFO()でFIFOバッファにあるデータ数を確認
	4.その数だけReadAccFIFO(&x, &y, &z)、ReadGyrFIFO(&x, &y, &z)で加速度ジャイロ読み出し
	5.ReadMag(&x, &y, &z)で磁力センサ読み出し
	6.3に戻る

	ACC_SAMPLING_RATE、GYR_SAMPLING_RAT、MAG_SAMPLING_RATEでサンプリングレートを変更できる（デフォルトは238Hz,238Hz,80Hz）
	データシートに書いてないけどCTRL_REG1_Mの375行目をいじるとサブモードで磁力センサレートを1000Hzまであげられる
	FIFOバッファ使わず読みだしても良い.その場合availableFIFO()を読まずにReadAcc(),ReadGyr().
	内部フィルタでハイパス・ローパスフィルタをかけられるらしいがFiltersInit()が未完成.コメントアウトしてあるので誰か作って.

*/

#include "I2CHandler.h"

#define LSM9DS1_M 0x1E
#define LSM9DS1_AG 0x6B

#define LSM9DS1_ACT_THS 0x04
#define LSM9DS1_ACT_DUR 0x05
#define LSM9DS1_INT_GEN_CFG_XL 0x06
#define LSM9DS1_INT_GEN_THS_X_XL 0x07
#define LSM9DS1_INT_GEN_THS_Y_XL 0x08
#define LSM9DS1_INT_GEN_THS_Z_XL 0x09
#define LSM9DS1_INT_GEN_DUR_XL 0x0A
#define LSM9DS1_REFERENCE_G 0x0B
#define LSM9DS1_INT1_CTRL 0x0C
#define LSM9DS1_INT2_CTRL 0x0D
#define LSM9DS1_WHO_AM_I_XG 0x0F
#define LSM9DS1_CTRL_REG1_G 0x10
#define LSM9DS1_CTRL_REG2_G 0x11
#define LSM9DS1_CTRL_REG3_G 0x12
#define LSM9DS1_ORIENT_CFG_G 0x13
#define LSM9DS1_INT_GEN_SRC_G 0x14
#define LSM9DS1_OUT_TEMP_L 0x15
#define LSM9DS1_OUT_TEMP_H 0x16
#define LSM9DS1_STATUS_REG_0 0x17
#define LSM9DS1_OUT_X_L_G 0x18
#define LSM9DS1_OUT_X_H_G 0x19
#define LSM9DS1_OUT_Y_L_G 0x1A
#define LSM9DS1_OUT_Y_H_G 0x1B
#define LSM9DS1_OUT_Z_L_G 0x1C
#define LSM9DS1_OUT_Z_H_G 0x1D
#define LSM9DS1_CTRL_REG4 0x1E
#define LSM9DS1_CTRL_REG5_XL 0x1F
#define LSM9DS1_CTRL_REG6_XL 0x20
#define LSM9DS1_CTRL_REG7_XL 0x21
#define LSM9DS1_CTRL_REG8 0x22
#define LSM9DS1_CTRL_REG9 0x23
#define LSM9DS1_CTRL_REG10 0x24
#define LSM9DS1_INT_GEN_SRC_XL 0x26
#define LSM9DS1_STATUS_REG_1 0x27
#define LSM9DS1_OUT_X_L_XL 0x28
#define LSM9DS1_OUT_X_H_XL 0x29
#define LSM9DS1_OUT_Y_L_XL 0x2A
#define LSM9DS1_OUT_Y_H_XL 0x2B
#define LSM9DS1_OUT_Z_L_XL 0x2C
#define LSM9DS1_OUT_Z_H_XL 0x2D
#define LSM9DS1_FIFO_CTRL 0x2E
#define LSM9DS1_FIFO_SRC 0x2F
#define LSM9DS1_INT_GEN_CFG_G 0x30
#define LSM9DS1_INT_GEN_THS_XH_G 0x31
#define LSM9DS1_INT_GEN_THS_XL_G 0x32
#define LSM9DS1_INT_GEN_THS_YH_G 0x33
#define LSM9DS1_INT_GEN_THS_YL_G 0x34
#define LSM9DS1_INT_GEN_THS_ZH_G 0x35
#define LSM9DS1_INT_GEN_THS_ZL_G 0x36
#define LSM9DS1_INT_GEN_DUR_G 0x37

///////////////////////////////
// LSM9DS1 Magneto Registers //
///////////////////////////////
#define LSM9DS1_OFFSET_X_REG_L_M 0x05
#define LSM9DS1_OFFSET_X_REG_H_M 0x06
#define LSM9DS1_OFFSET_Y_REG_L_M 0x07
#define LSM9DS1_OFFSET_Y_REG_H_M 0x08
#define LSM9DS1_OFFSET_Z_REG_L_M 0x09
#define LSM9DS1_OFFSET_Z_REG_H_M 0x0A
#define LSM9DS1_WHO_AM_I_M 0x0F
#define LSM9DS1_CTRL_REG1_M 0x20
#define LSM9DS1_CTRL_REG2_M 0x21
#define LSM9DS1_CTRL_REG3_M 0x22
#define LSM9DS1_CTRL_REG4_M 0x23
#define LSM9DS1_CTRL_REG5_M 0x24
#define LSM9DS1_STATUS_REG_M 0x27
#define LSM9DS1_OUT_X_L_M 0x28
#define LSM9DS1_OUT_X_H_M 0x29
#define LSM9DS1_OUT_Y_L_M 0x2A
#define LSM9DS1_OUT_Y_H_M 0x2B
#define LSM9DS1_OUT_Z_L_M 0x2C
#define LSM9DS1_OUT_Z_H_M 0x2D
#define LSM9DS1_INT_CFG_M 0x30
#define LSM9DS1_INT_SRC_M 0x31
#define LSM9DS1_INT_THS_L_M 0x32
#define LSM9DS1_INT_THS_H_M 0x33

////////////////////////////////
// LSM9DS1 WHO_AM_I Responses //
////////////////////////////////
#define LSM9DS1_WHO_AM_I_AG_RSP 0x68
#define LSM9DS1_WHO_AM_I_M_RSP 0x3D


#define LSM9DS1_ACC_SAMPLING_RATE 0b100 //011 : 119Hz / 100 : 238Hz / 101 : 476Hz / 110 : 952Hz
#define LSM9DS1_GYR_SAMPLING_RATE 0b100 //011 : 119Hz / 100 : 238Hz / 101 : 476Hz / 110 : 952Hz
// To use FIFO, AccODR and GyrODR must be same.
#define LSM9DS1_MAG_SAMPLING_RATE 0b111 //011 : 5Hz / 100 : 10Hz / 101 : 20Hz / 110 : 40Hz / 111 : 80Hz

#define LSM9DS1_SENSITIVITY_ACCELEROMETER_2 0.000061
#define LSM9DS1_SENSITIVITY_ACCELEROMETER_4 0.000122
#define LSM9DS1_SENSITIVITY_ACCELEROMETER_8 0.000244
#define LSM9DS1_SENSITIVITY_ACCELEROMETER_16 0.000732
#define LSM9DS1_SENSITIVITY_GYROSCOPE_245 0.00875
#define LSM9DS1_SENSITIVITY_GYROSCOPE_500 0.0175
#define LSM9DS1_SENSITIVITY_GYROSCOPE_2000 0.07
#define LSM9DS1_SENSITIVITY_MAGNETOMETER_4 0.00014
#define LSM9DS1_SENSITIVITY_MAGNETOMETER_8 0.00029
#define LSM9DS1_SENSITIVITY_MAGNETOMETER_12 0.00043
#define LSM9DS1_SENSITIVITY_MAGNETOMETER_16 0.00058

class LSM_9DS1
{
private:
	bool initialized = false;
	I2CHandler *handler;
	void FIFOInit();
	void AccInit(int scale);
	void GyrInit(int scale);
	void MagInit(int scale);
	void FiltersInit();
	float AccelSensitivity;
	float GyroSensitivity;
	float MagnetSensitivity;

public:
	bool Initialize(I2CHandler *handler);
	bool Initialize(I2CHandler *handler, int AccelScale, int GyroScale, int MagnetScale);
	bool ReadAcc(float *x, float *y, float *z);
	bool ReadGyr(float *x, float *y, float *z);
	bool ReadAccFIFO(float *x, float *y, float *z);
	bool ReadGyrFIFO(float *x, float *y, float *z);
	bool ReadMag(float *x, float *y, float *z);
	int availableFIFO();
	bool FIFOenabled = false;
	bool Filterenabled = false;
};