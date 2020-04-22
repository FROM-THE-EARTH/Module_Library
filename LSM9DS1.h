/*

	2019 Oku Dan / oku_dan@yahoo.co.jp

	Adafruit社製LSM9DS1用のI2C版ライブラリです.
	使用する際はデバイスのSDAとSCLをセンサに接続してください.

	1.LSM_9DS1型クラスを作成
	2.Initialize(16,2000,16)でセンサの出力スケールつき初期化
	3.availableFIFO()でFIFOバッファにあるデータ数を確認
	4.その数だけReadAccFIFO(&x, &y, &z)、ReadGyrFIFO(&x, &y, &z)で加速度ジャイロ読み出し
	5.ReadMag(&x, &y, &z)で磁力センサ読み出し
	6.3に戻る

	110~113行目ACC_SAMPLING_RATE、GYR_SAMPLING_RAT、MAG_SAMPLING_RATEでサンプリングレートを変更できる（デフォルトは238Hz,238Hz,80Hz）
	データシートに書いてないけどCTRL_REG1_Mの375行目をいじるとサブモードで磁力センサレートを1000Hzまであげられる
	FIFOバッファ使わず読みだしても良い.その場合Initialize()内のFIFOInit()をコメントアウトしてavailableFIFO()を読まずにReadAcc(),ReadGyr().
	内部フィルタでハイパス・ローパスフィルタをかけられるらしいがFiltersInit()が未完成.コメントアウトしてあるので誰か作って.

*/

#include "I2CHandler.h"
#include "SystemHandler.h"

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

const char InitErrMsg[] = "This LSM9DS1 has not been initialized\n";
const char FifoErrMsg[] = "FIFO has not been enabled in this LSM9DS1\n";

struct LSM9DS1
{
	bool initialized = false;
	I2CHandle *i2chandle;
	int AccelScale = 16;
	int GyroScale = 2000;
	int MagnetScale = 16;
	float AccelSensitivity;
	float GyroSensitivity;
	float MagnetSensitivity;
	bool FilterEnable = false;
	bool FifoEnable = false;
};

static void FifoInit(LSM9DS1 *lsm){
	uint8_t tempRegValue = 0;

	// CTRL_REG9 (0x23) (Default value: 0x00)
	//	[0][SLEEP_G][0][FIFO_TEMP_END][RDY_mask_bit][I2C_DISABLE][FIFO_EN][STOP_ON_FTH]
	//	SLEEP_G - Gyroscope sleep mode enable. Default value: 0	(0: disabled; 1: enabled)
	//	FIFO_TEMP_EN - Temperature data storage in FIFO enable. Default value: 0
	//		(0: temperature data not stored in FIFO; 1: temperature data stored in FIFO)
	//	DRDY_mask_bit -  Data available enable bit. Default value: 0 (0: DA timer disabled; 1: DA timer enabled)
	//	I2C_DISABLE - Disable I2C interface. Default value: 0
	// 		(0: both I2C and SPI enabled; 1: I2C disabled, SPI only)
	// FIFO_EN FIFO - memory enable. Default value: 0 (0: disabled; 1: enabled)
	// STOP_ON_FTH - Enable FIFO threshold level use. Default value: 0
	// 		(0: FIFO depth is not limited; 1: FIFO depth is limited to threshold level)
	tempRegValue |= (0b10 & 0x3);
	I2cWriteByte(lsm->i2chandle, LSM9DS1_AG, LSM9DS1_CTRL_REG9, tempRegValue);

	//	FIFO_CTRL (0x2E) (Default value: 0x00)
	//	[FMODE2][FMODE1][FMODE0][FTH4][FTH3][FTH2][FTH1][FTH0]
	//	FMODE [2:0] FIFO mode selection bits. Default value: 000
	// 		000 Bypass mode. FIFO turned off
	// 		001 FIFO mode. Stops collecting data when FIFO is full.
	// 		010 Reserved
	//		011 Continuous mode until trigger is deasserted, then FIFO mode.
	// 		100 Bypass mode until trigger is deasserted, then Continuous mode.
	// 		110 Continuous mode. If the FIFO is full, the new sample overwrites the older sample.
	//	FTH [4:0] FIFO threshold level setting. Default value: 00000
	tempRegValue = 0;
	tempRegValue |= (0b110 & 0x07) << 5;
	I2cWriteByte(lsm->i2chandle, LSM9DS1_AG, LSM9DS1_FIFO_CTRL, tempRegValue);
}

static void FiltersInit(LSM9DS1 *lsm)
{
	uint8_t tempRegValue = 0;

	// REFERENCE_G (0x0B)
	// [REF7_G][REF6_G][REF5_G][REF4_G][REF3_G][REF2_G][REF1_G][REF0_G]
	//	REF_G [7:0] Reference value for gyroscope’s digital high-pass filter (r/w).(Default value : 0000 0000)
	I2cWriteByte(lsm->i2chandle, LSM9DS1_AG, LSM9DS1_REFERENCE_G, 0x00);

	// CTRL_REG3_G (Default value: 0x00)
	// [LP_mode][HP_EN][0][0][HPCF3_G][HPCF2_G][HPCF1_G][HPCF0_G]
	// LP_mode - Low-power mode enable (0: disabled, 1: enabled)
	// HP_EN - HPF enable (0:disabled, 1: enabled)
	// HPCF_G[3:0] - HPF cutoff frequency (0000 ~ 1001 depends on ODR)
	tempRegValue |= (0b01 & 0x03) << 6;
	tempRegValue |= (0b0000 & 0x11);
	I2cWriteByte(lsm->i2chandle, LSM9DS1_AG, LSM9DS1_CTRL_REG3_G, tempRegValue);

	// CTRL_REG6_XL (0x20) (Default value: 0x00)
	// [ODR_XL2][ODR_XL1][ODR_XL0][FS1_XL][FS0_XL][BW_SCAL_ODR][BW_XL1][BW_XL0]
	// ODR_XL[2:0] - Output data rate & power mode selection
	// FS_XL[1:0] - Full-scale selection
	// BW_SCAL_ODR - Bandwidth selection
	//	(0: bandwidth determined by ODR selection:
	//	 -BW = 408 Hz when ODR = 952 Hz, 50 Hz, 10 Hz;
	//	 -BW = 211 Hz when ODR = 476 Hz;
	//	 -BW = 105 Hz when ODR = 238 Hz;
	//	 -BW = 50 Hz when ODR = 119 Hz;)
	//	1: bandwidth selected according to BW_XL [2:1] selection)
	// BW_XL[1:0] - Anti-aliasing filter bandwidth selection (00: 408 Hz; 01: 211 Hz; 10: 105 Hz; 11: 50 Hz)
	// To disable the accel, set the sampleRate bits to 0.
	tempRegValue = I2cReadByte(lsm->i2chandle, LSM9DS1_AG, LSM9DS1_CTRL_REG6_XL);
	tempRegValue &= 0b11111000;
	tempRegValue |= (0b000 & 0x07);
	I2cWriteByte(lsm->i2chandle, LSM9DS1_AG, LSM9DS1_CTRL_REG6_XL, tempRegValue);

	// CTRL_REG7_XL (0x21) (Default value: 0x00)
	// [HR][DCF1][DCF0][0][0][FDS][0][HPIS1]
	// HR - High resolution mode (0: disable, 1: enable)
	// DCF[1:0] - Digital filter cutoff frequency
	//	1 00 : ODR / 50
	//	1 01 : ODR / 100
	//	1 10 : ODR / 9
	//	1 11 : ODR / 400
	// FDS - Filtered data selection
	//	(0: internal filter bypassed; 1: data from internal filter sent to output register and FIFO)
	// HPIS1 - HPF enabled for interrupt function
	tempRegValue = 0;
	tempRegValue |= 0x01 << 7;
	tempRegValue |= (0b00 & 0x3) << 5;
	tempRegValue |= 0b100;
	I2cWriteByte(lsm->i2chandle, LSM9DS1_AG, LSM9DS1_CTRL_REG7_XL, tempRegValue);
}

static void AccInit(LSM9DS1 *lsm)
{
	uint8_t tempRegValue;

	//	CTRL_REG5_XL (0x1F) (Default value: 0x38)
	//	[DEC_1][DEC_0][Zen_XL][Yen_XL][Zen_XL][0][0][0]
	//	DEC[0:1] - Decimation of accel data on OUT REG and FIFO.
	//		00: None, 01: 2 samples, 10: 4 samples 11: 8 samples
	//	Zen_XL - Z-axis output enabled
	//	Yen_XL - Y-axis output enabled
	//	Xen_XL - X-axis output enabled
	tempRegValue = 0;
	I2cWriteByte(lsm->i2chandle, LSM9DS1_AG, LSM9DS1_CTRL_REG5_XL, 0b111000);

	// CTRL_REG6_XL (0x20) (Default value: 0x00)
	// [ODR_XL2][ODR_XL1][ODR_XL0][FS1_XL][FS0_XL][BW_SCAL_ODR][BW_XL1][BW_XL0]
	// ODR_XL[2:0] - Output data rate & power mode selection
	// FS_XL[1:0] - Full-scale selection
	// BW_SCAL_ODR - Bandwidth selection
	// BW_XL[1:0] - Anti-aliasing filter bandwidth selection
	// To disable the accel, set the sampleRate bits to 0.
	tempRegValue = I2cReadByte(lsm->i2chandle, LSM9DS1_AG, LSM9DS1_CTRL_REG6_XL);
	tempRegValue &= 0b00000111;
	tempRegValue |= (LSM9DS1_ACC_SAMPLING_RATE & 0x07) << 5;
	switch (lsm->AccelScale)
	{
	case 4:
		tempRegValue |= (0x2 << 3);
		lsm->AccelSensitivity = LSM9DS1_SENSITIVITY_ACCELEROMETER_4;
		break;
	case 8:
		tempRegValue |= (0x3 << 3);
		lsm->AccelSensitivity = LSM9DS1_SENSITIVITY_ACCELEROMETER_8;
		break;
	case 16:
		tempRegValue |= (0x1 << 3);
		lsm->AccelSensitivity = LSM9DS1_SENSITIVITY_ACCELEROMETER_16;
		break;
	default :
		tempRegValue |= (0x0 << 3);
		lsm->AccelSensitivity = LSM9DS1_SENSITIVITY_ACCELEROMETER_2;
	}
	I2cWriteByte(lsm->i2chandle, LSM9DS1_AG, LSM9DS1_CTRL_REG6_XL, tempRegValue);
}

static void GyrInit(LSM9DS1 *lsm)
{
	uint8_t tempRegValue = 0;
	// CTRL_REG1_G (Default value: 0x00)
	// [ODR_G2][ODR_G1][ODR_G0][FS_G1][FS_G0][0][BW_G1][BW_G0]
	// ODR_G[2:0] - Output data rate selection
	// FS_G[1:0] - Gyroscope full-scale selection
	// BW_G[1:0] - Gyroscope bandwidth selection

	// To disable gyro, set sample rate bits to 0. We'll only set sample
	// rate if the gyro is enabled.
	tempRegValue = 0;
	tempRegValue = (LSM9DS1_GYR_SAMPLING_RATE & 0x07) << 5;
	switch (lsm->GyroScale)
	{
	case 500:
		tempRegValue |= (0x1 << 3);
		lsm->GyroSensitivity = LSM9DS1_SENSITIVITY_GYROSCOPE_500;
		break;
	case 2000:
		tempRegValue |= (0x3 << 3);
		lsm->GyroSensitivity = LSM9DS1_SENSITIVITY_GYROSCOPE_2000;
		break;
	default :
		tempRegValue |= (0x0 << 3);
		lsm->GyroSensitivity = LSM9DS1_SENSITIVITY_GYROSCOPE_245;
	}
	I2cWriteByte(lsm->i2chandle, LSM9DS1_AG, LSM9DS1_CTRL_REG1_G, tempRegValue);

	// CTRL_REG2_G (Default value: 0x00)
	// [0][0][0][0][INT_SEL1][INT_SEL0][OUT_SEL1][OUT_SEL0]
	// INT_SEL[1:0] - INT selection configuration
	// OUT_SEL[1:0] - Out selection configuration
	I2cWriteByte(lsm->i2chandle, LSM9DS1_AG, LSM9DS1_CTRL_REG2_G, 0x00);

	// CTRL_REG4 (Default value: 0x38)
	// [0][0][Zen_G][Yen_G][Xen_G][0][LIR_XL1][4D_XL1]
	// Zen_G - Z-axis output enable (0:disable, 1:enable)
	// Yen_G - Y-axis output enable (0:disable, 1:enable)
	// Xen_G - X-axis output enable (0:disable, 1:enable)
	// LIR_XL1 - Latched interrupt (0:not latched, 1:latched)
	// 4D_XL1 - 4D option on interrupt (0:6D used, 1:4D used)
	tempRegValue = 0;
	I2cWriteByte(lsm->i2chandle, LSM9DS1_AG, LSM9DS1_CTRL_REG4, 0b111000);

	// ORIENT_CFG_G (Default value: 0x00)
	// [0][0][SignX_G][SignY_G][SignZ_G][Orient_2][Orient_1][Orient_0]
	// SignX_G - Pitch axis (X) angular rate sign (0: positive, 1: negative)
	// Orient [2:0] - Directional user orientation selection
	I2cWriteByte(lsm->i2chandle, LSM9DS1_AG, LSM9DS1_ORIENT_CFG_G, 0);
}

static void MagInit(LSM9DS1 *lsm)
{
	uint8_t tempRegValue = 0;

	// CTRL_REG1_M (Default value: 0x10)
	// [TEMP_COMP][OM1][OM0][DO2][DO1][DO0][FAST_ODR][ST]
	// TEMP_COMP - Temperature compensation
	// OM[1:0] - X & Y axes op mode selection
	//	00:low-power, 01:medium performance
	//	10: high performance, 11:ultra-high performance
	// DO[2:0] - Output data rate selection
	// FAST_ODR - data rate higher than 80Hz (0:disable, 1:enable)
	// 	LP - 1000Hz
	// 	MP - 560Hz
	// 	HP - 300Hz
	// 	UHP - 155Hz
	// ST - Self-test enable
	tempRegValue = 0;
	tempRegValue |= (0b1 & 0x1) << 7;
	tempRegValue |= (0b11 & 0x3) << 5;
	tempRegValue |= (LSM9DS1_MAG_SAMPLING_RATE & 0x7) << 2;
	tempRegValue |= 0b00;	//change to 10 to set ODR faster
	I2cWriteByte(lsm->i2chandle, LSM9DS1_M, LSM9DS1_CTRL_REG1_M, tempRegValue);

	// CTRL_REG2_M (Default value 0x00)
	// [0][FS1][FS0][0][REBOOT][SOFT_RST][0][0]
	// FS[1:0] - Full-scale configuration
	// REBOOT - Reboot memory content (0:normal, 1:reboot)
	// SOFT_RST - Reset config and user registers (0:default, 1:reset)
	tempRegValue = 0;
	switch (lsm->MagnetScale)
	{
	case 8:
		tempRegValue |= (0x1 << 5);
		lsm->MagnetSensitivity = LSM9DS1_SENSITIVITY_MAGNETOMETER_8;
		break;
	case 12:
		tempRegValue |= (0x2 << 5);
		lsm->MagnetSensitivity = LSM9DS1_SENSITIVITY_MAGNETOMETER_12;
		break;
	case 16:
		tempRegValue |= (0x3 << 5);
		lsm->MagnetSensitivity = LSM9DS1_SENSITIVITY_MAGNETOMETER_16;
		break;
	default :
		tempRegValue |= (0x0 << 5);
		lsm->MagnetSensitivity = LSM9DS1_SENSITIVITY_MAGNETOMETER_4;
	}
	I2cWriteByte(LSM9DS1_M, LSM9DS1_CTRL_REG2_M, tempRegValue);

	// CTRL_REG3_M (Default value: 0x03)
	// [I2C_DISABLE][0][LP][0][0][SIM][MD1][MD0]
	// I2C_DISABLE - Disable I2C interace (0:enable, 1:disable)
	// LP - Low-power mode cofiguration (1:enable)
	// SIM - SPI mode selection (0:write-only, 1:read/write enable)
	// MD[1:0] - Operating mode
	//	00:continuous conversion, 01:single-conversion,
	//  10,11: Power-down
	I2cWriteByte(lsm->i2chandle, LSM9DS1_M, LSM9DS1_CTRL_REG3_M, 0); // Continuous conversion mode

	// CTRL_REG4_M (Default value: 0x00)
	// [0][0][0][0][OMZ1][OMZ0][BLE][0]
	// OMZ[1:0] - Z-axis operative mode selection
	//	00:low-power mode, 01:medium performance
	//	10:high performance, 10:ultra-high performance
	// BLE - Big/little endian data
	tempRegValue = 0;
	tempRegValue = (0b11 & 0x3) << 2;
	I2cWriteByte(lsm->i2chandle, LSM9DS1_M, LSM9DS1_CTRL_REG4_M, 0b1000);

	// CTRL_REG5_M (Default value: 0x00)
	// [0][BDU][0][0][0][0][0][0]
	// BDU - Block data update for magnetic data
	//	0:continuous, 1:not updated until MSB/LSB are read
	I2cWriteByte(lsm->i2chandle, LSM9DS1_M, LSM9DS1_CTRL_REG5_M, 0);
}

bool InitializeLSM9DS1(LSM9DS1 *lsm,I2CHandle i2chandle)
{
	if(lsm->initialized == false){
		if(I2cInitialize(i2chandle)){
			//=====接続確認=====
			uint8_t mTest = I2cReadByte(lsm->i2chandle, LSM9DS1_M, LSM9DS1_WHO_AM_I_M);
			uint8_t agTest = I2cReadByte(lsm->i2chandle, LSM9DS1_AG, LSM9DS1_WHO_AM_I_XG);
			if(mTest == LSM9DS1_WHO_AM_I_M_RSP && agTest == LSM9DS1_WHO_AM_I_AG_RSP){
				//=====FIFO有効化=====
				if(lsm->FifoEnable)FifoInit();
				//=====フィルタ有効化=====
				if(lsm->FilterEnable)FiltersInit();
				//=====センサ初期化=====
				AccInit(lsm);
				GyrInit(lsm);
				MagInit(lsm);
				lsm->initialized = true;
			}else{
				DebugMessage("LSM9DS1 WHO_AM_I test is not correct\n");
			}
		}
	}else{
		DebugMessage("LSM9DS1  was initialized twice\n");
	}
	return lsm->initialized;
}

bool ReadAccLSM9DS1(LSM9DS1 *lsm,float *x, float *y, float *z)
{
	if(lsm->initialized){
		uint8_t status = I2cReadByte(lsm->i2chandle, LSM9DS1_AG, LSM9DS1_STATUS_REG_1);
		if (status & 0b01)
		{
			uint8_t temp[6];
			I2cReadBytes(lsm->i2chandle, LSM9DS1_AG, LSM9DS1_OUT_X_L_XL, temp, 6);

			int16_t temp_;

			temp_ = (temp[1] << 8) | temp[0];
			*x = temp_ * lsm->AccelSensitivity;

			temp_ = (temp[3] << 8) | temp[2];
			*y = -temp_ * lsm->AccelSensitivity;

			temp_ = (temp[5] << 8) | temp[4];
			*z = temp_ * lsm->AccelSensitivity;
			return true;
		}
	}else{
		DebugMessage(InitErrMsg);
	}
	return false;
}

bool ReadGyrLSM9DS1(LSM9DS1 *lsm,float *x, float *y, float *z)
{
	if(lsm->initialized){
		uint8_t status = I2cReadByte(lsm->i2chandle, LSM9DS1_AG, LSM9DS1_STATUS_REG_1);
		if ((status & 0b10) >> 1)
		{
			uint8_t temp[6];
			I2cReadBytes(lsm->i2chandle, LSM9DS1_AG, LSM9DS1_OUT_X_L_G, temp, 6);

			int16_t temp_;

			temp_ = (temp[1] << 8) | temp[0];
			*x = temp_ * lsm->GyroSensitivity;

			temp_ = (temp[3] << 8) | temp[2];
			*y = -temp_ * lsm->GyroSensitivity;

			temp_ = (temp[5] << 8) | temp[4];
			*z = temp_ * lsm->GyroSensitivity;
			return true;
		}
	}else{
		DebugMessage(InitErrMsg);
	}
	return false;
}

void ReadAccFifoLSM9DS1(LSM9DS1 *lsm,float *x, float *y, float *z)
{
	if(lsm->initialized){
		if(lsm->FifoEnable){
			uint8_t temp[6];
			I2cReadBytes(lsm->i2chandle, LSM9DS1_AG, LSM9DS1_OUT_X_L_XL, temp, 6);

			int16_t temp_;

			temp_ = (temp[1] << 8) | temp[0];
			*x = temp_ * lsm->AccelSensitivity;

			temp_ = (temp[3] << 8) | temp[2];
			*y = -temp_ * lsm->AccelSensitivity;

			temp_ = (temp[5] << 8) | temp[4];
			*z = temp_ * lsm->AccelSensitivity;
		}else{
			DebugMessage(FifoErrMsg);
		}
	}else{
		DebugMessage(InitErrMsg);
	}
}

void ReadGyrFifoLSM9DS1(LSM9DS1 *lsmfloat *x, float *y, float *z)
{
	if(lsm->initialized){
		if(lsm->FifoEnable){
			uint8_t temp[6];
			I2cReadBytes(lsm->i2chandle, LSM9DS1_AG, LSM9DS1_OUT_X_L_G, temp, 6);

			int16_t temp_;

			temp_ = (temp[1] << 8) | temp[0];
			*x = temp_ * lsm->GyroSensitivity;

			temp_ = (temp[3] << 8) | temp[2];
			*y = -temp_ * lsm->GyroSensitivity;

			temp_ = (temp[5] << 8) | temp[4];
			*z = temp_ * lsm->GyroSensitivity;
		}else{
			DebugMessage(FifoErrMsg);
		}
	}else{
		DebugMessage(InitErrMsg);
	}
}

bool ReadMagLSM9DS1(LSM9DS1 *lsm,float *x, float *y, float *z)
{
	if(lsm->initialized){
		uint8_t status = I2cReadByte(lsm->i2chandle, LSM9DS1_M, LSM9DS1_STATUS_REG_1);
		if (status & 1)
		{
			uint8_t temp[6];
			I2cReadBytes(lsm->i2chandle, LSM9DS1_M, LSM9DS1_OUT_X_L_M, temp, 6);

			int16_t temp_;

			temp_ = (temp[1] << 8) | temp[0];
			*x = temp_ * lsm->GyroSensitivity;

			temp_ = (temp[3] << 8) | temp[2];
			*y = -temp_ * lsm->GyroSensitivity;

			temp_ = (temp[5] << 8) | temp[4];
			*z = temp_ * lsm->GyroSensitivity;
			return true;
		}
	}else{
		DebugMessage(InitErrMsg);
	}
	return false;
}

int availableFifoLSM9DS1(LSM9DS1 *lsm)
{
	// FIFO_SRC (0x2F)
	// [FTH][OVRN][FSS5][FSS4][FSS3][FSS2][FSS1][FSS0]
	// FTH -  FIFO threshold status.
	//	(0: FIFO filling is lower than threshold level; 1: FIFO filling is equal or higher than threshold level)
	// OVRN - FIFO overrun status.
	//	(0 : FIFO is not completely filled; 1 : FIFO is completely filled and at least one samples has been overwritten)
	//FSS [5:0] Number of unread samples stored into FIFO.
	//	(000000 : FIFO empty; 100000 : FIFO full, 32 unread samples)
	if(lsm->initialized){
		uint8_t status = I2cReadByte(lsm->i2chandle, LSM9DS1_AG, LSM9DS1_FIFO_SRC);
		return status & 0b111111;
	}else{
		DebugMessage(FifoErrMsg);
	}
}