/* Base frequency = 901.999649 */
/* Manchester enable = true */
/* Modulation format = GFSK */
/* Device address = 0 */
/* Packet length = 255 */
/* RX filter BW = 60.267857 */
/* Whitening = false */
/* CRC enable = true */
/* Data rate = 2.40111 */
/* Channel spacing = 26.367188 */
/* TX power = 26 */
/* Packet length mode = Variable packet length mode. Packet length configured by the first byte after sync word */
/* Data format = Normal mode */
/* Carrier frequency = 901.999649 */
/* Preamble count = 4 */
/* Deviation = 2.883911 */
/* Modulated = true */
/* CRC autoflush = false */
/* Sync word qualifier mode = 30/32 sync word bits detected */
/* Channel number = 0 */
/* PA ramping = false */
/* Address config = No address check */
/* PA table */
#define PA_TABLE {0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,}
const int REG_SIZE = 61;
byte Pkt_Buffer[REG_SIZE] = {0};
void RegConfig() {

  CC1101.WriteSingleReg( RADIO_IOCFG0,  0x06 );
  CC1101.WriteSingleReg( RADIO_FIFOTHR, 0x47 );
  CC1101.WriteSingleReg( RADIO_PKTLEN,  0x3D ); // Left out by SmartRF
  CC1101.WriteSingleReg( RADIO_PKTCTRL0,0x05 );
  CC1101.WriteSingleReg( RADIO_FSCTRL1, 0x06 );
  CC1101.WriteSingleReg( RADIO_FREQ2,   0x21 );
  CC1101.WriteSingleReg( RADIO_FREQ1,   0x68 );
  CC1101.WriteSingleReg( RADIO_FREQ0,   0x4B );
  CC1101.WriteSingleReg( RADIO_MDMCFG4, 0xF6 );
  CC1101.WriteSingleReg( RADIO_MDMCFG3, 0x75 );
  CC1101.WriteSingleReg( RADIO_MDMCFG2, 0x1B );
  CC1101.WriteSingleReg( RADIO_MDMCFG1, 0x20 );
  CC1101.WriteSingleReg( RADIO_MDMCFG0, 0x00 );
  CC1101.WriteSingleReg( RADIO_DEVIATN, 0x06 );
  CC1101.WriteSingleReg( RADIO_MCSM0,   0x18 );
  CC1101.WriteSingleReg( RADIO_FOCCFG,  0x16 );
  //CC1101.WriteSingleReg( RADIO_BSCFG,   0x1C ); Non-boosted only
  //CC1101.WriteSingleReg( RADIO_AGCCTRL2,0x47 );
  //CC1101.WriteSingleReg( RADIO_AGCCTRL0,0xB0 );
  //CC1101.WriteSingleReg( RADIO_WOREVT1, 0x00 );
  //CC1101.WriteSingleReg( RADIO_WOREVT0, 0x00 );
  //CC1101.WriteSingleReg( RADIO_FREND1,  0xB7 );
  CC1101.WriteSingleReg( RADIO_WORCTRL, 0xFB ); // Boosted only
  CC1101.WriteSingleReg( RADIO_FSCAL3,  0xE9 );
  CC1101.WriteSingleReg( RADIO_FSCAL2,  0x2A );
  CC1101.WriteSingleReg( RADIO_FSCAL1,  0x00 );
  CC1101.WriteSingleReg( RADIO_FSCAL0,  0x1F );
  //CC1101.WriteSingleReg( RADIO_RCCTRL1, 0x00 ); Non-boosted only
  CC1101.WriteSingleReg( RADIO_TEST2,   0x81 );
  CC1101.WriteSingleReg( RADIO_TEST1,   0x35 );
  CC1101.WriteSingleReg( RADIO_TEST0,   0x09 );

  byte paTable[8] = PA_TABLE;
  CC1101.WriteBurstReg( RADIO_PATABLE, paTable, 8 );

}
