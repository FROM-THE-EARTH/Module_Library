#ifndef INC_IM920_H_
#define INC_IM920_H_

#define IM920_Mode_HighSpeed 1
#define IM920_Mode_LongRange 2

typedef struct{
	uint16_t ID;	//ID
	uint16_t RID;	//ReceiveID
	uint8_t NN;		//NodeNumber
	uint8_t BRT;	//BaudRate
					//	0:1200bps
					//	1:2400bps
					//	2:4800bps
					//	3:9600bps
					//	4:19200bps
					//	5:38400bps
					//	6:57600bps
					//	7:115200bps
	uint8_t PO;		//OutputPower
					//	1:0.1mW
					//	2:1mW
					//	3:10mW
	uint8_t RT;		//Rate
					//	1:HighSpeed
					//	2:LongRange
	uint8_t CH;		//Channel
	uint16_t STM;	//SleepTime
	uint16_t WTM;	//WaitTime
	bool ABK;		//AnswerBack
	bool CIO;		//CharacteristicInputOutput
	bool RPT;		//RelayPacketTransimit
}IM920_Setting;

void IM920_UART_Receive(uint8_t c);

bool IM920_Initialize();
bool IM920_SetReieveID(uint16_t ID);
bool IM920_EraceReieveIDs();
bool IM920_SetNodeNumber(uint8_t NodeNumber);
bool IM920_SetChannel(uint8_t Channel);
bool IM920_SetRateMode(uint8_t Mode);
bool IM920_SetRelayMode(bool RelayMode);
bool IM920_Sleep();
bool IM920_UnSleep();
bool IM920_Send(uint8_t *data,uint16_t len);
bool IM920_NewMessage();
void IM920_Read(uint8_t *data);

#endif
