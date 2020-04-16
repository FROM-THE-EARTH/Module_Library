#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H

/*

センサのライブラリ群用のSerialライブラリです。
別のマイコンを利用する場合は関数名、引数を変えないで互換性を保ってください。
新たにセンサのライブラリを作る際にもSerialを利用する際はこのハンドラを通して、特定のマイコン専用にならないようにしてください。

*/

class SerialPort
{
    private:
        bool initialized = false;
        void* handler;
    public:
        bool Initialize();
        bool Initialize(int baudrate);
        bool Initialize(int baudrate,int num);  //複数ポートがあるマイコン用
        void WriteByte(uint8_t data);
        void WriteBytes(uint8_t* buffer,int length);
        int ReadByte();
        void ReadByte(uint8_t* buffer,int length);
}

#define ARDUINO	//ここにマイコンを追記していく
//#define MBED
//#define STM32

#ifdef ARDUINO						//arduinoのSerial用

bool SerialPort::Initialize(int baudrate,int num){
    if(num == 0){
        if(!initialized){
            Serial.begin(baudrate);
            initialized = true;
        }
        return true;
    }else{
        return false;
    }
}

bool SerialPort::Initialize(int baudrate){
    return Initialize(baudrate,0);
}

bool SerialPort::Initialize(){
    return Initialize(115200,0);
}

void SerialPort::WriteByte(uint8_t data)
{
    Serial.write(data);
}

void SerialPort::WriteBytes(uint8_t* buffer,int length){
    Serial.write(buffer,length);
}

int SerialPort::ReadByte()
{
    return Serial.read();
}

void SerialPort::ReadBytes(uint8_t* buffer,int length){
    Serial.readBytes(buffer, length);
}

#endif

#ifdef STM32						//STM32のSerial用

bool SerialPort::Initialize(int baudrate,int num){
    if(!initialized){
        switch (num)
        {
        case 1:
            handler = &huart1;
            break;
        case 2:
            handler = &huart2;
            break;
        case 3:
            handler = &huart3;
            break;
        default:
            break;
        }
        (*handler).Init.BaudRate = baudrate;
        if (HAL_UART_Init(handler) != HAL_OK)return false;
        initialized = true;
    }
    return true;
}

bool SerialPort::Initialize(int baudrate){
    return Initialize(baudrate,1);
}

bool SerialPort::Initialize(){
    return Initialize(115200,1);
}

void SerialPort::WriteByte(uint8_t data)
{
    HAL_UART_Transmit(handler,&data, 1, 0xF);
}

void SerialPort::WriteBytes(uint8_t* buffer,int length){
    HAL_UART_Transmit(handler,buffer, length, 0xFFF);
}

int SerialPort::ReadByte()
{
    uint8_t data;
    HAL_UART_Receive(handler,&data, 1, 0xF);
    return (int)data;
}

void SerialPort::ReadBytes(uint8_t* buffer,int length){
    HAL_UART_Receive(handler,buffer, length, 0xFFF);
}

#endif

#endif