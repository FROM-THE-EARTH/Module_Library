#ifndef HSPPAD143A_H
#define HSPPAD143A_H

#define HSPPAD143A_SLAVE_ADDRESS     0x48//0x90

#define HSPPAD143A_WHO_I_AM          0x00
#define HSPPAD143A_INFO              0x01
#define HSPPAD143A_FIFO_STATUS       0x02
#define HSPPAD143A_STATUS            0x03
#define HSPPAD143A_PRESS_OUTPUT_L    0x04
#define HSPPAD143A_PRESS_OUTPUT_M    0x05
#define HSPPAD143A_PRESS_OUTPUT_H    0x06

#define HSPPAD143A_TEMP_OUTPUT_L     0x09
#define HSPPAD143A_TEMP_OUTPUT_H     0x0A

#define HSPPAD143A_CTRL_1            0x0E
#define HSPPAD143A_CTRL_2            0x0F
#define HSPPAD143A_ACT_CTRL_1        0x10
#define HSPPAD143A_ACT_CTRL_2        0x11
#define HSPPAD143A_FIFO_CTRL         0x12
#define HSPPAD143A_AVG_CTRL          0x13

#define HSPPAD143A_PRODUCT_NUM       0x1C

#define HSPPAD143A_PRESS_DETECT_CMD  0x22

#define HSPPAD143A_SOFT_RESET_CMD    0x26

#define HSPPAD143A_PT_DETECT_CMD     0x29

class HSPPAD143A {
  public:
  int Get_FIFO_Status();
  int Is_Busy();
  int Temp_Ready();
  int Is_OverRun();
  int Press_Ready();
  long get_Press();
  float get_Temp(); 
  void CIC_Filter(int);
  void Temp_Measure(int);
  void Press_Measure(int);
  void Sampling_rate(int);
  void Mode_Setteing(int);
  void Temp_measure_Reg(int);
  void Press_measure_Reg(int);
  void Software_Reset(int);
  void FIFO_Enable(int);
  void FIFO_threshold(int);
  void Frequency_Temp_Maeasure(int);
  void Ser_AVG_Num(int);
};

#endif
