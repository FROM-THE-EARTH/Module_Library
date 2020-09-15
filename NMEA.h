#ifndef INC_NMEA_H_
#define INC_NMEA_H_

#include "stdint.h"
#include "stdbool.h"

typedef struct _NMEA_Data
{
    uint16_t UTC_year = 0;
    uint8_t UTC_month = 0;
    uint8_t UTC_date = 0;
    uint8_t UTC_hour = 0;
    uint8_t UTC_minute = 0;
    uint8_t UTC_second = 0;
    bool positioning_available = false;
    bool utf_available = false;
    uint16_t latitude_ddd = 0;
    uint8_t latitude_mm = 0;
    float latitude_pt_mm = 0;
    char N_S = ' ';
    uint16_t longitude_ddd = 0;
    uint8_t longitude_mm = 0;
    float longitude_pt_mm = 0;
    char E_W = ' ';
    uint16_t height_above_sea_level = 0;
    uint16_t height_geoid = 0;
    uint8_t active_satellite_num = 0;
}NMEA_Data;

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
