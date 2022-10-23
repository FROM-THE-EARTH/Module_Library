#include "HSCDHD005A.h"
#include "I2C_Handler.h"

int HSCDHD005A::MAGNETIC::Who_I_Am() {
//  0:Not Detect
//  1:Detect
  uint8_t flag;
  I2C_Read(HSCDHD005A_MAG_SLAVE_ADDRESS, HSCDHD005A_MAG_WHO_I_AM, &flag, 1);
  return flag == 0x49;
}

int HSCDHD005A::MAGNETIC::Data_Ready() {
//  0:Not Detect
//  1:Detect
  uint8_t flag;
  I2C_Read(HSCDHD005A_MAG_SLAVE_ADDRESS, HSCDHD005A_MAG_STATUS, &flag, 1);
  return (flag >> 6) & 0b1;
}

int HSCDHD005A::MAGNETIC::Data_OverRun() {
//  0:Not Detect
//  1:Detect
  uint8_t flag;
  I2C_Read(HSCDHD005A_MAG_SLAVE_ADDRESS, HSCDHD005A_MAG_STATUS, &flag, 1);
  return (flag >> 5) & 0b1;
}

void HSCDHD005A::MAGNETIC::Power_Mode(int dat) {
//  0:Stand_by Mode
//  1:Active Mode
  uint8_t flag;
  I2C_Read(HSCDHD005A_MAG_SLAVE_ADDRESS, HSCDHD005A_MAG_CTRL1, &flag, 1);
  flag = flag & 0b01111111 | (dat << 7);
  I2C_Write(HSCDHD005A_MAG_SLAVE_ADDRESS, HSCDHD005A_MAG_CTRL1, &flag);
}

void HSCDHD005A::MAGNETIC::Output_Data_Rate(int dat) {
//  00:0.5Hz
//  01:10Hz
//  10:20Hz
//  11:100Hz
  uint8_t flag;
  I2C_Read(HSCDHD005A_MAG_SLAVE_ADDRESS, HSCDHD005A_MAG_CTRL1, &flag, 1);
  flag = flag & 0b11100111 | (dat << 3);
  I2C_Write(HSCDHD005A_MAG_SLAVE_ADDRESS, HSCDHD005A_MAG_CTRL1, &flag);
}

void HSCDHD005A::MAGNETIC::State_Control(int dat) {
//  0:Normal State
//  1:Force State
  uint8_t flag;
  I2C_Read(HSCDHD005A_MAG_SLAVE_ADDRESS, HSCDHD005A_MAG_CTRL1, &flag, 1);
  flag = flag & 0b11111101 | (dat << 1);
  I2C_Write(HSCDHD005A_MAG_SLAVE_ADDRESS, HSCDHD005A_MAG_CTRL1, &flag);
}

void HSCDHD005A::MAGNETIC::Data_Ready_Func_Ctrl_Enable(int dat) {
//  0:Disabled
//  1:Enabled
  uint8_t flag;
  I2C_Read(HSCDHD005A_MAG_SLAVE_ADDRESS, HSCDHD005A_MAG_CTRL2, &flag, 1);
  flag = flag & 0b11110111 | (dat << 3);
  I2C_Write(HSCDHD005A_MAG_SLAVE_ADDRESS, HSCDHD005A_MAG_CTRL2, &flag);
}

void HSCDHD005A::MAGNETIC::Data_Ready_Active_Level_Ctrl(int dat) {
//  0:ACTIVE LOW
//  1:ACTIVE HIGH
  uint8_t flag;
  I2C_Read(HSCDHD005A_MAG_SLAVE_ADDRESS, HSCDHD005A_MAG_CTRL2, &flag, 1);
  flag = flag & 0b11111011 | (dat << 2);
  I2C_Write(HSCDHD005A_MAG_SLAVE_ADDRESS, HSCDHD005A_MAG_CTRL2, &flag);
}

int HSCDHD005A::MAGNETIC::Soft_Reset_Ctrl_Enable(int dat) {
//  0:No Action
//  1:Soft reset
  uint8_t flag;
  I2C_Read(HSCDHD005A_MAG_SLAVE_ADDRESS, HSCDHD005A_MAG_CTRL3, &flag, 1);
  flag = flag & 0b01111111 | (dat << 7);
  I2C_Write(HSCDHD005A_MAG_SLAVE_ADDRESS, HSCDHD005A_MAG_CTRL3, &flag);
  I2C_Read(HSCDHD005A_MAG_SLAVE_ADDRESS, HSCDHD005A_MAG_CTRL3, &flag, 1);
  return (flag >> 7) & 0b1;
}

int HSCDHD005A::MAGNETIC::Measure_In_Force_Mode(int dat) {
//  0:No Action
//  1:Measurement Start
  uint8_t flag;
  I2C_Read(HSCDHD005A_MAG_SLAVE_ADDRESS, HSCDHD005A_MAG_CTRL3, &flag, 1);
  flag = flag & 0b10111111 | (dat << 6);
  I2C_Write(HSCDHD005A_MAG_SLAVE_ADDRESS, HSCDHD005A_MAG_CTRL3, &flag);
  I2C_Read(HSCDHD005A_MAG_SLAVE_ADDRESS, HSCDHD005A_MAG_CTRL3, &flag, 1);
  return (flag >> 6) & 0b1;
}

int HSCDHD005A::MAGNETIC::SelfTest_Ctrl_Enable(int dat) {
//  0:No Action
//  1:Set Parameters to Self Test Response register
  uint8_t flag;
  I2C_Read(HSCDHD005A_MAG_SLAVE_ADDRESS, HSCDHD005A_MAG_CTRL3, &flag, 1);
  flag = flag & 0b11101111 | (dat << 4);
  I2C_Write(HSCDHD005A_MAG_SLAVE_ADDRESS, HSCDHD005A_MAG_CTRL3, &flag);
  I2C_Read(HSCDHD005A_MAG_SLAVE_ADDRESS, HSCDHD005A_MAG_CTRL3, &flag, 1);
  return (flag >> 4) & 0b1;
}

void HSCDHD005A::MAGNETIC::Calibration(int dat) {
//  0:No Action
//  1:Action
  uint8_t flag;
  I2C_Read(HSCDHD005A_MAG_SLAVE_ADDRESS, HSCDHD005A_MAG_CTRL3, &flag, 1);
  flag = flag & 0b11111110 | dat;
  I2C_Write(HSCDHD005A_MAG_SLAVE_ADDRESS, HSCDHD005A_MAG_CTRL3, &flag);
}

void HSCDHD005A::MAGNETIC::Set_Output_Range(int dat) {
//  0:14 bit
//  1:15 bit
  uint8_t flag;
  I2C_Read(HSCDHD005A_MAG_SLAVE_ADDRESS, HSCDHD005A_MAG_CTRL3, &flag, 1);
  flag = flag & 0b11101111 | (dat << 4);
  I2C_Write(HSCDHD005A_MAG_SLAVE_ADDRESS, HSCDHD005A_MAG_CTRL4, &flag);
}

void HSCDHD005A::MAGNETIC::Get_Mag(int* mag_x, int* mag_y, int* mag_z) {
  uint8_t flag[6];
  I2C_Read(HSCDHD005A_MAG_SLAVE_ADDRESS, HSCDHD005A_MAG_OUTPUT_X_LSB, flag , 6);
  *mag_x = ((int)flag[1] & 0b01111111 << 8) | flag[0];
  *mag_y = ((int)flag[3] & 0b01111111 << 8) | flag[2];
  *mag_z = ((int)flag[5] & 0b01111111 << 8) | flag[4];
}

void HSCDHD005A::MAGNETIC::Read_Offset() {
  uint8_t flag[6];
  I2C_Read(HSCDHD005A_MAG_SLAVE_ADDRESS, HSCDHD005A_MAG_OFFSET_X_LSB, flag , 6);
  HSCDHD005A::MAGNETIC::offset[0] = ((int)flag[1] & 0b01111111 << 8) | flag[0];
  HSCDHD005A::MAGNETIC::offset[1] = ((int)flag[3] & 0b01111111 << 8) | flag[2];
  HSCDHD005A::MAGNETIC::offset[2] = ((int)flag[5] & 0b01111111 << 8) | flag[4];
}

int HSCDHD005A::MAGNETIC::Get_Temp() {
  uint8_t flag;
  I2C_Read(HSCDHD005A_MAG_SLAVE_ADDRESS, HSCDHD005A_MAG_TEMP_VALUE, &flag , 1);
  return flag;
}

//------------------------------------------------------------------------------

int HSCDHD005A::ACCELERATION::Read_TAP_Positive() {
  uint8_t flag;
  I2C_Read(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_STATUS, &flag , 1);
  return (flag & 0b11111110) | ((flag & 0b11111011) << 1) | ((flag & 0b11101111) << 2);
}

int HSCDHD005A::ACCELERATION::Read_TAP_Negative() {
  uint8_t flag;
  I2C_Read(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_STATUS, &flag , 1);
  return (flag & 0b11111101) | ((flag & 0b11110111) << 1) | ((flag & 0b11011111 << 2));
}

int HSCDHD005A::ACCELERATION::Is_Sample_Ready() {
  uint8_t flag;
  I2C_Read(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_STATUS, &flag , 1);
  return (flag >> 7) & 0b1; 
}

int HSCDHD005A::ACCELERATION::Device_Status() {
//  00:Device is in STANBY state, no sampling
//  01:Device is in WAKE state, sampling at set sample rate
//  10:Reserved
//  11:Reserved
  uint8_t flag;
  I2C_Read(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_OP_STATUS, &flag , 1);
  return flag & 0b11; 
}

int HSCDHD005A::ACCELERATION::I2C_Watchdog_Timeout() {
//  0:No watchdog event detected
//  1:Watchdog event has been detected by hardware, I2C slave machine reset to idel. This flag is cleasred by reading this register.
  uint8_t flag;
  I2C_Read(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_OP_STATUS, &flag , 1);
  return (flag >> 4) & 0b1; 
}

int HSCDHD005A::ACCELERATION::OTP_Activity_Status() {
//  0:Internal memory is idle and the device is ready for use
//  1:Internal memory is active and the device is no yet ready for use
  uint8_t flag;
  I2C_Read(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_OP_STATUS, &flag , 1);
  return (flag >> 7) & 0b1; 
}

void HSCDHD005A::ACCELERATION::Set_Positive_Tap_Int_Enable(int dat) {
//  dat:ZYX
//  0:disabled
//  1:Enabled. The corresponding TAP enable bit in register 0x009 must be enabled. The INTA pad will transition.
  uint8_t flag;
  I2C_Read(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_INT_ENABLE, &flag , 1);
  flag = (flag & 0b10101010) | ((dat & 0b100) << 2) | ((dat & 0b010) << 1) | (dat & 0b001);
  I2C_Write(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_INT_ENABLE, &flag);
}

void HSCDHD005A::ACCELERATION::Set_Negative_Tap_Int_Enable(int dat) {
//  dat:ZYX
//  0:disabled
//  1:Enabled. The corresponding TAP enable bit in register 0x009 must be enabled. The INTA pad will transition.
  uint8_t flag;
  I2C_Read(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_INT_ENABLE, &flag , 1);
  flag = (flag & 0b10010101) | ((dat & 0b100) << 3) | ((dat & 0b010) << 2) | ((dat & 0b001) << 1);
  I2C_Write(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_INT_ENABLE, &flag);
}

void HSCDHD005A::ACCELERATION::Generate_Int(int dat) {
//  dat:ZYX
//  0:disabled
//  1:Enabled. The corresponding TAP enable bit in register 0x009 must be enabled. The INTA pad will transition.
  uint8_t flag;
  I2C_Read(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_INT_ENABLE, &flag , 1);
  flag = (flag & ~0b01111111) | (dat << 7);
  I2C_Write(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_INT_ENABLE, &flag);
}

void HSCDHD005A::ACCELERATION::Set_Mode(int dat) {
//  00:STANBY state
//  01:WAKE state
//  10:Reserved
//  11:Reserved
  uint8_t flag;
  I2C_Read(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_MODE, &flag , 1);
  flag = (flag & 0b11111100) | dat;
  I2C_Write(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_MODE, &flag);
}

void HSCDHD005A::ACCELERATION::I2C_Watching_Timer_Negative(int dat) {
//  0:I2C watching timer for negative SCL stalls disabled
//  1:I2C watching timer for negative SCL stalls enabled
  uint8_t flag;
  I2C_Read(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_MODE, &flag , 1);
  flag = (flag & 0b11101111) | (dat << 4);
  I2C_Write(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_MODE, &flag);
}

void HSCDHD005A::ACCELERATION::I2C_Watching_Timer_Positive(int dat) {
//  0:I2C watching timer for positive SCL stalls disabled
//  1:I2C watching timer for positive SCL stalls enabled
  uint8_t flag;
  I2C_Read(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_MODE, &flag , 1);
  flag = (flag & 0b11011111) | (dat << 5);
  I2C_Write(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_MODE, &flag);
}

void HSCDHD005A::ACCELERATION::Int_Push_Pull(int dat) {
//  0:Interrupt pin INTA is open drain and requires an external pull-up to VDD;
//  1:Interrupt pin INTA is push-pull. No external pull-up resister should be ibstalled.
  uint8_t flag;
  I2C_Read(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_MODE, &flag , 1);
  flag = (flag & 0b10111111) | (dat << 6);
  I2C_Write(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_MODE, &flag);
}

void HSCDHD005A::ACCELERATION::Int_Pin_Level(int dat) {
//  0:Interrupt pin INTA is active low
//  1:Interrupt pin INTA is active high
  uint8_t flag;
  I2C_Read(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_MODE, &flag , 1);
  flag = (flag & 0b01111111) | (dat << 7);
  I2C_Write(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_MODE, &flag);
}



void HSCDHD005A::ACCELERATION::Set_Sampling_rate(int dat) {
//  0000: 32   Hz
//  0001: 16   Hz
//  0010:  8   Hz
//  0011:  4   Hz
//  0100:  2   Hz
//  0101:  1   Hz
//  0110:  0.5 Hz
//  0111:  0.25Hz
//  1000: 64   Hz
//  1001:128   Hz
//  1010:256   Hz
//  1011:Reserved
//  1100:Reserved
//  1101:Reserved
//  1110:Reserved
//  1111:Reserved
  uint8_t flag;
  I2C_Read(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_SAMPLE_RATE, &flag , 1);
  flag = (flag & ~0b00001111) | dat;
  I2C_Write(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_SAMPLE_RATE, &flag);
}

void HSCDHD005A::ACCELERATION::Positive_Positive_TAP_Switch(int dat) {
//  dat:ZYX
//  0:Positive and negative tap are not switched
//  1:Positive and negative tap are switched
  uint8_t flag;
  I2C_Read(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_SAMPLE_RATE, &flag , 1);
  flag = (flag & 0b10001111) | (dat << 4);
  I2C_Write(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_SAMPLE_RATE, &flag);
}

void HSCDHD005A::ACCELERATION::Set_Tap_Latch(int dat) {
//  dat:ZYX
//  0:
//  1:
  uint8_t flag;
  I2C_Read(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_SAMPLE_RATE, &flag , 1);
  flag = (flag & 0b01111111) | (dat << 7);
  I2C_Write(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_SAMPLE_RATE, &flag);
}

void HSCDHD005A::ACCELERATION::Positive_Positive_TAP_Enable(int dat) {
//  dat:ZYX
//  0:Disable positive tap detection
//  1:Eable   positive tap detection
  uint8_t flag;
  I2C_Read(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_TAP_CTRL, &flag , 1);
  flag = (flag & 0b11111000) | dat;
  I2C_Write(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_TAP_CTRL, &flag);
}

void HSCDHD005A::ACCELERATION::Positive_Negative_TAP_Enable(int dat) {
//  dat:ZYX
//  0:Disable negative tap detection
//  1:Eable   negative tap detection
  uint8_t flag;
  I2C_Read(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_TAP_CTRL, &flag , 1);
  flag = (flag & 0b11000111) | (dat << 3);
  I2C_Write(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_TAP_CTRL, &flag);
}

void HSCDHD005A::ACCELERATION::Select_Tap_Config(int dat) {
//  0:Register x0A, 0x0B, 0x0C point tap duaration and quiet period
//  1:Register x0A, 0x0B, 0x0C point tap threshold settings
  uint8_t flag;
  I2C_Read(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_TAP_CTRL, &flag , 1);
  flag = (flag & 0b10111111) | (dat << 6);
  I2C_Write(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_TAP_CTRL, &flag);
}

void HSCDHD005A::ACCELERATION::Tap_Detact_Enable(int dat) {
//  0:All tap detection is disabled, regarddless of bit[5:0]
//  1:Tap detection is enab;ed. individual enables control detection
  uint8_t flag;
  I2C_Read(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_TAP_CTRL, &flag , 1);
  flag = (flag & 0b01111111) | (dat << 7);
  I2C_Write(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_TAP_CTRL, &flag);
}

void HSCDHD005A::ACCELERATION::Set_Tap_Duration(int datx, int daty, int datz) {
//  0:All tap detection is disabled, regarddless of bit[5:0]
//  1:Tap detection is enab;ed. individual enables control detection
  uint8_t flag[3];
  I2C_Read(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_X_TUP_DURATION, flag , 3);
  flag[0] = (flag[0] & 0b11110000) | datx;
  flag[1] = (flag[1] & 0b11110000) | daty;
  flag[2] = (flag[2] & 0b11110000) | datz;
  I2C_Write(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_X_TUP_DURATION, flag, 3);
}

void HSCDHD005A::ACCELERATION::Set_Tap_Qualify(int datx, int daty, int datz) {
//  0:All tap detection is disabled, regarddless of bit[5:0]
//  1:Tap detection is enab;ed. individual enables control detection
  uint8_t flag[3];
  I2C_Read(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_X_TUP_DURATION, flag , 3);
  flag[0] = (flag[0] & 0b00001111) | (datx << 4);
  flag[1] = (flag[1] & 0b00001111) | (daty << 4);
  flag[2] = (flag[2] & 0b00001111) | (datz << 4);
  I2C_Write(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_X_TUP_DURATION, flag, 3);
}

void HSCDHD005A::ACCELERATION::Set_Tap_Threashold(int datx, int daty, int datz) {
//  0:All tap detection is disabled, regarddless of bit[5:0]
//  1:Tap detection is enab;ed. individual enables control detection
  uint8_t flag[3];
  I2C_Read(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_X_TUP_DURATION, flag , 3);
  flag[0] = datx;
  flag[1] = daty;
  flag[2] = datz;
  I2C_Write(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_X_TUP_DURATION, flag, 3);
}

void HSCDHD005A::ACCELERATION::Get_Acc(float* datx, float* daty, float* datz) {
  uint8_t flag[6];
  I2C_Read(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_X_L_Extend, flag , 6);
  *datx = ((int16_t)(((uint16_t)flag[1]) << 8) | flag[0]) / (float)(0b1111111111111111 >> (17 - HSCDHD005A::ACCELERATION::resolution)) * HSCDHD005A::ACCELERATION::range;
  *daty = ((int16_t)(((uint16_t)flag[3]) << 8) | flag[2]) / (float)(0b1111111111111111 >> (17 - HSCDHD005A::ACCELERATION::resolution)) * HSCDHD005A::ACCELERATION::range;
  *datz = ((int16_t)(((uint16_t)flag[5]) << 8) | flag[4]) / (float)(0b1111111111111111 >> (17 - HSCDHD005A::ACCELERATION::resolution)) * HSCDHD005A::ACCELERATION::range;
}

void HSCDHD005A::ACCELERATION::Set_Acc_Resolution(int dat) {
//  000:Select  6-bits for accelerometer measurements
//  001:Select  7-bits for accelerometer measurements
//  010:Select  8-bits for accelerometer measurements
//  011:Select 10-bits for accelerometer measurements
//  100:Select 12-bits for accelerometer measurements
//  101:Select 14-bits for accelerometer measurements
//  110:Reserved
//  111:Reserved
       if (dat==0b000) HSCDHD005A::ACCELERATION::resolution =  6;
  else if (dat==0b001) HSCDHD005A::ACCELERATION::resolution =  7;
  else if (dat==0b010) HSCDHD005A::ACCELERATION::resolution =  8;
  else if (dat==0b011) HSCDHD005A::ACCELERATION::resolution = 10;
  else if (dat==0b100) HSCDHD005A::ACCELERATION::resolution = 12;
  else if (dat==0b101) HSCDHD005A::ACCELERATION::resolution = 14;
  uint8_t flag;
  I2C_Read(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_OUTPUT_CONFIG, &flag , 1);
  flag = (flag & 0b11111000) | dat;
  I2C_Write(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_OUTPUT_CONFIG, &flag);
}

void HSCDHD005A::ACCELERATION::Set_Acc_Range(int dat) {
//  000:Select +/-  2g range
//  001:Select +/-  4g range
//  010:Select +/-  8g range
//  011:Select +/- 16g range
//  100:Select +/- 12g range
//  101:Reserved
//  110:Not Defined in DataSheet
//  111:Reserved
       if (dat==0b000) HSCDHD005A::ACCELERATION::range =  2*GRAVITY;
  else if (dat==0b001) HSCDHD005A::ACCELERATION::range =  4*GRAVITY;
  else if (dat==0b010) HSCDHD005A::ACCELERATION::range =  8*GRAVITY;
  else if (dat==0b011) HSCDHD005A::ACCELERATION::range = 16*GRAVITY;
  else if (dat==0b100) HSCDHD005A::ACCELERATION::range = 12*GRAVITY;
  uint8_t flag;
  I2C_Read(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_OUTPUT_CONFIG, &flag , 1);
  flag = (flag & 0b10001111) | (dat << 4);
  I2C_Write(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_OUTPUT_CONFIG, &flag);
}

void HSCDHD005A::ACCELERATION::Set_Offset(int datx, int daty, int datz) {
  uint8_t flag;
  I2C_Read(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_X_H_OFFSET, &flag , 1);
  flag = (flag & 0b10000000) | ((datx >> 8) & 0b01111111);
  I2C_Write(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_X_H_OFFSET, &flag);
  flag = (datx & 0b11111111);
  I2C_Write(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_X_L_OFFSET, &flag);
  
  I2C_Read(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_Y_H_OFFSET, &flag , 1);
  flag = (flag & 0b10000000) | ((daty >> 8) & 0b01111111);
  I2C_Write(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_Y_H_OFFSET, &flag);
  flag = daty & 0b11111111;
  I2C_Write(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_Y_L_OFFSET, &flag);

  I2C_Read(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_Z_H_OFFSET, &flag , 1);
  flag = (flag & 0b10000000) | ((datz >> 8) & 0b01111111);
  I2C_Write(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_Z_H_OFFSET, &flag);
  flag = datz & 0b11111111;
  I2C_Write(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_Z_L_OFFSET, &flag);
}

void HSCDHD005A::ACCELERATION::Set_Gain(int datx, int daty, int datz) {
  uint8_t flag;
  I2C_Read(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_X_H_OFFSET, &flag , 1);
  flag = (flag & 0b01111111) | ((datx >> 1) & 0b01111111);
  I2C_Write(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_X_H_OFFSET, &flag);
  flag = datx & 0b1111111011111111;
  I2C_Write(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_X_GAIN, &flag);
  
  I2C_Read(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_Y_H_OFFSET, &flag , 1);
  flag = (flag & 0b01111111) | ((daty >> 1) & 0b01111111);
  I2C_Write(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_Y_H_OFFSET, &flag);
  flag = daty & 0b1111111011111111;
  I2C_Write(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_Y_GAIN, &flag);

  I2C_Read(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_Z_H_OFFSET, &flag , 1);
  flag = (flag & 0b01111111) | ((datz >> 1) & 0b01111111);
  I2C_Write(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_Z_H_OFFSET, &flag);
  flag = datz & 0b1111111011111111;
  I2C_Write(HSCDHD005A_ACC_SLAVE_ADDRESS, HSCDHD005A_ACC_Z_GAIN, &flag);
}
