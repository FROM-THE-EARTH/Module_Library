#include "LSM9DS1.h"

void LSM_9DS1::FIFOInit()
{
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
	I2cWriteByte(handler, LSM9DS1_AG, LSM9DS1_CTRL_REG9, tempRegValue);

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
	I2cWriteByte(handler, LSM9DS1_AG, LSM9DS1_FIFO_CTRL, tempRegValue);
}

void LSM_9DS1::FiltersInit()
{
	uint8_t tempRegValue = 0;

	// REFERENCE_G (0x0B)
	// [REF7_G][REF6_G][REF5_G][REF4_G][REF3_G][REF2_G][REF1_G][REF0_G]
	//	REF_G [7:0] Reference value for gyroscope’s digital high-pass filter (r/w).(Default value : 0000 0000)
	I2cWriteByte(handler, LSM9DS1_AG, LSM9DS1_REFERENCE_G, 0x00);

	// CTRL_REG3_G (Default value: 0x00)
	// [LP_mode][HP_EN][0][0][HPCF3_G][HPCF2_G][HPCF1_G][HPCF0_G]
	// LP_mode - Low-power mode enable (0: disabled, 1: enabled)
	// HP_EN - HPF enable (0:disabled, 1: enabled)
	// HPCF_G[3:0] - HPF cutoff frequency (0000 ~ 1001 depends on ODR)
	tempRegValue |= (0b01 & 0x03) << 6;
	tempRegValue |= (0b0000 & 0x11);
	I2cWriteByte(handler, LSM9DS1_AG, LSM9DS1_CTRL_REG3_G, tempRegValue);

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
	tempRegValue = I2cReadByte(handler, LSM9DS1_AG, LSM9DS1_CTRL_REG6_XL);
	tempRegValue &= 0b11111000;
	tempRegValue |= (0b000 & 0x07);
	I2cWriteByte(handler, LSM9DS1_AG, LSM9DS1_CTRL_REG6_XL, tempRegValue);

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
	I2cWriteByte(handler, LSM9DS1_AG, LSM9DS1_CTRL_REG7_XL, tempRegValue);
}

void LSM_9DS1::AccInit(int scale)
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
	I2cWriteByte(handler, LSM9DS1_AG, LSM9DS1_CTRL_REG5_XL, 0b111000);

	// CTRL_REG6_XL (0x20) (Default value: 0x00)
	// [ODR_XL2][ODR_XL1][ODR_XL0][FS1_XL][FS0_XL][BW_SCAL_ODR][BW_XL1][BW_XL0]
	// ODR_XL[2:0] - Output data rate & power mode selection
	// FS_XL[1:0] - Full-scale selection
	// BW_SCAL_ODR - Bandwidth selection
	// BW_XL[1:0] - Anti-aliasing filter bandwidth selection
	// To disable the accel, set the sampleRate bits to 0.
	tempRegValue = I2cReadByte(handler, LSM9DS1_AG, LSM9DS1_CTRL_REG6_XL);
	tempRegValue &= 0b00000111;
	tempRegValue |= (LSM9DS1_ACC_SAMPLING_RATE & 0x07) << 5;
	switch (scale)
	{
	case 4:
		tempRegValue |= (0x2 << 3);
		AccelSensitivity = LSM9DS1_SENSITIVITY_ACCELEROMETER_4;
		break;
	case 8:
		tempRegValue |= (0x3 << 3);
		AccelSensitivity = LSM9DS1_SENSITIVITY_ACCELEROMETER_8;
		break;
	case 16:
		tempRegValue |= (0x1 << 3);
		AccelSensitivity = LSM9DS1_SENSITIVITY_ACCELEROMETER_16;
		break;
	default :
		tempRegValue |= (0x0 << 3);
		AccelSensitivity = LSM9DS1_SENSITIVITY_ACCELEROMETER_2;
	}
	I2cWriteByte(handler, LSM9DS1_AG, LSM9DS1_CTRL_REG6_XL, tempRegValue);
}

void LSM_9DS1::GyrInit(int scale)
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
	switch (scale)
	{
	case 500:
		tempRegValue |= (0x1 << 3);
		GyroSensitivity = LSM9DS1_SENSITIVITY_GYROSCOPE_500;
		break;
	case 2000:
		tempRegValue |= (0x3 << 3);
		GyroSensitivity = LSM9DS1_SENSITIVITY_GYROSCOPE_2000;
		break;
	default :
		tempRegValue |= (0x0 << 3);
		GyroSensitivity = LSM9DS1_SENSITIVITY_GYROSCOPE_245;
	}
	I2cWriteByte(handler, LSM9DS1_AG, LSM9DS1_CTRL_REG1_G, tempRegValue);

	// CTRL_REG2_G (Default value: 0x00)
	// [0][0][0][0][INT_SEL1][INT_SEL0][OUT_SEL1][OUT_SEL0]
	// INT_SEL[1:0] - INT selection configuration
	// OUT_SEL[1:0] - Out selection configuration
	I2cWriteByte(handler, LSM9DS1_AG, LSM9DS1_CTRL_REG2_G, 0x00);

	// CTRL_REG4 (Default value: 0x38)
	// [0][0][Zen_G][Yen_G][Xen_G][0][LIR_XL1][4D_XL1]
	// Zen_G - Z-axis output enable (0:disable, 1:enable)
	// Yen_G - Y-axis output enable (0:disable, 1:enable)
	// Xen_G - X-axis output enable (0:disable, 1:enable)
	// LIR_XL1 - Latched interrupt (0:not latched, 1:latched)
	// 4D_XL1 - 4D option on interrupt (0:6D used, 1:4D used)
	tempRegValue = 0;
	I2cWriteByte(handler, LSM9DS1_AG, LSM9DS1_CTRL_REG4, 0b111000);

	// ORIENT_CFG_G (Default value: 0x00)
	// [0][0][SignX_G][SignY_G][SignZ_G][Orient_2][Orient_1][Orient_0]
	// SignX_G - Pitch axis (X) angular rate sign (0: positive, 1: negative)
	// Orient [2:0] - Directional user orientation selection
	I2cWriteByte(handler, LSM9DS1_AG, LSM9DS1_ORIENT_CFG_G, 0);
}

void LSM_9DS1::MagInit(int scale)
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
	I2cWriteByte(handler, LSM9DS1_M, LSM9DS1_CTRL_REG1_M, tempRegValue);

	// CTRL_REG2_M (Default value 0x00)
	// [0][FS1][FS0][0][REBOOT][SOFT_RST][0][0]
	// FS[1:0] - Full-scale configuration
	// REBOOT - Reboot memory content (0:normal, 1:reboot)
	// SOFT_RST - Reset config and user registers (0:default, 1:reset)
	tempRegValue = 0;
	switch (scale)
	{
	case 8:
		tempRegValue |= (0x1 << 5);
		MagnetSensitivity = LSM9DS1_SENSITIVITY_MAGNETOMETER_8;
		break;
	case 12:
		tempRegValue |= (0x2 << 5);
		MagnetSensitivity = LSM9DS1_SENSITIVITY_MAGNETOMETER_12;
		break;
	case 16:
		tempRegValue |= (0x3 << 5);
		MagnetSensitivity = LSM9DS1_SENSITIVITY_MAGNETOMETER_16;
		break;
	default :
		tempRegValue |= (0x0 << 5);
		MagnetSensitivity = LSM9DS1_SENSITIVITY_MAGNETOMETER_4;
	}
	I2cWriteByte(handler, LSM9DS1_M, LSM9DS1_CTRL_REG2_M, tempRegValue);

	// CTRL_REG3_M (Default value: 0x03)
	// [I2C_DISABLE][0][LP][0][0][SIM][MD1][MD0]
	// I2C_DISABLE - Disable I2C interace (0:enable, 1:disable)
	// LP - Low-power mode cofiguration (1:enable)
	// SIM - SPI mode selection (0:write-only, 1:read/write enable)
	// MD[1:0] - Operating mode
	//	00:continuous conversion, 01:single-conversion,
	//  10,11: Power-down
	I2cWriteByte(handler, LSM9DS1_M, LSM9DS1_CTRL_REG3_M, 0); // Continuous conversion mode

	// CTRL_REG4_M (Default value: 0x00)
	// [0][0][0][0][OMZ1][OMZ0][BLE][0]
	// OMZ[1:0] - Z-axis operative mode selection
	//	00:low-power mode, 01:medium performance
	//	10:high performance, 10:ultra-high performance
	// BLE - Big/little endian data
	tempRegValue = 0;
	tempRegValue = (0b11 & 0x3) << 2;
	I2cWriteByte(handler, LSM9DS1_M, LSM9DS1_CTRL_REG4_M, 0b1000);

	// CTRL_REG5_M (Default value: 0x00)
	// [0][BDU][0][0][0][0][0][0]
	// BDU - Block data update for magnetic data
	//	0:continuous, 1:not updated until MSB/LSB are read
	I2cWriteByte(handler, LSM9DS1_M, LSM9DS1_CTRL_REG5_M, 0);
}

bool LSM_9DS1::Initialize(I2CHandler *handler){
	Initialize(handler, 16, 2000, 16);
}

bool LSM_9DS1::Initialize(I2CHandler *i2chandler, int AccelScale, int GyroScale, int MagnetScale)
{
	if(initialized){
		if(I2cInitialize(i2chandler)){
			handler = i2chandler;
			//=====接続確認=====
			uint8_t mTest = I2cReadByte(handler, LSM9DS1_M, LSM9DS1_WHO_AM_I_M);
			uint8_t agTest = I2cReadByte(handler, LSM9DS1_AG, LSM9DS1_WHO_AM_I_XG);
			if (LSM9DS1_WHO_AM_I_AG_RSP == agTest && LSM9DS1_WHO_AM_I_M_RSP == mTest)
			{
				//=====FIFO有効化=====
				if(FIFOenabled)FIFOInit();
				//=====フィルタ有効化=====
				if(Filterenabled)FiltersInit();
				//=====センサ初期化=====
				AccInit(AccelScale);
				GyrInit(GyroScale);
				MagInit(MagnetScale);
				initialized = true;
			}
		}
	}
	return initialized;
}

bool LSM_9DS1::ReadAcc(float *x, float *y, float *z)
{
	if(initialized){
		uint8_t status = I2cReadByte(handler, LSM9DS1_AG, LSM9DS1_STATUS_REG_1);
		if (status & 0b01)
		{
			uint8_t temp[6];
			I2cReadBytes(handler, LSM9DS1_AG, LSM9DS1_OUT_X_L_XL, temp, 6);

			int16_t temp_;

			temp_ = (temp[1] << 8) | temp[0];
			*x = temp_ * AccelSensitivity;

			temp_ = (temp[3] << 8) | temp[2];
			*y = -temp_ * AccelSensitivity;

			temp_ = (temp[5] << 8) | temp[4];
			*z = temp_ * AccelSensitivity;
			return true;
		}
	}
	return false;
}

bool LSM_9DS1::ReadGyr(float *x, float *y, float *z)
{
	if(initialized){
		uint8_t status = I2cReadByte(handler, LSM9DS1_AG, LSM9DS1_STATUS_REG_1);
		if ((status & 0b10) >> 1)
		{
			uint8_t temp[6];
			I2cReadBytes(handler, LSM9DS1_AG, LSM9DS1_OUT_X_L_G, temp, 6);

			int16_t temp_;

			temp_ = (temp[1] << 8) | temp[0];
			*x = temp_ * GyroSensitivity;

			temp_ = (temp[3] << 8) | temp[2];
			*y = -temp_ * GyroSensitivity;

			temp_ = (temp[5] << 8) | temp[4];
			*z = temp_ * GyroSensitivity;
			return true;
		}
	}
	return false;
}

bool LSM_9DS1::ReadAccFIFO(float *x, float *y, float *z)
{
	if(initialized && FIFOenabled){
		uint8_t temp[6];
		I2cReadBytes(handler, LSM9DS1_AG, LSM9DS1_OUT_X_L_XL, temp, 6);

		int16_t temp_;

		temp_ = (temp[1] << 8) | temp[0];
		*x = temp_ * AccelSensitivity;

		temp_ = (temp[3] << 8) | temp[2];
		*y = -temp_ * AccelSensitivity;

		temp_ = (temp[5] << 8) | temp[4];
		*z = temp_ * AccelSensitivity;
		return true;
	}
	return false;
}

bool LSM_9DS1::ReadGyrFIFO(float *x, float *y, float *z)
{
	if(initialized && FIFOenabled){
		uint8_t temp[6];
		I2cReadBytes(handler, LSM9DS1_AG, LSM9DS1_OUT_X_L_G, temp, 6);

		int16_t temp_;

		temp_ = (temp[1] << 8) | temp[0];
		*x = temp_ * GyroSensitivity;

		temp_ = (temp[3] << 8) | temp[2];
		*y = -temp_ * GyroSensitivity;

		temp_ = (temp[5] << 8) | temp[4];
		*z = temp_ * GyroSensitivity;
		return true;
	}
	return false;
}

bool LSM_9DS1::ReadMag(float *x, float *y, float *z)
{
	if(initialized){
		uint8_t status = I2cReadByte(handler, LSM9DS1_M, LSM9DS1_STATUS_REG_1);
		if (status & 1)
		{
			uint8_t temp[6];
			I2cReadBytes(handler, LSM9DS1_M, LSM9DS1_OUT_X_L_M, temp, 6);

			int16_t temp_;

			temp_ = (temp[1] << 8) | temp[0];
			*x = temp_ * GyroSensitivity;

			temp_ = (temp[3] << 8) | temp[2];
			*y = -temp_ * GyroSensitivity;

			temp_ = (temp[5] << 8) | temp[4];
			*z = temp_ * GyroSensitivity;
			return true;
		}
	}
	return false;
}

int LSM_9DS1::availableFIFO()
{
	if(initialized && FIFOenabled){
		// FIFO_SRC (0x2F)
		// [FTH][OVRN][FSS5][FSS4][FSS3][FSS2][FSS1][FSS0]
		// FTH -  FIFO threshold status.
		//	(0: FIFO filling is lower than threshold level; 1: FIFO filling is equal or higher than threshold level)
		// OVRN - FIFO overrun status.
		//	(0 : FIFO is not completely filled; 1 : FIFO is completely filled and at least one samples has been overwritten)
		//FSS [5:0] Number of unread samples stored into FIFO.
		//	(000000 : FIFO empty; 100000 : FIFO full, 32 unread samples)
		uint8_t status = I2cReadByte(handler, LSM9DS1_AG, LSM9DS1_FIFO_SRC);
		return status & 0b111111;
	}
	return 0;
}