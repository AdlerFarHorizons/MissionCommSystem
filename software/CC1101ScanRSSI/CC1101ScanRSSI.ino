/*
  Scans 33 cm band channels selected for appropriate Illinois Repeater Association
  usage and for center frequency within about 2kHz of 12.5 kHz channels using CC1101
  channel selection. Really should just program frequency directly, but the
  calculation isn't too simple and wasn't included in this version.
*/

#define CC1190
#include <SPI.h>
#include "RADIOFH_B.h"
#include "Step4_CC1190_915.h"
//String rssi;
// Channels are 902 - 902.2875 MHz with 12.5 kHz spacing.
// Channel 8 (902.1) is for calling and we won't be using it.
byte channel[23] = { 0,1,2,3,4,5,6,7,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23 };
float average[23];
float max[23] = {-200,-200,-200,-200,-200,-200,-200,-200,-200,-200,-200,-200,-200,-200,-200,-200,-200,-200,-200,-200,-200,-200,-200 };
int count, num;
float tmpave;

byte freq0;
byte freq1;
byte freq2;
  
void setup() {
  Serial.flush();
  Serial.begin (9600);
  
  
  SPI.begin ();
  SPI.setClockDivider(SPI_CLOCK_DIV4);   
  SPI.setBitOrder(MSBFIRST);   
  SPI.setDataMode(SPI_MODE0) ;
  CC1101.GDO_Set();
  CC1101.Reset();
  RegConfig();
  count = 0;
  num = 10;
  tmpave = 0;
  delay(1000);
  CC1101.SetReceive();
  delay(1000);  
}


void loop() {
  Serial.print( "Sweep " );Serial.print( count + 1 );
  Serial.println( " -------------------");
  for( int j = 0 ; j < sizeof( channel ) - 1 ; j++ ) {
    // Amateur experimental subband 902.0 - 902.875 MHz
    float base = 902.0 / (27.0 / 65536); //(29491200.0/13.0);  
    float chan = (4096.0 / 135.0) * channel[j]; //((2048.0/65.0)*channel[j]);
    long freq = (long)(base + chan + 0.5);
    
    freq0= (byte)(freq&0xFF);
    freq1= (byte)((freq&0xFF00)>>8);
    freq2= (byte)((freq&0xFF0000)>>16);
    
    CC1101.WriteSingleReg( RADIO_FREQ0, freq0);
    CC1101.WriteSingleReg( RADIO_FREQ1, freq1);
    CC1101.WriteSingleReg( RADIO_FREQ2, freq2);
    delay(100);
    tmpave = 0;
    for ( int k = 0 ; k < num ; k ++ ) {
      int tmprssi = CC1101.ReadRSSI( RSSI_OFFSET );
      tmpave += (float)tmprssi;
      delay(10);
    }
    tmpave /= num;
    average[j] = ( average[j] * count + tmpave ) / ( count + 1 );
    if ( tmpave > max[j] ) max[j] = tmpave;
    Serial.print("Chan ");Serial.print(channel[j]);
    Serial.print(": freq2 | freq1 | freq 0 | current | cum ave | max (dbm) ");
    Serial.print(" "); Serial.print(freq2); 
    Serial.print(" | "); Serial.print(freq1); 
    Serial.print(" | "); Serial.print(freq0); 
    Serial.print(" | ");
    Serial.print( tmpave, 1 );Serial.print( " | " );
    Serial.print( average[j], 1 );Serial.print( " | " );
    Serial.print( max[j], 1 );Serial.println( "" );
    delay(400);
  }
  count += 1;
}











