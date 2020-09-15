#ifndef INC_NMEA_H_
#define INC_NMEA_H_

#include "stdint.h"
#include "stdbool.h"

void GPS_UART_Receive(char c);	//set UART rx interrupt
static void GPS_Update();
void GPS_Get_UTC(uint16_t *year,uint8_t *month, uint8_t *date, uint8_t *hour, uint8_t *minute, uint8_t *second);
bool GPS_Get_utfReady();
bool GPS_Get_isReady();
void GPS_Get_Position_DMM(char *N_S,double *lat,char *E_W, double *lon);
void GPS_Get_Position_DDD(char *N_S,double *lat,char *E_W, double *lon);
void GPS_Get_Position_DMS(char *N_S,uint16_t *lat_ddd,uint8_t *lat_mm,float *lat_ss,char *E_W,uint16_t *lon_ddd,uint8_t *lon_mm,float *lon_ss);
uint16_t GPS_Get_Height();

#endif
