#include "IM920.h"

#define IM920_BUFFER_LEN 256

#ifdef STM32
#include "main.h"
extern UART_HandleTypeDef huart1;
#endif

static IM920_Setting setting;

static uint8_t messageBuffer[IM920_BUFFER_LEN];
static uint8_t responceBuffer[20];

static uint16_t messageBufferWritePosition = 0;
static uint16_t messageBufferReadPosition = 0;
static uint8_t responceBufferWritePosition = 0;

static uint8_t newMessages = 0;

static volatile bool waitingResp = false;
static volatile bool waitingMssg = false;

//----------------------------------------------
void UartWriteMulti(uint8_t *data,uint8_t len){
#ifdef STM32
	HAL_UART_Transmit(&huart1, data, len, 100);
#endif
}

static bool CheckBusy(){
	bool busy = false;
#ifdef STM32
	busy = (HAL_GPIO_ReadPin(IMBUSY_GPIO_Port,IMBUSY_Pin) == GPIO_PIN_SET);
#endif
	return busy;
}

static void SoftReset(){
#ifdef STM32
	HAL_GPIO_WritePin(IMRESET_GPIO_Port,IMRESET_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(IMRESET_GPIO_Port,IMRESET_Pin,GPIO_PIN_RESET);
#endif
	Delay(10);
}

void IM920_UART_Receive(uint8_t c){
	if(waitingResp){
		responceBuffer[responceBufferWritePosition++] = c;

		if(c == '\n')waitingResp = false;
	}else if(waitingMssg){
		messageBuffer[messageBufferWritePosition] = c;
		
		messageBufferWritePosition++;
		if(messageBufferWritePosition >= IM920_BUFFER_LEN)messageBufferWritePosition = 0;

		if(c == '\n')newMessages++;
	}
}

static void Delay(uint16_t ms){
#ifdef STM32
	HAL_Delay(1);
#endif
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

static void WaitMessage(){
	waitingMssg = true;
}

static bool WaitResponce(){
	responceBufferWritePosition = 0;
	for(int i = 0;i < sizeof(responceBuffer);i++)responceBuffer[i] = 0;

	waitingResp = true;

	bool res = false;
	for(uint32_t i = 0;i < 0x2000;i++){
		if(!waitingResp){
			res = true;
			break;
		}else{
			Delay(1);
		}
	}

	waitingResp = false;
	return res;
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

	uint8_t _cmd[11] = {cmd[0],cmd[1],cmd[2],cmd[3]};
	if(param_len == 1){
		_cmd[4] = ' ';
		_cmd[5] = param;
		_cmd[6] = '\r';
		_cmd[7] = '\n';
	}else if(param_len == 2){
		_cmd[4] = ' ';
		_cmd[5] = ConvInt16ToChar16(param%0x100/0x10);
		_cmd[6] = ConvInt16ToChar16(param%0x10);
		_cmd[7] = '\r';
		_cmd[8] = '\n';
	}else if(param_len == 4){
		_cmd[4] = ' ';
		_cmd[5] = ConvInt16ToChar16(param%0x10000/0x1000);
		_cmd[6] = ConvInt16ToChar16(param%0x1000/0x100);
		_cmd[7] = ConvInt16ToChar16(param%0x100/0x10);
		_cmd[8] = ConvInt16ToChar16(param%0x10);
		_cmd[9] = '\r';
		_cmd[10] = '\n';
	}
	UartWriteMulti(_cmd,(param_len > 0?param_len + 7:6));

	if(!WaitResponce())return false;

	if(responceBuffer[0] == 'N' && responceBuffer[1] == 'G')return false;
	else return true;
}

bool IM920_Initialize(){
	SoftReset();
	uint16_t temp;
	if(ReadParam("RDID",&temp))setting.ID = temp;
	else return false;
	if(ReadParam("RDNN",&temp))setting.NN = temp;
	else return false;
	if(ReadParam("RDCH",&temp))setting.CH = temp;
	else return false;
	if(!SetParam("ECIO",0,0))
	return false;

	WaitMessage();
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
	if(CheckBusy())return false;

	uint8_t _len;
	for(_len = 0;_len<len&&_len<64&&data[len] != '\r'&&data[len] != '\n';_len++);
	UartWriteMulti((uint8_t*)"TXDA ",5);
	UartWriteMulti(data,_len);
	UartWriteMulti((uint8_t*)"\r\n",2);

	if(!WaitResponce())return false;

	if(responceBuffer[0] == 'N' && responceBuffer[1] == 'G')return false;
	return true;
}

int IM920_NewMessages(){
	return (int)newMessages;
}

void IM920_Read(char *str){
	if(newMessages > 0){
		for(int i = 0;;i++){
			str[i] = messageBuffer[messageBufferReadPosition];
			
			messageBufferReadPosition++;
			if(messageBufferReadPosition >= IM920_BUFFER_LEN)messageBufferReadPosition = 0;
			
			if(str[i] == '\n'){
				newMessages--;
				break;
			}
		}
	}
	return;
}