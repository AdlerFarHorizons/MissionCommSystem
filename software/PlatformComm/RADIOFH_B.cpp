/* Arduino Radio Library 

  Lou Nigra, Anuja Mahashabde
  Far Horizons Lab

  Extended from:
  RADIO library for Arduino Version 0.1
  by BHARAT UDDAGIRI

  Additional Files:
    RADIOFH.h Header file - must be in RADIO directory along with this file.

  Version 1.0 Dec 3, 2013
  Notes:

  1.Unlike RADIO.cpp, there is no RegConfig method with application-specific
    register settings. The intent is to make the library application-independent.
    Similarly, there are no PA Table settings. PA settings are done with the
    WriteBurstReg method with the PaTable array as in the settings header
    for the initial setting.

  2.An additional method to read RSSI in dBm was added.

  3.Application-specific non-default register settings are set through a custom 
    header file <some_name>.h in the application sketch directory with the
    following template:

    ---- Start Template ---
    #define PA_TABLE {<pa1>, <pa1>, <pa1>, <pa1>, <pa1>, <pa1>, <pa1>, <pa1>, }

    void RegConfig() {
      CC1101.WriteSingleReg( RADIO_<REGISTER_NAME1>, <reg_val1> );
      CC1101.WriteSingleReg( RADIO_<REGISTER_NAME2>, <reg_val2> );
      ...

      CC1101.WriteSingleReg( RADIO_<REGISTER_NAMEn>, <reg_valn> );

      byte paTable[8] = PA_TABLE;
      CC1101.WriteBurstReg( RADIO_PATABLE, paTable, 8 );
    }
    ---- End ----

    This file can be auto-generated using the TI SmartRF software with the appropriate
    template defined. One note, however, use copy and paste as the PA table #define isn't
    included in the output file.

  4.Arduino includes:

      #include <SPI.h>
      #include "RADIOFH.h"
      #include "some_name.h" // Local Register settings file
 */
#include "RADIOFH_B.h"
#include "pins_arduino.h"
#include "SPI.h"

RADIOClass CC1101;

void RADIOClass::Reset (void) {

  digitalWrite(SS, LOW);
  delay(1);
  digitalWrite(SS, HIGH);
  delay(1);
  digitalWrite(SS, LOW);
  while(digitalRead(MISO));
  SPI.transfer(RADIO_SRES);
  while(digitalRead(MISO));
  digitalWrite(SS, HIGH);
  digitalWrite( LNA_EN, LOW );
  digitalWrite( PA_EN, LOW );
}



void RADIOClass::WriteSingleReg(byte addr, byte paue)  {

  byte temp;
  temp = addr|WRITE_SINGLE;
  digitalWrite(SS, LOW);
  while(digitalRead(MISO));     
  SPI.transfer(temp);
  SPI.transfer(paue);        
  digitalWrite(SS, HIGH);
 
}


byte RADIOClass::ReadSingleReg(byte addr) {

  byte temp, paue;
  temp = addr|READ_SINGLE;
  digitalWrite(SS, LOW);
  while(digitalRead(MISO));
  SPI.transfer(temp);
  paue=SPI.transfer(0);
  digitalWrite(SS, HIGH);

  return paue;
 
}


void RADIOClass::ReadBurstReg(byte addr, byte *buffer, byte count) {

  byte i,temp;
  temp = addr | READ_BURST;
  digitalWrite(SS, LOW);
  while(digitalRead(MISO));
  SPI.transfer(temp);
  for(i=0;i<count;i++)  {

    buffer[i]=SPI.transfer(0);

  }

  digitalWrite(SS, HIGH);

}


void RADIOClass::WriteBurstReg(byte addr, byte *buffer, byte count) {

  byte i, temp;
  temp = addr | WRITE_BURST;
  digitalWrite(SS, LOW);
  while(digitalRead(MISO));
  SPI.transfer(temp);

  for (i = 0; i < count; i++) {

    SPI.transfer(buffer[i]);

  }

  digitalWrite(SS, HIGH);

}

void RADIOClass::SendData(byte *txBuffer,byte size)
{
  digitalWrite( LNA_EN, LOW );
  digitalWrite( PA_EN, HIGH );
  WriteSingleReg(RADIO_TXFIFO,size);
  WriteBurstReg(RADIO_TXFIFO,txBuffer,size);      //write data to send
  Strobe(RADIO_STX);                              //start send  
  while (!digitalRead(GDO0));                     // Wait for GDO0 to be set -> sync transmitted  
  while (digitalRead(GDO0));                      // Wait for GDO0 to be cleared -> end of packet
  digitalWrite( PA_EN, LOW );
  Strobe(RADIO_SFTX);                             //flush TXfifo

}


void RADIOClass::Strobe(byte strobe) {

  digitalWrite(SS, LOW);
  while(digitalRead(MISO));
  SPI.transfer(strobe);
  digitalWrite(SS, HIGH);

}



void RADIOClass::GDO_Set (void) {

  pinMode(GDO0, INPUT);
  pinMode(GDO2, INPUT);
  pinMode(LNA_EN, OUTPUT);
  pinMode(PA_EN, OUTPUT);

}


// Receive mode setup and methods 


void RADIOClass::SetReceive(void) {
  digitalWrite( PA_EN, LOW );
  digitalWrite( LNA_EN, HIGH );
  Strobe(RADIO_SRX);

}

byte RADIOClass::CheckReceiveFlag(void) {

  if(digitalRead(GDO0)) {     //receive data

    while (digitalRead(GDO0));
    digitalWrite( LNA_EN, LOW );
    return 1;

  }  else {                   // no data

    return 0;

  }

}

byte RADIOClass::ReadStatusReg(byte addr) {

  byte temp, paue;
  temp = addr|READ_BURST;   
  digitalWrite(SS, LOW);
  while(digitalRead(MISO));
  SPI.transfer(temp);
  paue=SPI.transfer(0);
  digitalWrite(SS, HIGH);

  return paue;
 
}

byte RADIOClass::ReceiveData(byte *rxBuffer) {
  byte size;
  byte status[2];

  if(ReadStatusReg(RADIO_RXBYTES) & BURST_BYTES_IN_TXFIFO) {  // 0x7F MULTIPLE TRY PRESENT BF
    size=ReadSingleReg(RADIO_RXFIFO);
    ReadBurstReg(RADIO_RXFIFO,rxBuffer,size);
    ReadBurstReg(RADIO_RXFIFO,status,2);
    Strobe(RADIO_SFRX);
    return size;

  } else {

    Strobe(RADIO_SFRX);

    return 0;
  }  
}

int RADIOClass::ReadRSSI() {
  int rssiOffset = 74;
  Serial.println(rssiOffset);
  int rssiDec = (int)CC1101.ReadStatusReg(RADIO_RSSI);
  if(rssiDec >= 128) {
    return (rssiDec-256)/2-rssiOffset;
  } else {
    return (rssiDec)/2-rssiOffset;
  }
}

int RADIOClass::ReadRSSI(int offset) {
  int rssiDec = (int)CC1101.ReadStatusReg(RADIO_RSSI);
  if(rssiDec >= 128) {
    return (rssiDec-256)/2-offset;
  } else {
    return (rssiDec)/2-offset;
  }
}
