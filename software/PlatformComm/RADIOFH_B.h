/* RADIO LIBRARY FOR TI RADIO MODUELS
   RADIO_CC110L.h - CC110L  module library
	  Author  : BHARAT UDDAGIRI
	  Version : 0.1
  	DATE    : MARCH 9, 2012
  	EMAIL   : UDDAGIRI@GMAIL.COM
    This library is designed to use CC110L module on Arduino platform.
	
*/

#ifndef _RADIOFH_H_B_INCLUDED
#define _RADIOFH_H_B_INCLUDED


#include <stdio.h>
#include <Arduino.h>
#include <pins_arduino.h>



//  RADIO READ WRITE INSTRUCTION BYTES
/****************************************************************/

#define 	READ_SINGLE            	0x80						//read single
#define 	READ_BURST      	      0xC0						//read burst

#define   WRITE_SINGLE            0x00            //Write single
#define 	WRITE_BURST     	      0x40						//write burst

#define 	BYTES_IN_TXFIFO         0x3F  				  //0x3F: Single byte access to TX FIFO
#define 	BURST_BYTES_IN_TXFIFO   0x7F            // 0x7F: Burst access to TX FIFO

#define 	BYTES_IN_RXFIFO         0xBF  				  //0xBF: Single byte access to RX FIFO
#define 	BURST_BYTES_IN_RXFIFO   0xFF  	       	//0xFF: Burst access to RX FIFO

/****************************************************************/










#define GDO0	2 // Pin Config Test 0
#define GDO2	9 // Pin Config Test 1
#define LNA_EN  8 // Pin Config LNA Enable for CC1190
#define PA_EN   7 // Pin Config PA Enable for CC1190

#ifndef CC1190
  #define RSSI_OFFSET 74
#else
  #define RSSI_OFFSET 81
#endif




//****************RADIO define***************************************//
// RADIO CONFIG REGSITER
#define RADIO_IOCFG2       0x00        // GDO2 output pin configuration
#define RADIO_IOCFG1       0x01        // GDO1 output pin configuration
#define RADIO_IOCFG0       0x02        // GDO0 output pin configuration
#define RADIO_FIFOTHR      0x03        // RX FIFO and TX FIFO thresholds
#define RADIO_SYNC1        0x04        // Sync word, high INT8U
#define RADIO_SYNC0        0x05        // Sync word, low INT8U
#define RADIO_PKTLEN       0x06        // Packet length
#define RADIO_PKTCTRL1     0x07        // Packet automation control
#define RADIO_PKTCTRL0     0x08        // Packet automation control
#define RADIO_ADDR         0x09        // Device address
#define RADIO_CHANNR       0x0A        // Channel number
#define RADIO_FSCTRL1      0x0B        // Frequency synthesizer control
#define RADIO_FSCTRL0      0x0C        // Frequency synthesizer control
#define RADIO_FREQ2        0x0D        // Frequency control word, high INT8U
#define RADIO_FREQ1        0x0E        // Frequency control word, middle INT8U
#define RADIO_FREQ0        0x0F        // Frequency control word, low INT8U
#define RADIO_MDMCFG4      0x10        // Modem configuration
#define RADIO_MDMCFG3      0x11        // Modem configuration
#define RADIO_MDMCFG2      0x12        // Modem configuration
#define RADIO_MDMCFG1      0x13        // Modem configuration
#define RADIO_MDMCFG0      0x14        // Modem configuration
#define RADIO_DEVIATN      0x15        // Modem deviation setting
#define RADIO_MCSM2        0x16        // Main Radio Control State Machine configuration
#define RADIO_MCSM1        0x17        // Main Radio Control State Machine configuration
#define RADIO_MCSM0        0x18        // Main Radio Control State Machine configuration
#define RADIO_FOCCFG       0x19        // Frequency Offset Compensation configuration
#define RADIO_BSCFG        0x1A        // Bit Synchronization configuration
#define RADIO_AGCCTRL2     0x1B        // AGC control
#define RADIO_AGCCTRL1     0x1C        // AGC control
#define RADIO_AGCCTRL0     0x1D        // AGC control
#define RADIO_WOREVT1      0x1E        // High INT8U Event 0 timeout
#define RADIO_WOREVT0      0x1F        // Low INT8U Event 0 timeout
#define RADIO_WORCTRL      0x20        // Wake On Radio control
#define RADIO_FREND1       0x21        // Front end RX configuration
#define RADIO_FREND0       0x22        // Front end TX configuration
#define RADIO_FSCAL3       0x23        // Frequency synthesizer calibration
#define RADIO_FSCAL2       0x24        // Frequency synthesizer calibration
#define RADIO_FSCAL1       0x25        // Frequency synthesizer calibration
#define RADIO_FSCAL0       0x26        // Frequency synthesizer calibration
#define RADIO_RCCTRL1      0x27        // RC oscillator configuration
#define RADIO_RCCTRL0      0x28        // RC oscillator configuration
#define RADIO_FSTEST       0x29        // Frequency synthesizer calibration control
#define RADIO_PTEST        0x2A        // Production test
#define RADIO_AGCTEST      0x2B        // AGC test
#define RADIO_TEST2        0x2C        // Various test settings
#define RADIO_TEST1        0x2D        // Various test settings
#define RADIO_TEST0        0x2E        // Various test settings



//  RADIO STROBE  INSTRUCTIONS
/**********************************************************************************************/


//RADIO Strobe commands
#define RADIO_SRES         0x30        // Reset chip.
#define RADIO_SFSTXON      0x31        // Enable and calibrate frequency synthesizer (if MCSM0.FS_AUTOCAL=1).
                                        // If in RX/TX: Go to a wait state where only the synthesizer is
                                        // running (for quick RX / TX turnaround).
#define RADIO_SXOFF        0x32        // Turn off crystal oscillator.
#define RADIO_SCAL         0x33        // Calibrate frequency synthesizer and turn it off
                                        // (enables quick start).
#define RADIO_SRX          0x34        // Enable RX. Perform calibration first if coming from IDLE and
                                        // MCSM0.FS_AUTOCAL=1.
#define RADIO_STX          0x35        // In IDLE state: Enable TX. Perform calibration first if
                                        // MCSM0.FS_AUTOCAL=1. If in RX state and CCA is enabled:
                                        // Only go to TX if channel is clear.
#define RADIO_SIDLE        0x36        // Exit RX / TX, turn off frequency synthesizer and exit
                                        // Wake-On-Radio mode if applicable.
#define RADIO_SAFC         0x37        // Perform AFC adjustment of the frequency synthesizer
#define RADIO_SWOR         0x38        // Start automatic RX polling sequence (Wake-on-Radio)
#define RADIO_SPWD         0x39        // Enter power down mode when CSn goes high.
#define RADIO_SFRX         0x3A        // Flush the RX FIFO buffer.
#define RADIO_SFTX         0x3B        // Flush the TX FIFO buffer.
#define RADIO_SWORRST      0x3C        // Reset real time clock.
#define RADIO_SNOP         0x3D        // No operation. May be used to pad strobe commands to two
                                        // INT8Us for simpler software.
                                        
                                        
/*************************************************************************************************/                                        
                                        
//RADIO STATUS REGSITER
#define RADIO_PARTNUM      0x30
#define RADIO_VERSION      0x31
#define RADIO_FREQEST      0x32

#define RADIO_LQI          0x33    //CC11OL NO EXIST
#define RADIO_CRC_REG      0x33 

#define RADIO_RSSI         0x34
#define RADIO_MARCSTATE    0x35
#define RADIO_WORTIME1     0x36      //CC11OL NO EXIST
#define RADIO_WORTIME0     0x37       //CC11OL NO EXIST
#define RADIO_PKTSTATUS    0x38
#define RADIO_VCO_VC_DAC   0x39         //CC11OL NO EXIST
#define RADIO_TXBYTES      0x3A
#define RADIO_RXBYTES      0x3B

//RADIO PATABLE,TXFIFO,RXFIFO
#define RADIO_PATABLE      0x3E
#define RADIO_TXFIFO       0x3F
#define RADIO_RXFIFO       0x3F



/*************************************************************************/
/************************************************************************/        










class RADIOClass {
public:
 
  void Reset(void);
  byte ReadSingleReg(byte addr);
  void ReadBurstReg(byte addr, byte *buffer, byte count);
  void WriteSingleReg(byte addr, byte value);
  void WriteBurstReg(byte addr, byte *buffer, byte count);
  int ReadRSSI(void);
  int ReadRSSI(int offset);
  void RegConfig(void); 
  void PATABLE(void);
 
 // SEND DATA FUNCTIONS
  void SendData(byte *txBuffer,byte size);
  void Strobe(byte strobe);
  void GDO_Set (void);
  
// RECIEVE DATA FUNCTIONS
  void SetReceive(void);
  byte CheckReceiveFlag(void);
  
  byte ReadStatusReg(byte addr) ;
  byte ReceiveData(byte *rxBuffer);
  
  
  
  
};

extern RADIOClass CC1101;



#endif
