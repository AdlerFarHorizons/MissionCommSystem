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
byte channel[18] = { 0,1,2,8,9,10,81,82,83,84,90,91,92,93,99,100,101,102 };
float average[18];
float max[18] = {-200,-200,-200,-200,-200,-200,-200,-200,-200,-200,-200,-200,-200,-200,-200,-200,-200,-200 };
int count, num;
float tmpave;

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
    CC1101.WriteSingleReg( RADIO_CHANNR,  channel[j] );
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
    Serial.print(": current | cum ave | max (dbm) ");
    Serial.print( tmpave, 1 );Serial.print( " | " );
    Serial.print( average[j], 1 );Serial.print( " | " );
    Serial.print( max[j], 1 );Serial.println( "" );
    delay(400);
  }
  count += 1;
}











