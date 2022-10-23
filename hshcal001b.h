#ifndef HSHCAL001B_H
#define HSHCAL001B_H


#define HSHCAL001B_SLAVE_ADDRESS  0x18

#define HSHCAL001B_SELF_TEST_RES   0x0C
#define HSHCAL001B_MORE_INFO_VER   0x0D
#define HSHCAL001B_MORE_INFO_ALPS  0x0E
#define HSHCAL001B_WHO_I_AM        0x0F

#define HSHCAL001B_HUMI_OUTPUT1    0x10
#define HSHCAL001B_HUMI_OUTPUT2    0x11
#define HSHCAL001B_TEMP_OUTPUT1    0x12
#define HSHCAL001B_TEMP_OUTPUT2    0x13

#define HSHCAL001B_STATUS          0x18

#define HSHCAL001B_CTRL1           0x1B

#define HSHCAL001B_RESERVED_DATA1  0x20
#define HSHCAL001B_RESERVED_DATA2  0x21
#define HSHCAL001B_RESERVED_DATA3  0x22
#define HSHCAL001B_RESERVED_DATA4  0x23

#define HSHCAL001B_RESET_CMD       0x30
#define HSHCAL001B_SENS_DETECT_CMD 0x31
#define HSHCAL001B_SELF_TEST_CMD   0x32

class HSHCAL001B {
  public:
  float Get_Hum();
  float Get_Temp();
  int Is_active();
  int Is_Hum_Measure_End();
  int Is_OverRun();
  int Measure_Exec();
  int Is_Temp_Measure_End();
  void AD_Trans_Filter(int);
  void Output_Data_Rate(int);
  void Power_Saving(int);
  void Control_State(int);
  void Software_Reset();
  void Sensor_Detect_CMD();
  int Self_Test_CMD();
};

#endif
