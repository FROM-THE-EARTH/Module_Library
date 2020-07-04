#include "SPIRawHandler.h"


#define STM32

#ifdef STM32
#define DMA_ENABLE

#include "main.h"
extern SPI_HandleTypeDef hspi2;

void SpiRawInitialize(){
	SpiDeAsertSS();
}

void SpiRawWrite(uint8_t data){
#ifdef DMA_ENABLE
	HAL_SPI_Transmit_DMA(&hspi2, &data, 1);
	while(HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY);
#else
	HAL_SPI_Transmit(&hspi2, &data, 1, 100);
#endif
}
uint8_t SpiRawRead(){
	uint8_t data,dummy = 0xFF;
#ifdef DMA_ENABLE
	HAL_SPI_TransmitReceive_DMA(&hspi2, &dummy, &data, 1);
	while(HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY);
#else
	HAL_SPI_TransmitReceive(&hspi2, &dummy, &data, 1, 100);
#endif
	return data;
}
void SpiRawWrites(uint8_t *data,uint16_t count){
#ifdef DMA_ENABLE
	HAL_SPI_Transmit_DMA(&hspi2, data, count);
	while(HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY);
#else
	HAL_SPI_Transmit(&hspi2, data, count, 100);
#endif
}
void SpiRawReads(uint8_t *data,uint16_t count){
#ifdef DMA_ENABLE
	HAL_SPI_Receive_DMA(&hspi2, data, count);
	while(HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY);
#else
	HAL_SPI_Receive(&hspi2, data, count,100);
#endif
}

void SpiAsertSS(){
	for(uint8_t i = 0;i < 1;)if(SpiRawRead() == 0xFF)i++;
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,0);
	for(uint8_t i = 0;i < 1;)if(SpiRawRead() == 0xFF)i++;
}

void SpiDeAsertSS(){
	for(uint8_t i = 0;i < 1;)if(SpiRawRead() == 0xFF)i++;
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,1);
	for(uint8_t i = 0;i < 1;)if(SpiRawRead() == 0xFF)i++;
}

#endif
