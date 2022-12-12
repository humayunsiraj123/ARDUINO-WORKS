// demo: CAN-BUS Shield, send data
#include <mcp_can.h>
#include <SPI.h>
#include <PZEM004Tv30.h>

//Pot for adjusting value
int sensorPin = A0;
int sensorValue = 0;
int cantxValue = 0;


//Some sample CAN messages
unsigned char msg1[8] = {0, 1, 2, 3, 4, 5, 6, 7};
unsigned char msg2[8] = {0xFF, 0x01, 0x10, 0x0A, 0x00, 0x00, 0x00, 0x00};
unsigned char msg3[4] = {0xFF, 0x01, 0x10, 0x0A};
uint8_t rxBuf[8];
uint8_t txBuf[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t DataIDs[9] = {0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98};
uint8_t txBuf1[8] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#define CAN1_INT 2      // Set INT to pin 2
MCP_CAN CAN1(10);
PZEM004Tv30 pzem(11, 12);


uint32_t makeCANID(uint8_t dataID, uint8_t BMS_addr, uint8_t PC_addr) {
  uint32_t CANid = 0x07;

  CANid = (CANid << 8) | dataID;
  CANid = (CANid << 8) | BMS_addr;
  CANid = (CANid << 8) | PC_addr;

  return CANid;
}

void setup()
{
  Serial.begin(115200);
  // init can bus, baudrate: 100k
  if (CAN1.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ) == CAN_OK) Serial.println("MCP2515 Initialized Successfully!");
  else Serial.println("Error Initializing MCP2515...");
  
uint32_t canid = makeCANID(0x08, 0x02, 0x04);

  
  
}



void loop()

{byte sndStat = CAN1.sendMsgBuf(canid, 1, 8, txBuf);
  if (sndStat == CAN_OK) {
    Serial.print("Tx ID: ");
    Serial.println(canid, HEX);

    
  }
  //Read the value of the pot
  sensorValue = analogRead(sensorPin);
  //Each CAN bus byte can store a value between 0-255. 
  //Dividing sensorValue by 4 puts us in that range.
  cantxValue = sensorValue / 4;
  Serial.print("cantxValue: ");
  Serial.print(cantxValue);
  Serial.println();
  //Create data packet for CAN message
  unsigned char canMsg[8] = {cantxValue, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAB};
  // send data:  id = 0x123, standrad flame, data len = 8, stmp: data buf
  CAN1.sendMsgBuf(0x07B, 1, 8, canMsg);  
  delay(100);
}
