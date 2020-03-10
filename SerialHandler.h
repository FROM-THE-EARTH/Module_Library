#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H

/*

センサのライブラリ群用のSerialライブラリです。
別のマイコンを利用する場合は関数名、引数を変えないで互換性を保ってください。
新たにセンサのライブラリを作る際にもSerialを利用する際はこのハンドラを通して、特定のマイコン専用にならないようにしてください。

*/

#define ARDUINO	//ここにマイコンを追記していく
//#define MBED

#ifdef ARDUINO						//arduinoのSerial用
//とりあえずやっつけで適当に書いたので後世の人頼んだ
//いろいろ便利機能つけすぎて再利用性落とさんようにな～^^

bool initialized = false;

void SerilInitialize(long baudrate){
    if(!initialized){
        Serial.begin(baudrate);
        initialized = true;
    }
}

void SerilInitialize(){
    SerilInitialize(115200);
}

void SerialWriteByte(uint8_t data)
{
    Serial.write(data);
}

int SerialReadByte()
{
    return Serial.read();
}

#endif

#endif