#ifndef HSCDHD005A_H
#define HSCDHD005A_H

#define GRAVITY 9.80665

#define HSCDHD005A_MAG_SLAVE_ADDRESS  0x0C

#define HSCDHD005A_MAG_SELF_TEST_RES  0x0C

#define HSCDHD005A_MAG_MORE_INFO_VER  0x0D
#define HSCDHD005A_MAG_MORE_INFO_ALPS 0x0E
#define HSCDHD005A_MAG_WHO_I_AM       0x0F

#define HSCDHD005A_MAG_OUTPUT_X_LSB   0x10
#define HSCDHD005A_MAG_OUTPUT_X_MSB   0x11
#define HSCDHD005A_MAG_OUTPUT_Y_LSB   0x12
#define HSCDHD005A_MAG_OUTPUT_Y_MSB   0x13
#define HSCDHD005A_MAG_OUTPUT_Z_LSB   0x14
#define HSCDHD005A_MAG_OUTPUT_Z_MSB   0x15

#define HSCDHD005A_MAG_STATUS         0x18

#define HSCDHD005A_MAG_CTRL1          0x1B
#define HSCDHD005A_MAG_CTRL2          0x1C
#define HSCDHD005A_MAG_CTRL3          0x1D
#define HSCDHD005A_MAG_CTRL4          0x1E

#define HSCDHD005A_MAG_OFFSET_X_LSB   0x20
#define HSCDHD005A_MAG_OFFSET_X_MSB   0x21
#define HSCDHD005A_MAG_OFFSET_Y_LSB   0x22
#define HSCDHD005A_MAG_OFFSET_Y_MSB   0x23
#define HSCDHD005A_MAG_OFFSET_Z_LSB   0x24
#define HSCDHD005A_MAG_OFFSET_Z_MSB   0x25

#define HSCDHD005A_MAG_TEMP_VALUE     0x31

//----------------------------------------
// AS level GND
#define HSCDHD005A_ACC_SLAVE_ADDRESS  0x4C

// AS level VDD
//#define HSCDHD005A_ACC_SLAVE_ADDRESS  0x6C

#define HSCDHD005A_ACC_STATUS         0x03
#define HSCDHD005A_ACC_OP_STATUS      0x04

#define HSCDHD005A_ACC_INT_ENABLE     0x06
#define HSCDHD005A_ACC_MODE           0x07
#define HSCDHD005A_ACC_SAMPLE_RATE    0x08
#define HSCDHD005A_ACC_TAP_CTRL       0x09
#define HSCDHD005A_ACC_X_TUP_DURATION 0x0A
#define HSCDHD005A_ACC_Y_TUP_DURATION 0x0B
#define HSCDHD005A_ACC_Z_TUP_DURATION 0x0C
#define HSCDHD005A_ACC_X_L_Extend     0x0D
#define HSCDHD005A_ACC_X_H_Extend     0x0E
#define HSCDHD005A_ACC_Y_L_Extend     0x0F
#define HSCDHD005A_ACC_Y_H_Extend     0x10
#define HSCDHD005A_ACC_Z_L_Extend     0x11
#define HSCDHD005A_ACC_Z_H_Extend     0x12

#define HSCDHD005A_ACC_OUTPUT_CONFIG  0x20
#define HSCDHD005A_ACC_X_L_OFFSET     0x21
#define HSCDHD005A_ACC_X_H_OFFSET     0x22
#define HSCDHD005A_ACC_Y_L_OFFSET     0x23
#define HSCDHD005A_ACC_Y_H_OFFSET     0x24
#define HSCDHD005A_ACC_Z_L_OFFSET     0x25
#define HSCDHD005A_ACC_Z_H_OFFSET     0x26
#define HSCDHD005A_ACC_X_GAIN         0x27
#define HSCDHD005A_ACC_Y_GAIN         0x28
#define HSCDHD005A_ACC_Z_GAIN         0x29



class HSCDHD005A {
  public:
  class MAGNETIC {
    public:
    int offset[3] = {};
    int Who_I_Am();
    int Data_Ready();
    int Data_OverRun();
    void Power_Mode(int dat);
    void Output_Data_Rate(int dat);
    void State_Control(int dat);
    void Data_Ready_Func_Ctrl_Enable(int dat);
    void Data_Ready_Active_Level_Ctrl(int dat);
    int Soft_Reset_Ctrl_Enable(int dat);
    int Measure_In_Force_Mode(int dat);
    int SelfTest_Ctrl_Enable(int dat);
    void Calibration(int dat);
    void Set_Output_Range(int dat);
    void Get_Mag(int* mag_x, int* mag_y, int* mag_z);
    void Read_Offset();
    int Get_Temp();
  };
  class ACCELERATION {
    public:
    float range = 2 * 9.8;
    int resolution = 6;
    int Read_TAP_Positive();
    int Read_TAP_Negative();
    int Is_Sample_Ready();
    int Device_Status();
    int I2C_Watchdog_Timeout();
    int OTP_Activity_Status();
    void Set_Positive_Tap_Int_Enable(int dat);
    void Set_Negative_Tap_Int_Enable(int dat);
    void Generate_Int(int dat);
    void Set_Mode(int dat);
    void I2C_Watching_Timer_Negative(int dat);
    void I2C_Watching_Timer_Positive(int dat);
    void Int_Push_Pull(int dat);
    void Int_Pin_Level(int dat);
    void Set_Sampling_rate(int dat);
    void Set_Tap_Latch(int dat);
    void Positive_Positive_TAP_Switch(int dat);
    void Positive_Negative_TAP_Switch(int dat);;
    void Positive_Positive_TAP_Enable(int dat);
    void Positive_Negative_TAP_Enable(int dat);
    void Select_Tap_Config(int dat);
    void Tap_Detact_Enable(int dat);
    void Set_Tap_Duration(int datx, int daty, int datz);
    void Set_Tap_Qualify(int datx, int daty, int datz);
    void Set_Tap_Threashold(int datx, int day, int datz);
    void Get_Acc(float* datx, float* day, float* datz);
    void Set_Acc_Resolution(int dat);
    void Set_Acc_Range(int dat);
    void Set_Offset(int datx, int daty, int datz);
    void Set_Gain(int datx, int daty, int datz);
  };
  MAGNETIC MAG;
  ACCELERATION ACC;
};

#endif
