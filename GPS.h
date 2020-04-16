/*

	2020 Oku Dan / oku_dan@yahoo.co.jp

	GPSモジュール用ライブラリです.
    serialでモジュールとマイコンを接続してください.

    serial周りは自分で適宜実装してね.
    緯度経度は北東+,南西-のdddd.dddddでお願いします.

*/

#include <SystemHandler.h>  //Delaymsに必要

class GPS
{
    private:
        SerialPort serial;
        char str[128];
        bool available = false;

    public:
        bool Initialize();
        bool Update();
        bool available();
        float ReadLongitude();  //緯度
        float ReadLatitude();   //経度
        float ReadAltitude();   //高度
        float ReadUTC();        //標準時
};

bool GPS::Initialize(){
    if(!serial.Initialize(9600,1))return false;
    if(Update()){
        return true
    }else{
        Delayms(1000);
        return Update();
    }
}

bool GPS::Update(){
    int c = serial.ReadByte();
    if(c == '$'){
        for(int i = 0;i < 128;i++){
            c = serial.ReadByte();
            if(c == -1){
                return false;
            }else{
                if(c == '\n')break;
                else str[i] = (char)c;
            }
        }
        return true;
    }
    else{
        return false;
    }
}

bool GPS::available(){
    return available;
}


//以下文字列処理系の実装待ち
float GPS::ReadLongitude(){
}

float GPS::ReadLatitude(){

}

float GPS::ReadAltitude(){

}

float GPS::ReadUTC(){

}