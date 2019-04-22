#include <Arduino.h>
#include <mcp_can.h>
#include <SPI.h>

#include "icar.h"


icar ICar(0);
const int AVAR = 5;
const int FS = 3;
const int ACC = A0;

int AVAR_Status;
int FS_Status;
int ACC_Value;

unsigned char message_to_send[8] = {0,0,0,0,0,0,0,0};
unsigned char * p_message_to_send;
void setup() {
  Serial.begin(9600);
  ICar.init_can();
  ICar.init_FS_Pin(FS);
}

void loop() {
  //recuperer les information
  // 1) FS :
  FS_Status = ICar.FootSwitch_Read(FS);
  //Serial.println(FS_Status);
  
  // 2) AVAR : 
  AVAR_Status = ICar.AVAR_Read(AVAR);
  //Serial.println(AVAR_Status);
  
  // 3) Acceleration
  ACC_Value = ICar.Acceleration_Read(ACC);
  //Serial.println(ACC_Value);
  
  //initialiser le message
  p_message_to_send = ICar.initializingMessage(1, ACC_Value, AVAR_Status, FS_Status);
  
  for(int i=0; i < 8; ++i){
    message_to_send[i] = p_message_to_send[i];
    Serial.print(message_to_send[i]);
    Serial.print("\t");
  }
  delete[] p_message_to_send;
  Serial.println();
  
  //diffuser le message au can
  ICar.MessageCanWriter(message_to_send);
}
