/*

	2019 Oku Dan / oku_dan@yahoo.co.jp

	STMicroelectronics社製LPS331AP用のI2C版ライブラリです.
	使用する際はデバイスのSDAとSCLをセンサに接続してください.
	内部プルアップではないので注意.

	1.LPS331AP型構造体を作成
	2.InitializeLPS331AP()で初期化
	3.ReadPrs(),ReadTmp()で気圧、気温読み出し
	4.3に戻る

	ODRでサンプリングレートを変更できる（デフォルトは25Hz,25Hz)
	このセンサの更新速度にデバイス側で合わせて読みだせばSTATUS_REGのチェックが不要になる

*/

#include "I2CHandler.h"
#include "SystemHandler.h"

#define LPS331AP_ADDR0 0b1011100 //SA0=L(GND)
#define LPS331AP_ADDR1 0b1011101 //SA0=H(VDD)
#define LPS331AP_REF_P_XL 0x08
#define LPS331AP_REF_P_L 0x09
#define LPS331AP_REF_P_H 0x0A
#define LPS331AP_WHO_AM_I 0x0F
#define LPS331AP_WHO_AM_I_DEFAULT 0xBB
#define LPS331AP_RES_CONF 0x10
#define LPS331AP_CTRL_REG1 0x20
#define LPS331AP_CTRL_REG2 0x21
#define LPS331AP_CTRL_REG3 0x22
#define LPS331AP_INTERRUPT_CFG 0x23
#define LPS331AP_INT_SOURCE 0x24
#define LPS331AP_THS_P_L 0x25
#define LPS331AP_THS_P_H 0x26
#define LPS331AP_STATUS_REG 0x27
#define LPS331AP_PRESS_OUT_XL 0x28
#define LPS331AP_PRESS_OUT_L 0x29
#define LPS331AP_PRESS_OUT_H 0x2A
#define LPS331AP_TEMP_OUT_L 0x2B
#define LPS331AP_TEMP_OUT_H 0x2C
#define LPS331AP_AMP_CTRL 0x30
#define LPS331AP_DELTA_PRESS_XL 0x3C
#define LPS331AP_DELTA_PRESS_L 0x3D
#define LPS331AP_DELTA_PRESS_H 0x3E

#define LPS331AP_ODR 0b111
//	(Hz)	pressure	Temperature
//	000  -	One Shot	One Shot
//	001	 -	1			1
//	010  -	7			1
//	011  -	12.5		1
//	101  -	7			7
//	110  -	12.5		12.5
//	111  -	25			25

static const char InitErrMsg[] = "This LPS331AP has not been initialized\n";

typedef struct
{
	I2CHandle *i2chandle;
	uint8_t address;
	bool initialized = false;
	bool SA0 = 0;
} LPS331AP;

static void RegistersInit(LPS331AP *lps){	
	uint8_t tempRegValue = 0;

	//Power down before change config
	tempRegValue = 0;
	I2cWriteByte(lps->i2chandle, lps->address LPS331AP_CTRL_REG1, tempRegValue);

	//Reference pressure
	tempRegValue = 0;
	I2cWriteByte(lps->i2chandle, lps->address LPS331AP_REF_P_XL, tempRegValue);
	I2cWriteByte(lps->i2chandle, lps->address LPS331AP_REF_P_L, tempRegValue);
	I2cWriteByte(lps->i2chandle, lps->address LPS331AP_REF_P_H, tempRegValue);

	//RES_CONF
	//[RFU][AVGT2][AVGT1][AVGT0][AVGP3][AVGP2][AVGP1][AVGP0]
	//RFU - reserved for future
	//AVGT[2:0] - Temperature resolution configuration(Average of AVGT times)
	//	000:1
	//	001:2
	//	  ~
	//	111:128
	//AVGP[3:0] - Pressure resolution configuration(Average of AVGP times)
	//	0000:1
	//	0001:2
	//	  ~
	//	1111:512
	//Register configuration 7Ah not allowed with ODR = 25Hz/25Hz (Register CTRL_REG1).
	//For ORD 25Hz/25Hz the suggested configuration for RES_CONF is 6Ah.
	tempRegValue = 0b01111111;
	if(LPS331AP_ODR == 0b111)tempRegValue = 0x6A; 	//= 0b01101010
	I2cWriteByte(lps->i2chandle, lps->address LPS331AP_RES_CONF, tempRegValue);

	//CTRL_REG2
	//[BOOT][RESERVED][RESERVED][RESERVED][RESERVED][SWRESET][AUTO_ZERO][ONE_SHOT]
	//BOOT -  Reboot memory content. Default value: 0
	//SWRESET - Software reset. Default value: 0
	//AUTO_ZERO - Autozero enable. Default value: 0
	//ONE_SHOT -  One shot enable. Default value: 0
	tempRegValue = 0;
	I2cWriteByte(lps->i2chandle, lps->address LPS331AP_CTRL_REG2, tempRegValue);

	
	//CTRL_REG3
	//INTERRUPT_CFG
	//INT_SOURCE
	//THS_P_L
	//THS_P_H
	//To set about interrupt.
	tempRegValue = 0;
	I2cWriteByte(lps->i2chandle, lps->address LPS331AP_CTRL_REG3, tempRegValue);
	I2cWriteByte(lps->i2chandle, lps->address LPS331AP_INTERRUPT_CFG, tempRegValue);
	I2cWriteByte(lps->i2chandle, lps->address LPS331AP_INT_SOURCE, tempRegValue);
	I2cWriteByte(lps->i2chandle, lps->address LPS331AP_THS_P_L, tempRegValue);
	I2cWriteByte(lps->i2chandle, lps->address LPS331AP_THS_P_H, tempRegValue);

	//CTRL_REG1
	//[PD][ODR2][ODR1][ODR0][DIFF_EN][DBDU][DELTA_EN][SIM]
	//PD - PD: power down control(0: power-down mode; 1: active mode)
	//ODR[2:0] - output data rate selection
	//DIFF_EN - Interrupt circuit enable.(0: interrupt generation disabled; 1: interrupt circuit enabled)
	//DBDU - block data update(0: continuous update 1: output registers not updated until MSB and LSB reading)
	//DELTA_EN - delta pressure enable(1: delta pressure registers enabled. 0: disable)
	//SIM -  SPI Serial Interface Mode selection.(0: 4-wire interface; 1: 3-wire interface)
	tempRegValue = 0b10000100;
	tempRegValue |= LPS331AP_ODR << 4;
	I2cWriteByte(lps->i2chandle, lps->address LPS331AP_CTRL_REG1, tempRegValue);
	return;
}

bool InitializeLPS331AP(LPS331AP *lps,I2CHandle *i2c){
	if(lps->initialized == false){
		lps->i2chandle = i2c;
		if(I2cInitialize(lps->i2chandle) == true){
			lps->address = (lps->SA0 == 1 ? LPS331AP_ADDR1 : LPS331AP_ADDR0);
			if(I2cReadByte(lps->address, LPS331AP_WHO_AM_I) == LPS331AP_WHO_AM_I_DEFAULT){
				RegistersInit(lps);
				lps->initialized = true;
			}else{
				DebugMessage("LPS331AP WHO_AM_I test is not correct\n");
			}
		}
	}else{
		DebugMessage("LPS331AP was initialized twice\n");
	}

	return lps->initialized;
}

bool ReadPrsLPS331AP(LPS331AP *lps,float *prs){
	if(lps->initialized){
		uint8_t status;

		//STATUS_REG
		//[0][0][P_OR][T_OR][0][0][P_DA][T_DA]
		//P_OR - Pressure data overrun.Sampling rate is too fast.
		//T_OR - Temperature data overrun.
		//P_DA - Pressure data available.
		//T_DA - Temperature data available.
		status = I2cReadByte(lps->i2chandle, lps->address LPS331AP_STATUS_REG);

		if((status & 0b00000010) != 0){
			uint8_t prsTempXL,prsTempL,prsTempH;
			prsTempXL = I2cReadByte(lps->i2chandle, lps->address LPS331AP_PRESS_OUT_XL);
			prsTempL = I2cReadByte(lps->i2chandle, lps->address LPS331AP_PRESS_OUT_L);
			prsTempH = I2cReadByte(lps->i2chandle, lps->address LPS331AP_PRESS_OUT_H);
			*prs = ((uint32_t)prsTempH << 16 | (uint16_t)prsTempL << 8 | prsTempXL) / 4096.0f;
			return true;
		}
	}else{
		DebugMessage(InitErrMsg);
	}
	return false;
}

bool ReadTmpLPS331AP(float *tmp){
	if(lps->initialized){
		uint8_t status;
		status = I2cReadByte(lps->i2chandle, lps->address LPS331AP_STATUS_REG);
		if((status & 0b00000001) != 0){
			uint8_t tmpTempL,tmpTempH;
			tmpTempL = I2cReadByte(lps->i2chandle, lps->address LPS331AP_TEMP_OUT_L);
			tmpTempH = I2cReadByte(lps->i2chandle, lps->address LPS331AP_TEMP_OUT_H);
			*tmp =  42.5 + (int16_t)(tmpTempH << 8 | tmpTempL) / 480.0f;
			return true;
		}
	}else{
		DebugMessage(InitErrMsg);
	}
	return false;
}
