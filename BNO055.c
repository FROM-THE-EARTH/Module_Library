#include "BNO055.h"
#include "I2C2Handler.h"

bool initialized = false;

void I2cInitialize(){
  if(!initialized){
    initialized = true;
  }
}

static void Delay(uint16_t ms){
#ifdef STM32
	HAL_Delay(1);
#endif
}

bool BNO055_Initialize_Fusion(){
    if(I2cReadByte(BNO055_ADRESS,BNO055_CHIP_ID) != BNO055_CHIP_ID_DEFAULT)return false;
    I2cWriteByte(BNO055_ADRESS,BNO055_PWR_MODE,0);
    BNO055_SetUnit();
    I2cWriteByte(BNO055_ADRESS,BNO055_OPR_MODE,BNO055_OPR_MODE_NDOF);
    Delay(20);
    return true;
}

bool BNO055_Initialize_Raw(){
    if(I2cReadByte(BNO055_ADRESS,BNO055_CHIP_ID) != BNO055_CHIP_ID_DEFAULT)return false;
    I2cWriteByte(BNO055_ADRESS,BNO055_PWR_MODE,0);
    BNO055_SetUnit();
    I2cWriteByte(BNO055_ADRESS,BNO055_OPR_MODE,BNO055_OPR_MODE_AMG);
    Delay(20);
    return true;
}

void BNO055_SetUnit(){
    I2cWriteByte(BNO055_ADRESS,BNO055_UNIT_SEL,BNO055_UNIT_SEL_ACC | BNO055_UNIT_SEL_GYR | BNO055_UNIT_SEL_ANGL | BNO055_UNIT_SEL_Tmp | BNO055_UNIT_SEL_Fus);
}

void BNO055_ReadAcc(float* x,float* y,float* z){
    uint8_t data[6];
    I2cReadBytes(BNO055_ADRESS,BNO055_ACC_DATA_X_LSB,data,6);

    int16_t _x,_y,_z;
    _x = data[1] << 8 | data[0];
    _y = data[3] << 8 | data[2];
    _z = data[5] << 8 | data[4];

    #if BNO055_UNIT_SEL_ACC == BNO055_UNIT_SEL_ACC_ms2
        *x = (float)_x / 100;
        *y = (float)_y / 100;
        *z = (float)_z / 100;
    #elif BNO055_UNIT_SEL_ACC == BNO055_UNIT_SEL_ACC_mG
        *x = (float)_x;
        *y = (float)_y;
        *z = (float)_z;
    #endif
}

void BNO055_ReadLinAcc(float* x,float* y,float* z){
    uint8_t data[6];
    I2cReadBytes(BNO055_ADRESS,BNO055_LIA_Data_X_LSB,data,6);

    int16_t _x,_y,_z;
    _x = data[1] << 8 | data[0];
    _y = data[3] << 8 | data[2];
    _z = data[5] << 8 | data[4];

    #if BNO055_UNIT_SEL_ACC == BNO055_UNIT_SEL_ACC_ms2
        *x = (float)_x / 100;
        *y = (float)_y / 100;
        *z = (float)_z / 100;
    #elif BNO055_UNIT_SEL_ACC == BNO055_UNIT_SEL_ACC_mG
        *x = (float)_x;
        *y = (float)_y;
        *z = (float)_z;
    #endif
}

void BNO055_ReadGrvAcc(float* x,float* y,float* z){
    uint8_t data[6];
    I2cReadBytes(BNO055_ADRESS,BNO055_GRV_Data_X_LSB,data,6);

    int16_t _x,_y,_z;
    _x = data[1] << 8 | data[0];
    _y = data[3] << 8 | data[2];
    _z = data[5] << 8 | data[4];

    #if BNO055_UNIT_SEL_ACC == BNO055_UNIT_SEL_ACC_ms2
        *x = (float)_x / 100;
        *y = (float)_y / 100;
        *z = (float)_z / 100;
    #elif BNO055_UNIT_SEL_ACC == BNO055_UNIT_SEL_ACC_mG
        *x = (float)_x;
        *y = (float)_y;
        *z = (float)_z;
    #endif
}

void BNO055_ReadMag(float* x,float* y,float* z){
    uint8_t data[6];
    I2cReadBytes(BNO055_ADRESS,BNO055_MAG_DATA_X_LSB,data,6);

    int16_t _x,_y,_z;
    _x = data[1] << 8 | data[0];
    _y = data[3] << 8 | data[2];
    _z = data[5] << 8 | data[4];

    *x = (float)_x / 16;
    *y = (float)_y / 16;
    *z = (float)_z / 16;
}

void BNO055_ReadGyr(float* x,float* y,float* z){
    uint8_t data[6];
    I2cReadBytes(BNO055_ADRESS,BNO055_GYR_DATA_X_LSB,data,6);

    int16_t _x,_y,_z;
    _x = data[1] << 8 | data[0];
    _y = data[3] << 8 | data[2];
    _z = data[5] << 8 | data[4];

    #if BNO055_UNIT_SEL_GYR == BNO055_UNIT_SEL_GYR_Dps
        *x = (float)_x / 16;
        *y = (float)_y / 16;
        *z = (float)_z / 16;
    #elif BNO055_UNIT_SEL_GYR == BNO055_UNIT_SEL_GYR_Rps
        *x = (float)_x / 900;
        *y = (float)_y / 900;
        *z = (float)_z / 900;
    #endif
}

void BNO055_ReadAMG(float* ax,float* ay,float* az,float* mx,float* my,float* mz,float* gx,float* gy,float* gz){
    uint8_t data[18];
    I2cReadBytes(BNO055_ADRESS,BNO055_ACC_DATA_X_LSB,data,18);

    int16_t _x,_y,_z;

    _x = data[1] << 8 | data[0];
    _y = data[3] << 8 | data[2];
    _z = data[5] << 8 | data[4];

    #if BNO055_UNIT_SEL_ACC == BNO055_UNIT_SEL_ACC_ms2
        *ax = (float)_x / 100;
        *ay = (float)_y / 100;
        *az = (float)_z / 100;
    #elif BNO055_UNIT_SEL_ACC == BNO055_UNIT_SEL_ACC_mG
        *ax = (float)_x;
        *ay = (float)_y;
        *az = (float)_z;
    #endif

    _x = data[7] << 8 | data[6];
    _y = data[9] << 8 | data[8];
    _z = data[11] << 8 | data[10];

    *mx = (float)_x / 16;
    *my = (float)_y / 16;
    *mz = (float)_z / 16;

    _x = data[13] << 8 | data[12];
    _y = data[15] << 8 | data[14];
    _z = data[17] << 8 | data[16];

    #if BNO055_UNIT_SEL_GYR == BNO055_UNIT_SEL_GYR_Dps
        *gx = (float)_x / 16;
        *gy = (float)_y / 16;
        *gz = (float)_z / 16;
    #elif BNO055_UNIT_SEL_GYR == BNO055_UNIT_SEL_GYR_Rps
        *gx = (float)_x / 900;
        *gy = (float)_y / 900;
        *gz = (float)_z / 900;
    #endif
}

void BNO055_ReadEul(float* heading,float* roll,float* pitch){
    uint8_t data[6];
    I2cReadBytes(BNO055_ADRESS,BNO055_EUL_Heading_LSB,data,6);

    int16_t _h,_r,_p;
    _h = data[1] << 8 | data[0];
    _r = data[3] << 8 | data[2];
    _p = data[5] << 8 | data[4];

    #if BNO055_UNIT_SEL_ANGL == BNO055_UNIT_SEL_ANGL_Deg
        *heading = (float)_h / 16;
        *roll = (float)_r / 16;
        *pitch = (float)_p / 16;
    #elif BNO055_UNIT_SEL_ANGL == BNO055_UNIT_SEL_ANGL_Rad
        *heading = (float)_h / 900;
        *roll = (float)_r / 900;
        *pitch = (float)_p / 900;
    #endif
}

void BNO055_ReadQua(float* x,float* y,float* z,float* w){
    uint8_t data[8];
    I2cReadBytes(BNO055_ADRESS,BNO055_QUA_Data_w_LSB,data,8);

    int16_t _x,_y,_z,_w;
    _w = data[1] << 8 | data[0];
    _x = data[3] << 8 | data[2];
    _y = data[5] << 8 | data[4];
    _z = data[7] << 8 | data[6];

    *x = (float)_x / 16384;
    *y = (float)_y / 16384;
    *z = (float)_z / 16384;
    *w = (float)_w / 16384;
}
