#include "I2C_Handler.h"
#include <Wire.h>

void I2C_Write(uint8_t address, uint8_t reg, uint8_t* dat) {
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.write(*dat);
  Wire.endTransmission();
}

void I2C_Write(uint8_t address, uint8_t reg, uint8_t* dat, unsigned int num) {
  Wire.beginTransmission(address);
  Wire.write(reg);
  for (int i = 0; i < num; i++) {
    Wire.write(*dat);
    dat++;
  }
  
  Wire.endTransmission();
}

void I2C_Read(uint8_t address, uint8_t reg, uint8_t* dat, unsigned int num) {
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(address, num);
  while (Wire.available()) {
    *dat = 0;
    *dat = Wire.read();
    dat++;
  }
}

void Action_CMD(uint8_t address, uint8_t reg) {
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.endTransmission();
}
