#include "IM920.h"

#define BUFFER_LEN 100

#include "main.h"
extern UART_HandleTypeDef huart1;

IM920_Setting setting;

uint8_t receivedMessage[BUFFER_LEN],*bufferPtr;
bool waitingResp = false,waitingMssg = false;

//----------------------------------------------
void UartWriteMulti(uint8_t *data,uint8_t len){
	HAL_UART_Transmit(&huart1, data, len, 100);
}

bool CheckBusy(){
	return (HAL_GPIO_ReadPin(IMBUSY_GPIO_Port,IMBUSY_Pin) == GPIO_PIN_SET);
}

void IM920_UART_Receive(uint8_t c){
	if(waitingResp){
		*bufferPtr++ = c;
		if(c == '\n'){
			waitingResp = false;
		}
	}else if(waitingMssg){
		*bufferPtr++ = c;
		if(c == '\n')waitingMssg = false;
	}
}

static void Delay(uint16_t ms){
	HAL_Delay(1);
}
//----------------------------------------------

static uint8_t ConvChar16ToInt16(char c){
	if('0' <= c && c <= '9')return c - '0';
	else return 10 + (c - 'A');
}

static uint16_t ConvStrToInt16(uint8_t *ptr){
	uint16_t t = 1;
	uint16_t res = 0;

	uint8_t i;
	for(i = 0;('0' <= ptr[i] && ptr[i] <= '9') || ('A' <= ptr[i] && ptr[i] <= 'F');i++);
	for(;i > 0;i--){
		res += ConvChar16ToInt16(ptr[i - 1]) * t;
		t *= 16;
	}
	return res;
}

static char ConvInt16ToChar16(uint8_t t){
    if(t <= 9)return '0' + t;
    else return 'A' - 10 + t;
}

static void StartReceive(){
	bufferPtr = &receivedMessage[0];
	waitingResp = false;
	waitingMssg = true;
}

static bool WaitResponce(uint8_t *buffer){
	bufferPtr = buffer;
	waitingMssg = false;
	waitingResp = true;

	for(uint32_t i = 0;i < 0x2000;i++){
		if(!waitingResp){
			StartReceive();
			return true;
		}
		Delay(1);
	}
	return false;
}

static bool ReadParam(const char* cmd,uint16_t *param){
	if(CheckBusy())return false;

	uint8_t _cmd[] = {cmd[0],cmd[1],cmd[2],cmd[3],'\r','\n'};
	uint8_t responceBuffer[20];

	UartWriteMulti(_cmd,6);
	if(!WaitResponce(responceBuffer))return false;

	if(responceBuffer[0] == 'N' && responceBuffer[1] == 'G'){
		return false;
	}
	else{
		*param = ConvStrToInt16(responceBuffer);
		return true;
	}
}

static bool SetParam(const char* cmd,uint16_t param,uint8_t param_len){
	if(CheckBusy())return false;

	UartWriteMulti((uint8_t*)cmd,4);
	if(param_len == 2){
		uint8_t param_str[3];
		param_str[0] = ' ';
		param_str[1] = ConvInt16ToChar16(param%0x100/0x10);
		param_str[2] = ConvInt16ToChar16(param%0x10);
		UartWriteMulti(param_str,3);
	}else if(param_len == 4){
		uint8_t param_str[5];
		param_str[0] = ' ';
		param_str[1] = ConvInt16ToChar16(param%0x10000/0x1000);
		param_str[2] = ConvInt16ToChar16(param%0x1000/0x100);
		param_str[3] = ConvInt16ToChar16(param%0x100/0x10);
		param_str[4] = ConvInt16ToChar16(param%0x10);
		UartWriteMulti(param_str,5);
	}else if(param_len == 1){
		uint8_t param_str[2];
		param_str[0] = ' ';
		param_str[1] = param;
		UartWriteMulti(param_str,2);
	}
	UartWriteMulti((uint8_t*)"\r\n",2);

	uint8_t responceBuffer[5];
	if(!WaitResponce(responceBuffer))return false;

	if(responceBuffer[0] == 'N' && responceBuffer[1] == 'G')return false;
	else return true;
}

bool IM920_Initialize(){
	uint16_t temp;
	if(ReadParam("RDID",&temp))setting.ID = temp;
	else return false;
	if(ReadParam("RDNN",&temp))setting.NN = temp;
	else return false;
	if(ReadParam("RDCH",&temp))setting.CH = temp;
	else return false;
	StartReceive();
	return true;
}

bool IM920_SetReieveID(uint16_t ID){
	if(SetParam("ENWR",0,0)){
		return SetParam("SRID",ID,4);
	}else return false;
}

bool IM920_EraceReieveIDs(){
	if(SetParam("ENWR",0,0)){
		return SetParam("ERID",0,0);
	}else return false;
}

bool IM920_SetNodeNumber(uint8_t NodeNumber){
	return SetParam("STNN",NodeNumber,2);
}

bool IM920_SetChannel(uint8_t Channel){
	if(SetParam("ENWR",0,0)){
		return SetParam("STCH",Channel,2);
	}else return false;
}

bool IM920_SetRateMode(uint8_t Mode){
	return SetParam("STRT",Mode,1);
}

bool IM920_SetRelayMode(bool RelayMode){
	if(RelayMode)return SetParam("ERPT",0,0);
	else return SetParam("DRPT",0,0);
}

bool IM920_Sleep(){
	return SetParam("DSRX",0,0);
}

bool IM920_UnSleep(){
	UartWriteMulti((uint8_t*)"?",1);
	while(CheckBusy());
	return SetParam("ENRX",0,0);
}

bool IM920_Send(uint8_t *data,uint16_t len){
	while(CheckBusy());
	uint8_t _len;
	for(_len = 0;_len<len&&_len<64&&data[len] != '\r'&&data[len] != '\n';_len++);
	UartWriteMulti((uint8_t*)"TXDA ",5);
	UartWriteMulti(data,_len);
	UartWriteMulti((uint8_t*)"\r\n",2);

	uint8_t responceBuffer[5];
	if(!WaitResponce(responceBuffer))return false;

	if(responceBuffer[0] == 'N' && responceBuffer[1] == 'G')return false;
	return true;
}

bool IM920_NewMessage(){
	return !waitingMssg;
}

void IM920_Read(uint8_t *data){
	if(!waitingMssg){
		for(uint16_t i = 0;i < BUFFER_LEN;i++){
			data[i] = receivedMessage[i];
			receivedMessage[i] = 0;
			if(receivedMessage[i] == '\n')break;
		}
		StartReceive();
	}
	return;
}
