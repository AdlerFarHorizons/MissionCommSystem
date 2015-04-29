/*
  Basic example CC1101 Transceiver master unit.
 
 Lou Nigra
 Far Horizons Lab
 
 Adapted from basic TestRx and TestTx programs.
 
 Transmits a short packet on timer interrupt and waits for
 a response packet from the slave unit. Tests for receipt
 of response.  First byte of packet is an index count. 
*/

#define CC1190
#include <Wire.h>
#include <SPI.h>
#include "RADIOFH_B.h"
#include <MsTimer2.h>

// Local non-default settings exported from TI SmartRF Studio
#include "CC1101_CC1190_33cm_gfsk_2401_2884_Man_P26dbm.h"

int packet_length = 30; // Max is 61
//byte channel = 0;
int period = 5000; // ms
byte i;
byte size;
boolean rcvdFlag; // Used to identify a missing slave response.
unsigned int index; // First and second transmitted byte (LSByte first)
boolean intFlag;
int rssi;
int numMissed;

String txMsg;
String rxMsg;
int ledPin = 4;
boolean ledOn;
int tempPin = 3;
int tempSens;
int lightPin = 0;
int watchDogPin = 3;
int lightSens;
int rcvdRSSI;
float baseFreqMHz = 902.0;
int channel = 0;
float chanSpaceHz = 12500.0;

void setup () {

  intFlag = false;
  Serial.begin (9600);
  //Serial.println ();
  MsTimer2::set(period, transmit);
  MsTimer2::start();
  SPI.begin ();
  SPI.setClockDivider(SPI_CLOCK_DIV4);   
  SPI.setBitOrder(MSBFIRST);   
  SPI.setDataMode(SPI_MODE0) ;
  CC1101.GDO_Set();
  CC1101.Reset();
  RegConfig(); 
  pinMode( ledPin, OUTPUT );
  pinMode( watchDogPin, OUTPUT );
  digitalWrite( ledPin, false );
  //CC1101.WriteSingleReg( RADIO_CHANNR,channel );  
  setFreq( baseFreqMHz + channel * chanSpaceHz );
  delay(1000);
  // Note: there's no SetReceive() as Master starts with transmit
  rcvdFlag = true;
  index = 0;
  numMissed = 0;
  
  Serial.println("arduino reset: External 1sec watchdog enabled.");
  digitalWrite( watchDogPin, HIGH );  
}

void loop() {
  // Reset external watchdog 
  resetWatchDog();
  if ( intFlag ) {
    /* Read sensors
         Note temperature converted to integer representing
         temperature in tenths of a degree so we can use the
         String() function to convert it to a string (it doesn't
         do floats)
    */
    tempSens = (int)( 10 * readTemp( tempPin, 0, 3.3 ) + 0.5 );
    lightSens = analogRead( lightPin );
    if ( rcvdFlag ) {
      rcvdRSSI = -rssi;
    } else {
      rcvdRSSI = 200;
    }
    String callSign = "  WB9SKY ";
    // To concatenate strings with '+', first addend must be a string
    // variable, not a string literal.
    txMsg = callSign + "R" + String(rcvdRSSI) + "T" + String(tempSens) + "L" + String(lightSens);

    // Construct packet here instead of in transmit function
    txMsg.getBytes( Pkt_Buffer, packet_length + 1 );
    Pkt_Buffer[0]=(byte)(index & 0x00FF);
    Pkt_Buffer[1]=(byte)( ( index >> 8 ) & 0x00FF );
    Serial.print( "index:");Serial.print(index);
    Serial.print(" Sending:");Serial.println( (char*)&Pkt_Buffer[2] );
    CC1101.SendData(Pkt_Buffer,packet_length);
    index++;
    /* 
      A delay of 50 us from SendData to SetReceive seems to be the
      threshold for transmitting the packet properly most of the
      time.
    */
    delayMicroseconds(100);
    CC1101.SetReceive();
    if (!rcvdFlag ) {
      Serial.println( "!" );
      numMissed++;
    } else {
      digitalWrite( ledPin, ledOn );
    }
    resetWatchDog();
    rcvdFlag = false;
    intFlag = false;
    Serial.flush();
    resetWatchDog();
  }
  if(CC1101.CheckReceiveFlag()) {
    
    size=CC1101.ReceiveData(Pkt_Buffer);
    rssi = CC1101.ReadRSSI(RSSI_OFFSET);
    rcvdFlag = true;
    Serial.print( numMissed );
    Serial.print( " missed ");
    Serial.print(" ");
    Serial.print( rssi );
    Serial.print( " Msg:" );
    Pkt_Buffer[0] = ' ';
    Serial.print( (char*) Pkt_Buffer );
    Serial.println("");
    Serial.flush();
    boolean cmdFound = false;
    for ( int i = 0 ; i < packet_length ; i++ ) {
      if ( Pkt_Buffer[i] == 0 ) break;
      if ( Pkt_Buffer[i] == 'L' ) {
        Serial.print("rcvd L");        
        if ( ( i + 1 ) < packet_length ) {
          if ( Pkt_Buffer[i+1] == '0' ) {
            Serial.println("0");
            ledOn = false;
            break;
          }
          if ( Pkt_Buffer[i+1] == '1' ) {
            Serial.println("1");
            ledOn = true;
            break;
          }
        }
      }
    }
    resetWatchDog();
    if ( ledOn ) {
      boolean state = true;
      for ( int i = 0 ; i < 2000 ; i++ ) {
        digitalWrite( ledPin, state );
        state = !state;
        delayMicroseconds( 250 );
      }
    }
  }
}

void transmit() {
  intFlag = true;
//  txMsg.getBytes( Pkt_Buffer, packet_length + 1 );
//  Pkt_Buffer[0]=index;
//  index++;
}

String byteToHexStr( byte byteVal ) {
  if ( byteVal < 16 ) {
    return "0" + String( byteVal, HEX );
  } else {
    return String( byteVal, HEX );
  }
}

byte collecti2c(byte experiment, byte length, byte * storage)
{
  byte count=0;
  Wire.requestFrom(experiment,length);
  while(Wire.available())
  {
    storage[count]=Wire.read();
    count ++;
  }
  return count;
}

int getMaxRssi( int chan, int deltaTms, int numSamp ) {
  int  maxRssi = -999;
  int rssi = -999;
  for ( int i = 0 ; i < numSamp ; i++ ) {
    rssi = CC1101.ReadRSSI( RSSI_OFFSET );
    if ( rssi > maxRssi ) maxRssi = rssi;
    delay( deltaTms); 
  }
  return(maxRssi);
}

/*
  This is a function you can add to read the temperature
  from one of several devices:
  
  Arguments:
  pin (int) Analog pin number attached to sensor.
  sensType (int) Selects one of three sensor types.
  vRef (float) The arduino's supply voltage (5.0 for UNO)
*/

float readTemp( int pin, int sensType, float vRef ) {
  // sensType parameter vs. Temperature Sensor type:
  //   0  LM60
  //   1  MAX6605
  //   2  TMP36
  int mVoltsAtRefTemp[] = { 424, 744, 750 };
  int refTempC[] = { 0, 0, 25 };
  float mVperDegC[] = { 6.25, 11.9, 10.0 };

  int reading = analogRead( pin );
  float mVolts = reading * vRef / 1.024;

  return( ( mVolts - mVoltsAtRefTemp[sensType] ) / 
            ( mVperDegC[sensType] ) + 
            refTempC[sensType]);
  
}

void setFreq( float freqMHz ) {
  
    long freq = (long)( freqMHz / (27.0 / 65536) + 0.5);
    
    byte freq0= (byte)(freq&0xFF);
    byte freq1= (byte)((freq&0xFF00)>>8);
    byte freq2= (byte)((freq&0xFF0000)>>16);
    
    CC1101.WriteSingleReg( RADIO_FREQ0, freq0);
    CC1101.WriteSingleReg( RADIO_FREQ1, freq1);
    CC1101.WriteSingleReg( RADIO_FREQ2, freq2);
    delay(100);
  
}

/*
  Add this function to your sketch and call it whenever you want the
  amount of free bytes with:
  Serial.println( freeRam() );
*/

void resetWatchDog() {
  digitalWrite( watchDogPin, LOW );
  digitalWrite( watchDogPin, HIGH );
}
int freeRam() 
{
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
