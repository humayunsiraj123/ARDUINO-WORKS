#include <mcp_can.h>
#include <SPI.h>
bool flag1 =0;
bool flag2 =0;
byte byte_1;
byte byte_2;
byte byte_3;
byte byte_4;
byte byte_5;
byte byte_6;
byte byte_7;
byte byte_8;

uint32_t rpm_id = 0x1E001000;
uint32_t tx_rpm_id=0x00000280;
uint32_t tx_oil_id=0x000004E0;
//uint32_t tx_rpm_id=0x00000280;

uint32_t oil_id = 0x1E025000;
int rpm ; 
float oil_psi;
uint8_t rxBuf[8];
uint8_t txBuf[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t DataIDs[9] = {0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98};
uint8_t txBuf1[8] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//
//index Description Units CAN data bytes CAN ID (hex)
//0 RTC (1/1000 sec since power on) xx:xx:xx.xx time [0..3] 1E001000
//0 RPM xx,xxx RPM [4..7] 1E001000
//Barometric Pressure xxx.x kPa [0..3] 1E021000
//8 Coolant Temperature (CTS) xxx F [4..7] 1E021000
//Oil Pressure xxx psi [0..3] 1E025000
//9 Battery Voltage xx.x volts [4..7] 1E025000

//HEFI 3rd Party Communications Protocol
//Holley Performance Products – For public release 2
//For example,
//- CANID 0x1E001000: 00 00 00 00 00 1d 4c 00
//rpm data[4]=0x00, data[5]=0x1d, data[6]=0x4c, data[7]=00. Hex 0x001d4c00 =decimal
//1920000/256 = 7500.0 RPM
//- CANID 0x1E021000: 00 00 64 f8 00 00 82 80
//barometric pressure = 0x000064f8 = decimal 25848/256 = 100.97
//coolant temp = 0x00008280 = decimal 33408/256 = 130.5
//- CANID 0x1E025000: 00 00 0a 40 00 00 0d cc
//oil pressure = 0x00000a40 = decimal 2624/256 = 10.25
//battery voltage = 0x00000dcc = decimal 3532/256 = 13.79
#define CAN0_INT 2      // Set INT to pin 2
MCP_CAN CAN0(10);     // Set CS to pin 10


union data_u { // overlay CAN payload with Long intgeger
unsigned char data_array[4]; // 8 byte payload area to the CAN BUS
unsigned long int data;
};

void setup() {
 
  Serial.begin(115200);
pinMode(7,INPUT_PULLUP);
pinMode(8,OUTPUT);
  while (CAN0.begin(MCP_ANY, CAN_1000KBPS, MCP_8MHZ) != CAN_OK){
    Serial.println("CAN NOT CONNECTED");
    delay(100);
    } 
   
 if(CAN0.begin(MCP_ANY, CAN_1000KBPS, MCP_8MHZ) != CAN_OK) 
  {
     Serial.println("MCP2515 Initialized Successfully!");}

  else Serial.println("Error Initializing MCP2515...");
  CAN0.setMode(MCP_NORMAL);   // Change to normal mode to allow messages to be transmitted

}

void loop() {
  // put your main code here, to run repeatedly:
// send data:  ID = canid, Extended CAN Frame, Data length = 8 bytes, 'txbuf' = array of data bytes to send
  //read_rmp
//  byte sndStat = CAN0.sendMsgBuf(rpm_id, 1, 8, txBuf);
//  if (sndStat == CAN_OK) {
//    Serial.print("Tx ID: ");
//    Serial.println(rpm_id, HEX);
//  }

  if (!digitalRead(CAN0_INT))                        // If CAN0_INT pin is low, read receive buffer
  {
  
    CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)
    union data_u rx_data;
    rxId = rx_Id & 0x7FFFF000; 
    
    for (byte i = 0; i < len; i++) {
      Serial.print(rxBuf[i], HEX);
      Serial.print(" ");
    }
    
    switch ( rxId ) {
    case rmp_id :
    rx_data.data_array[0] = rxBuf[7];
    rx_data.data_array[1] = rxBuf[6];
    rx_data.data_array[2] = rxBuf[5];
    rx_data.data_array[3] = rxBuf[4];
    rpm = (int)(rx_data.data/256);
    Serial.print("RPM  = ");
    Serial.println(rpm); 
    tx_Buf[0]=0x00;
    tx_Buf[1]=0x00;
    tx_Buf[2]= (unsigned char)((rpm *4) & 0x00FF);//low byte
    tx_Buf[3]=(unsigned char)((rpm *4) /256);//high byte
    tx_Buf[4]=0x00;
    tx_Buf[5]=0x00;
    tx_Buf[6]=0x00;
    tx_Buf[7]=0x00;
    
    byte sndStat = CAN0.sendMsgBuf(tx_rpm_id, 1, 8, txBuf);
 if (sndStat == CAN_OK) {
    Serial.print("Tx ID: ");
    Serial.println(rpm_id, HEX);
  }
    
   break;
    case oil_id:
    
    rx_data.data_array[0] = rxBuf[3];
    rx_data.data_array[1] = rxBuf[2];
    rx_data.data_array[2] = rxBuf[1];
    rx_data.data_array[3] = rxBuf[0];
    oil_psi = (float)(rx_data.data/256.0);
    Serial.print("Oil pressure = ");
    Serial.println(oil_psi);

     tx_Buf[0]=0x00;
    tx_Buf[1]=0x00;
    tx_Buf[2]= (unsigned char)((rpm *4) & 0x00FF);//low byte
    tx_Buf[3]=(unsigned char)((rpm *4) /256);//high byte
    tx_Buf[4]=0x00;
    tx_Buf[5]=0x00;
    tx_Buf[6]=0x00;
    tx_Buf[7]=0x00;
    

   break;
    default: // debug printing to serial
return;
break;

  }
}
}
