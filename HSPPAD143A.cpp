#include "HSPPAD143A.h"
#include "I2C_Handler.h"

int HSPPAD143A::Get_FIFO_Status() {
  uint8_t fifo_data;
  I2C_Read(HSPPAD143A_SLAVE_ADDRESS, HSPPAD143A_FIFO_STATUS, &fifo_data, 1);
  return fifo_data & (1<<7) ? -1 : fifo_data & 0b11111;
}

int HSPPAD143A::Is_Busy() {
  uint8_t flag;
  I2C_Read(HSPPAD143A_SLAVE_ADDRESS, HSPPAD143A_STATUS, &flag, 1);
  return flag & (1<<7);
}

int HSPPAD143A::Temp_Ready() {
  uint8_t flag;
  I2C_Read(HSPPAD143A_SLAVE_ADDRESS, HSPPAD143A_STATUS, &flag, 1);
  return flag & (1<<4);
}

int HSPPAD143A::Is_OverRun() {
  uint8_t flag;
  I2C_Read(HSPPAD143A_SLAVE_ADDRESS, HSPPAD143A_STATUS, &flag, 1);
  return flag & (1<<2);
}

int HSPPAD143A::Press_Ready() {
  uint8_t flag;
  I2C_Read(HSPPAD143A_SLAVE_ADDRESS, HSPPAD143A_STATUS, &flag, 1);
  return flag & 1;
}

long HSPPAD143A::get_Press() {
  uint8_t press_data[3];
  I2C_Read(HSPPAD143A_SLAVE_ADDRESS, HSPPAD143A_PRESS_OUTPUT_L, press_data, 3);
  return (((long)(press_data[2] & 1) << 16) | ((long) press_data[1] << 8) |  press_data[0])*2;
}


float HSPPAD143A::get_Temp() {
  uint8_t temp_data[2];
  I2C_Read(HSPPAD143A_SLAVE_ADDRESS, HSPPAD143A_TEMP_OUTPUT_L, temp_data, 2);
  return (((int) temp_data[1] << 8) | temp_data[0])/256.0;
}

void HSPPAD143A::CIC_Filter(int dat) {
// 00 Ultra Low Power
// 01 Low Power
// 10 High Accuracy
// 11 Ultra Accuracy
  uint8_t flag;
  I2C_Read (HSPPAD143A_SLAVE_ADDRESS, HSPPAD143A_CTRL_1, &flag, 1);
  flag = flag & 0b11111100 | dat;
  I2C_Write(HSPPAD143A_SLAVE_ADDRESS, HSPPAD143A_CTRL_1, &flag);
}


void HSPPAD143A::Temp_Measure(int dat) {
// 0: Disable temperature measurement in Continuous mode.
// 1: Eable temperature measurement in Continuous mode.
  uint8_t flag;
  I2C_Read (HSPPAD143A_SLAVE_ADDRESS, HSPPAD143A_CTRL_1, &flag, 1);
  flag = flag & 0b01111111 | (dat << 7);
  I2C_Write(HSPPAD143A_SLAVE_ADDRESS, HSPPAD143A_CTRL_1, &flag);
}

void HSPPAD143A::Press_Measure(int dat) {
// 0: Disable temperature measurement in Continuous mode.
// 1:   Eable temperature measurement in Continuous mode.
  uint8_t flag;
  I2C_Read (HSPPAD143A_SLAVE_ADDRESS, HSPPAD143A_CTRL_2, &flag, 1);
  flag = flag & 0b11011111 | (dat << 5);
  I2C_Write(HSPPAD143A_SLAVE_ADDRESS, HSPPAD143A_CTRL_2, &flag);
}


void HSPPAD143A::Sampling_rate(int dat) {
// 00: 1Hz
// 01: 10Hz
// 10: 100Hz
// 11: 200Hz
  uint8_t flag;
  I2C_Read (HSPPAD143A_SLAVE_ADDRESS, HSPPAD143A_CTRL_2, &flag, 1);
  flag = flag & 0b11110011 | (dat << 2);
  I2C_Write(HSPPAD143A_SLAVE_ADDRESS, HSPPAD143A_CTRL_2, &flag);
}

void HSPPAD143A::Mode_Setteing(int dat) {
//00: Register Action Mode
//01: Continuous Measurement Mode
//10: Command Action Mode
//11: Command Action Mode
  uint8_t flag;
  I2C_Read (HSPPAD143A_SLAVE_ADDRESS, HSPPAD143A_CTRL_2, &flag, 1);
  flag = flag & 0b11111100 | dat;
  I2C_Write(HSPPAD143A_SLAVE_ADDRESS, HSPPAD143A_CTRL_2, &flag);
}

void HSPPAD143A::Temp_measure_Reg(int dat) {
//0: Not measure
//1: Measure
  uint8_t flag;
  I2C_Read (HSPPAD143A_SLAVE_ADDRESS, HSPPAD143A_ACT_CTRL_1, &flag, 1);
  flag = flag & 0b11110111 | (dat << 3);
  I2C_Write(HSPPAD143A_SLAVE_ADDRESS, HSPPAD143A_ACT_CTRL_1, &flag);
}

void HSPPAD143A::Press_measure_Reg(int dat) {
//0: Not measure
//1: Measure
  uint8_t flag;
  I2C_Read (HSPPAD143A_SLAVE_ADDRESS, HSPPAD143A_ACT_CTRL_1, &flag, 1);
  flag = flag & 0b11111101 | (dat << 1);
  I2C_Write(HSPPAD143A_SLAVE_ADDRESS, HSPPAD143A_ACT_CTRL_1, &flag);
}

void HSPPAD143A::Software_Reset(int dat) {
//0: None
//1: Execute Software Reset
  uint8_t flag;
  I2C_Read (HSPPAD143A_SLAVE_ADDRESS, HSPPAD143A_ACT_CTRL_2, &flag, 1);
  flag = flag & 0b01111111 | (dat << 7);
  I2C_Write(HSPPAD143A_SLAVE_ADDRESS, HSPPAD143A_ACT_CTRL_2, &flag);
}

void HSPPAD143A::FIFO_Enable(int dat) {
//0: Disable
//1: Enable
  uint8_t flag;
  I2C_Read (HSPPAD143A_SLAVE_ADDRESS, HSPPAD143A_FIFO_CTRL, &flag, 1);
  flag = flag & 0b01111111 | (dat << 7);
  I2C_Write(HSPPAD143A_SLAVE_ADDRESS, HSPPAD143A_FIFO_CTRL, &flag);
}

void HSPPAD143A::FIFO_threshold(int dat) {
  uint8_t flag;
  I2C_Read (HSPPAD143A_SLAVE_ADDRESS, HSPPAD143A_FIFO_CTRL, &flag, 1);
  flag = flag & 0b11100000 | dat;
  I2C_Write(HSPPAD143A_SLAVE_ADDRESS, HSPPAD143A_FIFO_CTRL, &flag);
}

void HSPPAD143A::Frequency_Temp_Maeasure(int dat) {
  uint8_t flag;
  I2C_Read (HSPPAD143A_SLAVE_ADDRESS, HSPPAD143A_AVG_CTRL, &flag, 1);
  flag = flag & 0b11000111 | (dat << 3);
  I2C_Write(HSPPAD143A_SLAVE_ADDRESS, HSPPAD143A_AVG_CTRL, &flag);
}

void HSPPAD143A::Ser_AVG_Num(int dat) {
  uint8_t flag;
  I2C_Read (HSPPAD143A_SLAVE_ADDRESS, HSPPAD143A_AVG_CTRL, &flag, 1);
  flag = flag & 0b11111000 | dat;
  I2C_Write(HSPPAD143A_SLAVE_ADDRESS, HSPPAD143A_AVG_CTRL, &flag);
}
