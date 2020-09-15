#include "NMEA.h"

static uint16_t UTC_year = 0;
static uint8_t UTC_month = 0;
static uint8_t UTC_date = 0;
static uint8_t UTC_hour = 0;
static uint8_t UTC_minute = 0;
static uint8_t UTC_second = 0;
static bool positioning_available = false;
static bool utf_available = false;
static uint16_t latitude_ddd = 0;
static uint8_t latitude_mm = 0;
static float latitude_pt_mm = 0;
static char N_S = ' ';
static uint16_t longitude_ddd = 0;
static uint8_t longitude_mm = 0;
static float longitude_pt_mm = 0;
static char E_W = ' ';
static uint16_t height_above_sea_level = 0;
static uint16_t height_geoid = 0;
static uint8_t active_satellite_num = 0;

char Buffer[90];
char *BufPtr = Buffer;

//-----------------------------------------
void GPS_UART_Receive(char c){
	if(c == '$'){
		BufPtr = Buffer;
	}

	*BufPtr++ = c;

	if(c == '\n'){
		*BufPtr++ = '\0';
		GPS_Update();
	}
}
//-----------------------------------------

static void ReadToComma(char *valstr){
    do{ *(valstr++) = *BufPtr++; }while(*(valstr - 1) != ',');
}

static void dddmm_mmm(char* valstr,uint16_t *ddd, uint8_t *mm,float *_mmm){
    char *ptr = valstr;
    for(;*ptr != '.';ptr++);
    if(ptr - 1 >= valstr)*mm = (*(ptr - 1) - '0');
    if(ptr - 2 >= valstr)*mm += (*(ptr - 2) - '0') * 10;
    if(ptr - 3 >= valstr)*ddd = (*(ptr - 3) - '0');
    if(ptr - 4 >= valstr)*ddd += (*(ptr - 4) - '0') * 10;
    if(ptr - 5 >= valstr)*ddd += (*(ptr - 5) - '0') * 100;

    *_mmm = 0;
    for(float t = 0.1;*++ptr != ',';t *= 0.1)*_mmm += (*ptr - '0') * t;
}

static void height(char* valstr,uint16_t *height){
	*height = 0;
    char *ptr = valstr;
    for(;*ptr != ',';ptr++);
    for(uint16_t t = 1;--ptr != valstr;t *= 10)*height += (*ptr - '0') * t;
}

static void GPS_Update(){
	BufPtr = Buffer;
	char valstr[15];
	ReadToComma(valstr);

	if(valstr[3] == 'G' && valstr[4] == 'G' && valstr[5] == 'A'){	//GPGGA
		ReadToComma(valstr);
		if(valstr[0] != ','){
		    UTC_hour = 10 * (valstr[0] - '0') + (valstr[1] - '0');
		    UTC_minute = 10 * (valstr[2] - '0') + (valstr[3] - '0');
		    UTC_second = 10 * (valstr[4] - '0') + (valstr[5] - '0');
		}

		ReadToComma(valstr);
		if(valstr[0] != ',')dddmm_mmm(valstr,&latitude_ddd,&latitude_mm,&latitude_pt_mm);

		ReadToComma(valstr);
		if(valstr[0] == 'N')N_S = 'N';
		else if(valstr[0] == 'S')N_S = 'S';

		ReadToComma(valstr);
		if(valstr[0] != ',')dddmm_mmm(valstr,&longitude_ddd,&longitude_mm,&longitude_pt_mm);

		ReadToComma(valstr);
		if(valstr[0] == 'E')E_W = 'E';
		else if(valstr[0] == 'W')E_W = 'W';

		ReadToComma(valstr);
		if(valstr[0] == '1')positioning_available = true;

		ReadToComma(valstr);
		if(valstr[0] != ','){
		    if(valstr[1] != ',')active_satellite_num = (valstr[0] - '0') * 10 + (valstr[1] - '0');
		    else active_satellite_num = (valstr[0] - '0');
		}

		ReadToComma(valstr);

		ReadToComma(valstr);
		if(valstr[0] != ',')height(valstr,&height_above_sea_level);

		ReadToComma(valstr);

		ReadToComma(valstr);
		if(valstr[0] != ',')height(valstr,&height_geoid);
	}

	if(valstr[3] == 'Z' && valstr[4] == 'D' && valstr[5] == 'A'){ //GPZDA
		ReadToComma(valstr);
		if(valstr[0] != ','){
		    UTC_hour = 10 * (valstr[0] - '0') + (valstr[1] - '0');
		    UTC_minute = 10 * (valstr[2] - '0') + (valstr[3] - '0');
		    UTC_second = 10 * (valstr[4] - '0') + (valstr[5] - '0');
		}

		ReadToComma(valstr);
		if(valstr[0] != ',')UTC_date = (valstr[0] - '0') * 10 + (valstr[1] - '0');

		ReadToComma(valstr);
		if(valstr[0] != ',')UTC_month = (valstr[0] - '0') * 10 + (valstr[1] - '0');

		ReadToComma(valstr);
		if(valstr[0] != ',')UTC_year = (valstr[0] - '0') * 1000 + (valstr[1] - '0') * 100 + (valstr[2] - '0') * 10  + (valstr[3] - '0');

		utf_available = true;
	}
}

void GPS_Get_UTC(uint16_t *year,uint8_t *month, uint8_t *date, uint8_t *hour, uint8_t *minute, uint8_t *second){
	*year = UTC_year;
	*month = UTC_month;
	*date = UTC_date;
	*hour = UTC_hour;
	*minute = UTC_minute;
	*second = UTC_second;
}

bool GPS_Get_utfReady(){
	return utf_available;
}

bool GPS_Get_isReady(){
	return positioning_available;
}

void GPS_Get_Position_DMM(char *n_s,double *lat,char *e_w, double *lon){
	*n_s = N_S;
	*lat = (double)(latitude_ddd * 100 + latitude_mm) + latitude_pt_mm;
	*e_w = E_W;
	*lon = (double)(longitude_ddd * 100 + longitude_mm) + longitude_pt_mm;
}

void GPS_Get_Position_DDD(char *n_s,double *lat,char *e_w, double *lon){
	*n_s = N_S;
	*lat = (double)latitude_ddd + ((double)latitude_mm + latitude_pt_mm) / 60.0;
	*e_w = E_W;
	*lon = (double)longitude_ddd + ((double)longitude_mm + longitude_pt_mm) / 60.0;
}

void GPS_Get_Position_DMS(char *n_s,uint16_t *lat_ddd,uint8_t *lat_mm,float *lat_ss,char *e_w,uint16_t *lon_ddd,uint8_t *lon_mm,float *lon_ss){
	*n_s = N_S;
	*lat_ddd = latitude_ddd;
	*lat_mm = latitude_mm;
	*lat_ss = latitude_pt_mm * 60.0;
	*e_w = E_W;
	*lat_ddd = longitude_ddd;
	*lat_mm = longitude_mm;
	*lat_ss = longitude_pt_mm * 60.0;
}

uint16_t GPS_Get_Height(){
	return height_above_sea_level - height_geoid;
}
