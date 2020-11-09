#include "I2C2Handler.h"

extern I2C_HandleTypeDef hi2c2;

static bool initialized = false;

void I2c2Initialize(){
	if(!initialized){
		initialized = true;
	}
}

void I2c2WriteByte(uint8_t add, uint8_t reg, uint8_t data)
{
	HAL_I2C_Mem_Write(&hi2c2, add << 1, reg, 1, &data, 1, 100);
}

uint8_t I2c2ReadByte(uint8_t add, uint8_t reg)
{
	uint8_t data;
	HAL_I2C_Mem_Read(&hi2c2, add << 1, reg, 1, &data, 1, 100);
	return data;
}

void I2c2ReadBytes(uint8_t add, uint8_t reg, uint8_t *data, uint8_t count)
{
	HAL_I2C_Mem_Read(&hi2c2, add << 1, reg, 1, data, count, 100);
	//HAL_I2C_Mem_Read_DMA(&hi2c1, add << 1, reg, 1, data, count);
	//HAL_I2C_Master_Transmit_DMA(&hi2c1, add << 1, &reg, 1);
	//HAL_I2C_Master_Receive_DMA(&hi2c1, add << 1, data, count);
	//HAL_I2C_Master_Transmit(&hi2c1, add << 1, &reg, 1, 100);
	//HAL_I2C_Master_Receive(&hi2c1, add << 1, data, count, 100);
}
