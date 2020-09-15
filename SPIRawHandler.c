#include "SPIRawHandler.h"

#ifdef STM32
#include "main.h"
extern SPI_HandleTypeDef hspi2;
#endif

//#define DMA_ENABLE
//#define DEBUG_PRINT

#ifdef DEBUG_PRINT
static int last_print = 0,r = 1,w = 2,rs = 3, ws = 4;
#endif

void SpiRawInitialize(){
	SpiRawDeAsertSS();
}

void SpiRawWrite(uint8_t data){
#ifdef DEBUG_PRINT
	if(last_print != w)printf("\nWRITE :");
	printf(" %x",data);
	last_print = w;
#endif

#ifdef STM32
#ifdef DMA_ENABLE
	HAL_SPI_Transmit_DMA(&hspi2, &data, 1);
	while(HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY);
#else
	HAL_SPI_Transmit(&hspi2, &data, 1, 100);
#endif
#endif
}
uint8_t SpiRawRead(){
	uint8_t data,dummy = 0xFF;

#ifdef STM32
#ifdef DMA_ENABLE
	HAL_SPI_TransmitReceive_DMA(&hspi2, &dummy, &data, 1);
	while(HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY);
#else
	HAL_SPI_TransmitReceive(&hspi2, &dummy, &data, 1, 100);
#endif
#endif

#ifdef DEBUG_PRINT
	if(last_print != r)printf("\nREAD :");
	printf(" %x",data);
	last_print = r;
#endif

	return data;
}
void SpiRawWriteMulti(uint8_t *data,uint16_t count){

#ifdef DEBUG_PRINT
	if(last_print != ws)printf("\nWRITES :");
	for(int i = 0;i < count;i++)printf(" %x",data[i]);
	last_print = ws;
#endif

#ifdef STM32
#ifdef DMA_ENABLE
	HAL_SPI_Transmit_DMA(&hspi2, data, count);
	while(HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY);
#else
	HAL_SPI_Transmit(&hspi2, data, count, 100);
#endif
#endif
}

void SpiRawReadMulti(uint8_t *data,uint16_t count){
#ifdef STM32
#ifdef DMA_ENABLE
	HAL_SPI_Receive_DMA(&hspi2, data, count);
	while(HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY);
#else
	HAL_SPI_Receive(&hspi2, data, count,100);
#endif
#endif

#ifdef DEBUG_PRINT
	if(last_print != rs)printf("\nREADS :");
	for(int i = 0;i < count;i++)printf(" %x",data[i]);
	last_print = rs;
#endif
}

void SpiRawAsertSS(){
	for(uint8_t i = 0;i < 1;)if(SpiRawRead() == 0xFF)i++;
#ifdef STM32
	HAL_GPIO_WritePin(SDSS_GPIO_Port,SDSS_Pin,0);
#endif

#ifdef DEBUG_PRINT
	printf("\n------ASSERT------");
#endif

	for(uint8_t i = 0;i < 1;)if(SpiRawRead() == 0xFF)i++;
}

void SpiRawDeAsertSS(){
	for(uint8_t i = 0;i < 1;)if(SpiRawRead() == 0xFF)i++;
#ifdef STM32
	HAL_GPIO_WritePin(SDSS_GPIO_Port,SDSS_Pin,1);
#endif

#ifdef DEBUG_PRINT
	printf("\n------DEASSERT------");
#endif

	for(uint8_t i = 0;i < 1;)if(SpiRawRead() == 0xFF)i++;
}
