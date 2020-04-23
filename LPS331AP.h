/*

	2019 Oku Dan / oku_dan@yahoo.co.jp

	STMicroelectronics社製LPS331AP用のI2C版ライブラリです.
	使用する際はデバイスのSDAとSCLをセンサに接続してください.
	内部プルアップではないので注意.

	1.LPS_331AP型クラスを作成
	2.Initialize()で初期化
	3.ReadPrs(&prs),ReadTmp(&tmp)で気圧、気温読み出し
	4.3に戻る

	49行目ODRでサンプリングレートを変更できる（デフォルトは25Hz,25Hz)
	このセンサの更新速度にデバイス側で合わせて読みだせばSTATUS_REGのチェックが不要になる

*/

#include "I2CHandler.h"

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

class LPS_331AP
{
private:
	bool initialized = false;
	I2CHandler *handler;
	void RegistersInit();
	uint8_t LPS331AP_ADDR;

public:
	bool Initialize(I2CHandler *handler, uint8_t SA0);
	bool ReadPrs(float *prs);
	bool ReadTmp(float *tmp);
};