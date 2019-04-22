#include <Arduino.h>
#include <mcp_can.h>
#include <SPI.h>

#include "icar.h"

//value to write on the variator
int ARR_pin = 3;
int AV_pin = 2;
int FS_pin = 1;
int ACC_pin = 6;

//pin Status_to_write
bool ARR_Status_to_write;
bool AV_Status_to_write;
bool FS_Status_to_write;
int ACC_Status_to_write;
int am_I_concened;
unsigned char message_recieved[8] = {0,0,0,0,0,0,0,0};
unsigned char *p_message_recieved;


unsigned char len = 8;


icar ICar(0);

void setup() {
  Serial.begin(9600);
  ICar.init_can();
  //Initial state of the variator :
  ARR_Status_to_write = LOW;
  AV_Status_to_write = HIGH;
  FS_Status_to_write = LOW;
  ACC_Status_to_write = 255;
  digitalWrite(FS_pin, FS_Status_to_write);
  digitalWrite(ARR_pin, ARR_Status_to_write);
  digitalWrite(AV_pin, AV_Status_to_write);
  digitalWrite(ACC_pin, ACC_Status_to_write);
}

void loop() {
  p_message_recieved = ICar.MessageCanReader();
  for(int i=0; i < 8; ++i){
    message_recieved[i] = p_message_recieved[i];
    Serial.print(message_recieved[i]);
    Serial.print("\t");
  }
  Serial.println();

  // filling variable with recent Status_to_write
  am_I_concened = message_recieved[0];
  
  if (am_I_concened == "1"){
    if (message_recieved[2] == 1) {
      FS_Status_to_write = HIGH;
    } else {
      FS_Status_to_write = LOW;
    }
    digitalWrite(FS_pin, FS_Status_to_write);

    if (message_recieved[3] == 1) {
      AV_Status_to_write = HIGH;
      ARR_Status_to_write = LOW;
    } else {
      AV_Status_to_write = LOW;
      ARR_Status_to_write = HIGH;
    }
    digitalWrite(ARR_pin, ARR_Status_to_write);
    digitalWrite(AV_pin, AV_Status_to_write);
    
    ACC_Status_to_write = message_recieved[1];
    digitalWrite(ACC_pin, ACC_Status_to_write); //3.3v
  }
  delete[] p_message_recieved;
}
