#ifndef __icar_h__
#define __icar_h__

//include library :
#include <Arduino.h> 
#include <SPI.h>
#include <mcp_can.h>

class icar
{
    public:
        icar();
        icar(long pin);
        void init_can();
        void init_FS_Pin(int FS_Pin);
        int FootSwitch_Read(const int FootSwitch_pin);
        int Acceleration_Read(const int Acc_pin);
        int AVAR_Read(const int AVAR_pin);
        void MessageCanWriter(unsigned char messagetowrite[8]);
        unsigned char* initializingMessage(int target_board, int analog, int AV_AR, int FS_status); //data to send are : concerned Arduino, FS status, Avant ou arriere, acceleration(speed) value
        unsigned char * MessageCanReader();
        unsigned char * getMsgReaded();
        void afficherMessage(unsigned char message[8]);

    protected:
        long _pin ;
        int _baud=9600;
        int FootSwitch_pin;
        unsigned char *messagetoread;
        int _fs;
        int _avarValue;
        int _analog;
};

#endif