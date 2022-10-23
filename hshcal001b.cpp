#include "HSHCAL001B.h"
#include "I2C_Handler.h"

float HSHCAL001B::Get_Hum() {
  uint8_t flag[2];
  I2C_Read(HSHCAL001B_SLAVE_ADDRESS, HSHCAL001B_HUMI_OUTPUT1, flag, 2);
  
  unsigned int HUMI = (((int)flag[1] << 8) | (int) flag[0]) & 0b11111111111111;
//  without re-hydration
  float RH = (HUMI -  896)/64.0;
//  with re-hydration
//  float RH = (HUMI - 1280)/64.0;
  return RH;
}

float HSHCAL001B::Get_Temp() {
  uint8_t flag[2];
  I2C_Read(HSHCAL001B_SLAVE_ADDRESS, HSHCAL001B_TEMP_OUTPUT1, flag, 2);
  
  unsigned int TEMP = (((int)flag[1] << 8) | (int) flag[0]) & 0b1111111111111;
  return (TEMP - 2096)/50.0;
}

int HSHCAL001B::Is_active() {
//  0:Inactive
//  1:Active
  uint8_t flag;
  I2C_Read(HSHCAL001B_SLAVE_ADDRESS, HSHCAL001B_STATUS, &flag, 1);
  return (flag >> 7) & 0b1;
}

int HSHCAL001B::Is_Hum_Measure_End() {
//  0:Inactive
//  1:Active
  uint8_t flag;
  I2C_Read(HSHCAL001B_SLAVE_ADDRESS, HSHCAL001B_STATUS, &flag, 1);
  return (flag >> 6) & 0b1;
}
 
int HSHCAL001B::Is_OverRun() {
//  0:No overrun
//  1:Detect overrun
  uint8_t flag;
  I2C_Read(HSHCAL001B_SLAVE_ADDRESS, HSHCAL001B_STATUS, &flag, 1);
  return (flag >> 5) & 0b1;
}
 
int HSHCAL001B::Measure_Exec() {
//  0:Sleep
//  1:Execute Measurement
  uint8_t flag;
  I2C_Read(HSHCAL001B_SLAVE_ADDRESS, HSHCAL001B_STATUS, &flag, 1);
  return (flag >> 4) & 0b1;
}

int HSHCAL001B::Is_Temp_Measure_End() {
//  0:Inactive
//  1:Active
  uint8_t flag;
  I2C_Read(HSHCAL001B_SLAVE_ADDRESS, HSHCAL001B_STATUS, &flag, 1);
  return (flag >> 2) & 0b1;
}

void HSHCAL001B::AD_Trans_Filter(int dat) {
//  00 : Ultra low power
//  01 : Low power
//  10 : Middle power / middle accuracy
//  11 : High accuracy
  uint8_t flag;
  I2C_Read(HSHCAL001B_SLAVE_ADDRESS, HSHCAL001B_CTRL1, &flag, 1);
  flag = flag & ~(0b11 << 5) | dat;
  I2C_Write(HSHCAL001B_SLAVE_ADDRESS, HSHCAL001B_CTRL1, &flag);
}

void HSHCAL001B::Output_Data_Rate(int dat) {
//  00 : 1Hz (1sec)
//  01 : 5Hz (200msec)
//  10 : 10Hz (100msec)
//  11 : 50Hz (20msec)
  uint8_t flag;
  I2C_Read(HSHCAL001B_SLAVE_ADDRESS, HSHCAL001B_CTRL1, &flag, 1);
  flag = flag & ~(0b11 << 3) | dat;
  I2C_Write(HSHCAL001B_SLAVE_ADDRESS, HSHCAL001B_CTRL1, &flag);
}

void HSHCAL001B::Power_Saving(int dat) {
//  0 : invalid
//  1 : valid
  uint8_t flag;
  I2C_Read(HSHCAL001B_SLAVE_ADDRESS, HSHCAL001B_CTRL1, &flag, 1);
  flag = flag & ~(1 << 2) | dat;
  I2C_Write(HSHCAL001B_SLAVE_ADDRESS, HSHCAL001B_CTRL1, &flag);
}

void HSHCAL001B::Control_State(int dat) {
//  0 : Nomal state
//  1 : Force state
  uint8_t flag;
  I2C_Read(HSHCAL001B_SLAVE_ADDRESS, HSHCAL001B_CTRL1, &flag, 1);
  flag = flag & ~(1 << 1) | dat;
  I2C_Write(HSHCAL001B_SLAVE_ADDRESS, HSHCAL001B_CTRL1, &flag);
}

void HSHCAL001B::Software_Reset() {
  Action_CMD(HSHCAL001B_SLAVE_ADDRESS, HSHCAL001B_RESET_CMD);
}

void HSHCAL001B::Sensor_Detect_CMD() {
  Action_CMD(HSHCAL001B_SLAVE_ADDRESS, HSHCAL001B_SENS_DETECT_CMD);
}

int HSHCAL001B::Self_Test_CMD() {
  uint8_t flag_1, flag_2, flag_3;
  I2C_Read(HSHCAL001B_SLAVE_ADDRESS, HSHCAL001B_SELF_TEST_RES, &flag_1, 1);
  Action_CMD(HSHCAL001B_SLAVE_ADDRESS, HSHCAL001B_SELF_TEST_CMD);
  I2C_Read(HSHCAL001B_SLAVE_ADDRESS, HSHCAL001B_SELF_TEST_RES, &flag_2, 1);
  I2C_Read(HSHCAL001B_SLAVE_ADDRESS, HSHCAL001B_SELF_TEST_RES, &flag_3, 1);
  return (flag_1 == 0x55) && (flag_2 == 0xAA) && (flag_2 == 0x55);
}
