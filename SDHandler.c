/*
 * SD.c
 *
 *  Created on: Jun 12, 2020
 *      Author: oku_d
 */

#include "SDHandler.h"

#ifndef NULL
#define NULL ((void*)0)
#endif

SD_Card sd;

const uint64_t CRC7 = 0b10001001;

static uint8_t waitCmdResponce(){
	uint8_t rsp = 0xFF;
	for(uint8_t i = 0;i < 8;i++){
		rsp = SpiRawRead();
		if(rsp != 0xFF)break;
	}
	return rsp;
}
static void waitBusyState(){
	for(uint8_t i = 0;i < 0xF;)
		if(SpiRawRead() == 0xFF)i++;
	return;
}
static uint8_t waitToken(){
	uint8_t token;
	for(uint16_t i = 0;i < 0xFF;i++){
		token = SpiRawRead();
		if(token == 0xFE || (token & 0b11110000) == 0)break;
	}
	return token;
}
static void sendToken(uint8_t token){
	SpiRawWrite(token);
	return;
}
static uint8_t waitDataResponce(){
	uint8_t datrsp;
	for(uint16_t i = 0;i < 0x2FF;i++){
		datrsp = SpiRawRead() & 0b11111;
		if((datrsp & 0b10001) == 0b000001){
			return datrsp;
		}
	}
	return 0xFF;
}
static bool sendDataPacket(uint8_t *data){
	sendToken(0xFC);

	SpiRawWrites(data, 512);

	uint8_t rsp = waitDataResponce();
	waitBusyState();
	return (rsp == 0b00101);
}
static bool waitDataPacket(uint8_t *data){
	if(waitToken() == 0xFE){
		SpiRawReads(data, 512);
		SpiRawRead();
		SpiRawRead();
		return true;
	}else{
		return false;
	}
}
static uint8_t CRCcal(uint64_t cmdarg){
	uint64_t t = cmdarg << 7;
	for(uint8_t i = 46; i >= 7; i-- ){
		if(t >> i == 1)t ^= CRC7 << (i - 7);
	}
	return t & 0xFF;
}

static uint8_t CMD(uint8_t cmd, uint32_t arg, uint8_t *subrsp){

	SpiRawWrite(0b01000000|cmd);

	SpiRawWrite(((uint8_t*)(&arg))[3]);
	SpiRawWrite(((uint8_t*)(&arg))[2]);
	SpiRawWrite(((uint8_t*)(&arg))[1]);
	SpiRawWrite(((uint8_t*)(&arg))[0]);

	if(!sd.initialized)
		SpiRawWrite((CRCcal( ((uint64_t)(0b01000000|cmd) << 32) | arg ) << 1) | 1);
	else
		SpiRawWrite(0xFF);

	uint8_t rsp;

	switch(cmd){
	case 12:
		SpiRawRead();
		rsp = waitCmdResponce();
	case 8:
		rsp = waitCmdResponce();
		for(uint8_t i = 0;i < 4;i++)subrsp[i] = SpiRawRead();
		break;
	case 9:
	case 10:
		rsp = waitCmdResponce();
		if(waitToken() == 0xFE)
			for(uint8_t i = 0;i < 16;i++)subrsp[i] = SpiRawRead();
		break;
	case 13:
		rsp = waitCmdResponce();
		*subrsp = SpiRawRead();
		break;
	case 58:
		rsp = waitCmdResponce();
		for(uint8_t i = 0;i < 4;i++)subrsp[i] = SpiRawRead();
		break;
	default:
		rsp = waitCmdResponce();
		break;
	}

	return rsp;
}

static uint8_t ACMD(uint8_t cmd, uint32_t arg, uint8_t *subrsp){
	uint8_t res;

	SpiAsertSS();
	res = CMD(55,0,NULL);
	SpiDeAsertSS();

	SpiAsertSS();
	res = CMD(cmd,arg,subrsp);
	SpiDeAsertSS();

	return res;
}

bool SDInitialize(){

	uint8_t rsp;

	sd.initialized = false;
	sd.is_SDHC = false;
	sd.sectors = 0;

	SpiInitialize();

	//--------------------------- 74clock ---------------------------
	SpiRawWrite(0xFF);
	SpiRawWrite(0xFF);
	SpiRawWrite(0xFF);
	SpiRawWrite(0xFF);
	SpiRawWrite(0xFF);
	SpiRawWrite(0xFF);
	SpiRawWrite(0xFF);
	SpiRawWrite(0xFF);
	SpiRawWrite(0xFF);
	SpiRawWrite(0xFF);

	//--------------------------- CMD0 ---------------------------
	SpiAsertSS();
	rsp = CMD(0,0,NULL);
	SpiDeAsertSS();
	if(rsp != 0x01)return false;
	//--------------------------- CMD8 ---------------------------
	SpiAsertSS();
	uint8_t r7[4];
	rsp = CMD(8,0x1AA,r7);
	uint32_t R7 = ( r7[0] << 24 | r7[1] << 16 | r7[2] << 8 | r7[3] );
	SpiDeAsertSS();
	if(rsp != 0x01 || (R7 & 0xFFF) != 0x1AA)return false;
	//--------------------------- ACMD41 ---------------------------
	for(uint8_t i = 0; i < 0xFF; i++){
		SpiAsertSS();
		rsp = ACMD(41,1<<30,NULL);
		SpiDeAsertSS();
		if(rsp != 0x01)break;
	}

	if(rsp == 0x00)sd.initialized = true;
	else return false;

	//--------------------------- CMD58 ---------------------------
	SpiAsertSS();
	uint8_t ocr[4];
	rsp = CMD(58,0,ocr);
	SpiDeAsertSS();
	uint32_t OCR = ( ocr[0] << 24 | ocr[1] << 16 | ocr[2] << 8 | ocr[3] );
	if(((OCR >> 30) & 0x01) == 1)sd.is_SDHC = true;
	else return false;

	//--------------------------- CMD9 ---------------------------
	SpiAsertSS();
	uint8_t CSD[16];
	rsp = CMD(9,0,CSD);
	SpiDeAsertSS();
	if(rsp == 0x00){
		uint32_t C_SIZE = (((CSD[7] & 0b111111) << 16) | (CSD[8] << 8) | CSD[9]);
		sd.sectors = 512 * C_SIZE;
	}
	//------------------------------------------------------------

	return true;
}

//----------------------------------------------------------------------------------------------------------------	未完成!!!
bool SDWrites(uint32_t sector, uint8_t *data, uint16_t count){
	uint8_t rsp;

	SpiAsertSS();

	rsp = CMD(25,sector,NULL);
	if(rsp == 0x00){
		for(uint32_t i = 0;i < count;i++){
			sendDataPacket(data + i * 512);
		}

		sendToken(0xFD);
		rsp = waitDataResponce();
		waitBusyState();
		if(rsp == 0xE5)rsp = 0x00;

	}

	SpiDeAsertSS();
	return (rsp == 0x00);
}

//---------------------------------------------------------------------------------------------------------------- 未完成!!!
bool SDReads(uint32_t sector, uint8_t *data, uint16_t count){
	bool res = true;
	uint8_t rsp;

	SpiAsertSS();
	rsp = CMD(18,sector,NULL);

	SpiDeAsertSS();
	SpiAsertSS();
	if(rsp == 0x00){
		/*
		for(uint16_t i = 0;i < count;i++){
			waitDataPacket(&data[i * 512]);
		}

		waitToken();

		rsp = CMD(12,0,NULL);
		if(rsp != 0x00){
			res = false;
		}
		waitBusyState();*/

		for(uint16_t i = 0;;i++){
			if(waitToken() == 0xFE){
				SpiRawReads(&data[512 * i],512);
			}

			if(i == count - 1){
				CMD(12,0,NULL);
				break;
			}
			else{
				SpiRawRead();
				SpiRawRead();
			}
		}
	}else res = false;

	SpiDeAsertSS();
	return res;
}

//----------------------------------------------------------------------------------------------------------------
bool SDWrite(uint32_t sector, uint8_t *data){
	bool res = true;
	uint8_t rsp;

	SpiAsertSS();

	rsp = CMD(24,sector,NULL);

	if(rsp == 0x00){

		sendToken(0xFE);

		SpiRawWrites(data,512);

		SpiRawWrite(0xFF);
		SpiRawWrite(0xFF);

		res = (waitDataResponce() == 0b00101);

		waitBusyState();

	}else res = false;

	SpiDeAsertSS();

	return res;
}

//----------------------------------------------------------------------------------------------------------------
bool SDRead(uint32_t sector, uint8_t *data){
	bool res = true;
	uint8_t rsp;

	SpiAsertSS();

	rsp = CMD(17,sector,NULL);

	if(rsp == 0x00){
		if(waitDataPacket(data)){
			res = true;
		}else res = false;
	}else res = false;

	SpiDeAsertSS();
	return res;
}
