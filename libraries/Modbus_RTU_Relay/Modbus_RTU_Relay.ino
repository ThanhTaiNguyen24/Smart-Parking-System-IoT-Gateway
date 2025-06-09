#include <SoftwareSerial.h>
#include "modbus_crc.h"

SoftwareSerial RS485(2, 8); // RX, TX
int RS485_E = 7;
unsigned char  cmd[8] = {0,0,0,0,0,0,0,0}; 
unsigned char  buf[20]; 
unsigned int   crc;
unsigned char i,j;

void setup()  
{
  Serial.begin(115200);
  Serial.println("*** Modbus RTU Relay Test Program ***\r\n");

  RS485.begin(9600);
  pinMode(RS485_E,OUTPUT);
}

void loop() // run over and over
{   
    for(i=0;i<8;i++){
      cmd[0] = 0x01;
      cmd[1] = 0x05;
      cmd[2] = 0;
      cmd[3] = i;
      cmd[4] = 0xFF;
      cmd[5] = 0;
      crc = ModbusCRC((unsigned char  *)cmd,6);
      cmd[6] = crc & 0xFF;
      cmd[7] = crc >> 8;
      digitalWrite(RS485_E,HIGH);   //send
      for(j=0;j<8;j++){
        RS485.write(cmd[j]);
      }
      digitalWrite(RS485_E,LOW);   //receive
      delay(200);
      while(RS485.available() > 0) {
        Serial.print(RS485.read(),HEX);
        Serial.print(" ");
      }
      Serial.println("");
    }
    
    for(i=0;i<8;i++){
      cmd[0] = 0x01;
      cmd[1] = 0x05;
      cmd[2] = 0;
      cmd[3] = i;
      cmd[4] = 0;
      cmd[5] = 0;
      crc = ModbusCRC((unsigned char  *)cmd,6);
      cmd[6] = crc & 0xFF;
      cmd[7] = crc >> 8;
      digitalWrite(RS485_E,HIGH);   //send
      for(j=0;j<8;j++){
        RS485.write(cmd[j]);
      }
      digitalWrite(RS485_E,LOW);   //receive
      delay(200);
      while(RS485.available() > 0) {
        Serial.print(RS485.read(),HEX);
        Serial.print(" ");
      }
      Serial.println("");
    }

    cmd[0] = 0x01;
    cmd[1] = 0x02;
    cmd[2] = 0;
    cmd[3] = 0;
    cmd[4] = 0;
    cmd[5] = 8;
    crc = ModbusCRC((unsigned char  *)cmd,6);
    cmd[6] = crc & 0xFF;
    cmd[7] = crc >> 8;
    digitalWrite(RS485_E,HIGH);   //send
    for(j=0;j<8;j++){
      RS485.write(cmd[j]);
    }
    digitalWrite(RS485_E,LOW);   //receive
    delay(200);
    i=0;
    while(RS485.available() > 0) {
      buf[i]=RS485.read();
      i=i+1;
    }
    Serial.print("Input status : ");
    Serial.println(buf[3],HEX);
}
