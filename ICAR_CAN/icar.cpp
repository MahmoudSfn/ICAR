#include "icar.h"
#include <Arduino.h>
#include <SPI.h>
#include <mcp_can.h>

//TODO : 
/*
    1) initialisation du CAN
*/


int _SPI_CS_PIN=9;
MCP_CAN CAN(_SPI_CS_PIN);

icar::icar() 
{
    //default constructor
}

icar::icar(long pin)
{
    _pin = pin;
}

void icar::init_can()
{
    Serial.begin(_baud);
    while(CAN_OK != CAN.begin(CAN_500KBPS))
    {
        Serial.println("CAN BUS initialisation failed! Trying again ...");
        delay(10);
    }
    Serial.println("CAN BUS initialisation succeeded!");
}

void icar::init_FS_Pin(int FS_Pin)
{
    FootSwitch_pin = FS_Pin;
    pinMode(FootSwitch_pin, INPUT_PULLUP);
}

int icar::FootSwitch_Read(const int FootSwitch_pin)
{   
    int fs = digitalRead(FootSwitch_pin);
    return fs;
}

int icar::AVAR_Read(const int AVAR_pin)
{   
    int avarValue = digitalRead(AVAR_pin);
    return avarValue;
}

int icar::Acceleration_Read(const int Acc_pin)
{
    int analog = analogRead(Acc_pin);
    analog = map(analog, 0, 1023, 0,255);
    return analog;
} 

unsigned char* icar::initializingMessage(int target_board, int analog, int AV_AR, int FS_status)
{
    unsigned char* message = new unsigned char[8];

    message[0] = target_board;
    message[1] = analog;
    message[2] = FS_status;
    message[3] = AV_AR;
    message[4] = 0;
    message[5] = 0;
    message[6] = 0;
    message[7] = 0;
    //afficherMessage(message);
    return message;
}
void icar::afficherMessage(unsigned char message[8])
{
    Serial.print("Message avant envoye: ");
    for (int i = 0; i < 8; i++){
        Serial.print(message[i]);
        Serial.print("\t");
    }
    Serial.println();
}
void icar::MessageCanWriter(unsigned char messagetowrite[8])  //le messagetowrite porte les differente detail recuperer et qui sont
{
    CAN.sendMsgBuf(0x00,0,8,(byte *)messagetowrite);
    delay(100);
}

unsigned char * icar::MessageCanReader()
{
    unsigned char length = 8;
    messagetoread = new unsigned char[8]; 

    if(CAN_MSGAVAIL == CAN.checkReceive())
    {
        CAN.readMsgBuf(&length, messagetoread);
        //unsigned long canId = CAN.getCanId();
    }
    return messagetoread;
}

unsigned char * icar::getMsgReaded()
{
    return messagetoread;
}