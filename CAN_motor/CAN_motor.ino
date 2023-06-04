#include <mcp_can.h>
#include <SPI.h>

#define CAN0_INT 2      // Set INT to pin 2
MCP_CAN CAN0(10);     // Set CS to pin 10
const int ENA_PIN = 5;   // Enable pin
const int IN1_PIN = 2;  // Input 1 pin
const int IN2_PIN = 3;  // Input 2 pin
uint16_t state_command = 0x9D; //temp and pahse current read;
uint32_t send_com =0x114;
bool dir =0;
long unsigned int rcv_com ;//=0x214;
byte id_ex[7] ={0x30,0x60,0x61,0x9A,0x71,0x70,0x92};//
uint8_t len = 0;
byte byte_1;
byte byte_2;
byte byte_3;
byte byte_4;
byte byte_5;
byte byte_6;
byte byte_7;
byte byte_8;
uint8_t Data[8] ;
uint8_t rxBuf[8];
int cnt=0;
uint8_t txBuf[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
long unsigned int rxId;

//uint8_t CurrKP; 
//uint8_t CurrKI;
//uint8_t SpdKP 
//uint8_t SpdKI; 
//uint8_t PosKP; 
//uint8_t PosKI; 


float CurrKP; 
float CurrKI;
float SpdKP ;
float SpdKI; 
float PosKP; 
float PosKI; 

unsigned long int prev;
unsigned long int curr;
unsigned long int prev1;
unsigned long int curr1;
 

int32_t encoderPosition;

void setup() {
  Serial.begin(115200);
  pinMode(ENA_PIN, OUTPUT);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  
  // put your setup code here, to run once:
if (CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) Serial.println("MCP2515 Initialized Successfully!");
  else Serial.println("Error Initializing MCP2515...");

  CAN0.setMode(MCP_NORMAL);
SetBaudRate(0xB4,0x00);  
}

void SetBaudRate(uint16_t canid, uint8_t baudrate)
{
  uint8_t txBuf[8] = {0xB4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, baudrate};

  byte sndStat = CAN0.sendMsgBuf(canid, 0, 8, txBuf);

  if (sndStat == CAN_OK)
  {
    Serial.print("Tx ID: 0x");
    Serial.println(canid, HEX);
    Serial.print("Baud Rate Set to: ");
    if (baudrate == 0)
    {
      Serial.println("500 Kbps");
    }
    else if (baudrate == 1)
    {
      Serial.println("1 Mbps");
    }
    else
    {
      Serial.println("Invalid baud rate");
    }
  }
  else
  {
    Serial.println("Failed to send baud rate command");
  }
}

void StopMotor(uint16_t canid)
{
  uint8_t txBuf[8] = {0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  byte sndStat = CAN0.sendMsgBuf(canid, 0, 8, txBuf);

  if (sndStat == CAN_OK)
  {
    Serial.print("Tx ID: 0x");
    Serial.println(canid, HEX);
    Serial.println("Motor Stopped");
  }
  else
  {
    Serial.println("Failed to send motor stop command");
  }
}

void Send(uint16_t canid, uint8_t fbyte){  
txBuf[0]=fbyte;
   byte sndStat = CAN0.sendMsgBuf(canid, 1, 8, txBuf);
   for (int i = 0; i < 8; i++)
  {
    Serial.print("the tx value at index ");
    Serial.print(i);
    Serial.print(" is ");
    Serial.println(txBuf[i], HEX);
  }
    if (sndStat == CAN_OK) {
    //Serial.print("Tx ID: ");
    //Serial.println(canid, HEX);
  }
  //delay(500);   // send data per 100ms
//txBuf[0]=0x00;


                    // If CAN0_INT pin is low, read receive buffer
  
    CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)
//
//    Serial.print("Rx ID: ");
//    Serial.print((rxId & 0x1FFFFFFFUL), HEX);
//    Serial.print("  ");
//    Serial.print(", dlc: ");
//    Serial.print(len, HEX);
//    Serial.print(", data: ");

    for (byte i = 0; i < len; i++) {
      Serial.print(rxBuf[i], HEX);
      Serial.print(" ");
    }
    //Serial.print("\n");
    //Serial.print(rxId);
    byte c=0;
    byte_1=rxBuf[c];
    byte_2=rxBuf[c+1];
    byte_3=rxBuf[c+2];
    byte_4=rxBuf[c+3];
    byte_5=rxBuf[c+4];
    byte_6=rxBuf[c+5];
    byte_7=rxBuf[c+6];
    byte_8=rxBuf[c+7];

    
  
  //Data=rxBuf;

  if(rxId==0x214){
    
  //switch(rxBuf[0]):
  if(rxBuf[0]==0x40){
     CurrKP =(0.1/255)*rxBuf[2];
    Serial.print("the value of CurrKp ");
    Serial.println(CurrKP);
    
 CurrKI=(0.1/255)*rxBuf[3];
 Serial.print("the value of CurrKI ");
    Serial.println(CurrKI);
    
 SpdKP =(0.1/255)*rxBuf[4];
Serial.print("the value of SPdKp ");
    Serial.println(SpdKP);
    
SpdKI=(0.1/255)*rxBuf[5];
Serial.print("the value of SPdKi ");
    Serial.println(SpdKI);
    
PosKP = (0.1/255)*rxBuf[6]; 
Serial.print("the value of posKp ");
    Serial.println(PosKP);
    
PosKI=(0.1/255)*rxBuf[7];

 Serial.print("the value of PosKI ");
    Serial.println(PosKI);
    

    
    }
  }
  }


  void Send_main(byte canid){

byte tx[8];
  tx[0] = canid;
  tx[1] = 0x00;
  tx[2] = 0x00;
  tx[3] = 0x00;
  tx[4] = 0x00;
  tx[5] = 0x00;
  tx[6] = 0x00;
  tx[7] = 0x00;

  byte sndStat = CAN0.sendMsgBuf(send_com, 1, 8, tx);

  for (int i = 0; i < 8; i++)
  {
    Serial.print("the tx value at index ");
    Serial.print(i);
    Serial.print(" is ");
    Serial.println(tx[i], HEX);
  }

  if (sndStat == CAN_OK)
  {
    Serial.print("Tx ID: ");
    Serial.println(canid, HEX);
  }

  //delay(500);

  CAN0.readMsgBuf(&rcv_com, &len, rxBuf);

  for (byte i = 0; i < len; i++)
  {
    Serial.print("Rx data at index ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(rxBuf[i], HEX);
  }

  if (rxId == 0x241)
  {
    if(rxBuf[0]==canid && rxBuf[0]==0x92){//
 
    int32_t motorAngle = ((int32_t)rxBuf[7] << 24) | ((int32_t)rxBuf[6] << 16) | ((int32_t)rxBuf[5] << 8) | rxBuf[4];
    float angleDegrees = motorAngle * 0.01;
    Serial.print("Motor Output Shaft Angle: ");
    Serial.print(angleDegrees);
    Serial.println(" degrees");

    // Perform operations with the motor output shaft angle data here
  }

  else if(rxBuf[0]==0x60){
    encoderPosition = ((int32_t)rxBuf[7] << 24) | ((int32_t)rxBuf[6] << 16) | ((int32_t)rxBuf[5] << 8) | rxBuf[4];
    Serial.print("Encoder Position: ");
    Serial.println(encoderPosition);

    // Perform calculations with the encoder position here
    // Subtract the encoder position from the original position, etc.
  }
  else if(rxBuf[0]==0x61)
   {
    int32_t encoderRaw = ((int32_t)rxBuf[7] << 24) | ((int32_t)rxBuf[6] << 16) | ((int32_t)rxBuf[5] << 8) | rxBuf[4];
    Serial.print("Encoder Original Position: ");
    Serial.println(encoderRaw);

    // Perform operations with the encoder original position data here
  }
  else if(rxBuf[0==0x30]){
   float CurrKP, CurrKI, SpdKP, SpdKI, PosKP, PosKI; 
     CurrKP =(0.1/255)*Data[2];
    Serial.print("the value of CurrKp ");
    Serial.println(CurrKP);
    
 CurrKI=(0.1/255)*Data[3];
 Serial.print("the value of CurrKI ");
    Serial.println(CurrKI);
    
 SpdKP =(0.1/255)*Data[4];
Serial.print("the value of SPdKp ");
    Serial.println(SpdKP);
    
SpdKI=(0.1/255)*Data[5];
Serial.print("the value of SPdKi ");
    Serial.println(SpdKI);
    
PosKP = (0.1/255)*Data[6]; 
Serial.print("the value of posKp ");
    Serial.println(PosKP);
    
PosKI=(0.1/255)*Data[7];

 Serial.print("the value of PosKI ");
    Serial.println(PosKI);
    
    }
  else if(rxBuf[0==0x9A]){
    int8_t temperature = rxBuf[1];
    uint8_t holdingBrakeStatus = rxBuf[3];
    uint16_t voltage = (rxBuf[5] << 8) | rxBuf[4];
    uint16_t errorState = (rxBuf[7] << 8) | rxBuf[6];

    Serial.print("Motor Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    Serial.print("Holding Brake Status: ");
    if (holdingBrakeStatus == 1)
    {
      Serial.println("Release");
    }
    else
    {
      Serial.println("Lock");
    }

    Serial.print("Voltage: ");
    Serial.print(voltage * 0.1);
    Serial.println(" V");

    Serial.print("Error State: ");
    if (errorState & 0x0002)
    {
      Serial.println("Motor Stall");
    }
    if (errorState & 0x0004)
    {
      Serial.println("Low Pressure");
    }
    if (errorState & 0x0008)
    {
      Serial.println("Overvoltage");
    }
   
  }

  
  else if(rxBuf[0]==0x70){
    
      uint8_t motorMode = rxBuf[7];

      Serial.print("Motor Operation Mode: 0x");
      Serial.println(motorMode, HEX);

      if (motorMode == 0x01)
      {
        Serial.println("Current loop mode");
      }
      else if (motorMode == 0x02)
      {
        Serial.println("Speed loop mode");
      }
      else if (motorMode == 0x03)
      {
        Serial.println("Position loop mode");
      }
    }
    else if(rxBuf[0]==0x71){
       uint16_t motorPower = (rxBuf[7] << 8) | rxBuf[6];
      float powerInWatts = motorPower * 0.1;

      Serial.print("Motor Power: ");
      Serial.print(powerInWatts);
      Serial.println(" W");
    }
    }
  }
    
  




  void Encode_pos(uint16_t canid)
    {
  byte sndStat = CAN0.sendMsgBuf(canid, 0, 8, txBuf);

  for (int i = 0; i < 8; i++)
  {
    Serial.print("the tx value at index ");
    Serial.print(i);
    Serial.print(" is ");
    Serial.println(txBuf[i], HEX);
  }

  if (sndStat == CAN_OK)
  {
    Serial.print("Tx ID: ");
    Serial.println(canid, HEX);
  }

  delay(500);

  CAN0.readMsgBuf(&rxId, &len, rxBuf);

  for (byte i = 0; i < len; i++)
  {
    Serial.print("Rx data at index ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(rxBuf[i], HEX);
  }

  if (rxId == 0x241)
  { if(rxBuf[0]==0x60){
    encoderPosition = ((int32_t)rxBuf[7] << 24) | ((int32_t)rxBuf[6] << 16) | ((int32_t)rxBuf[5] << 8) | rxBuf[4];
    Serial.print("Encoder Position: ");
    Serial.println(encoderPosition);

  }
  else if(rxBuf[0]==0x61)
   {
    int32_t encoderRaw = ((int32_t)rxBuf[7] << 24) | ((int32_t)rxBuf[6] << 16) | ((int32_t)rxBuf[5] << 8) | rxBuf[4];
    Serial.print("Encoder Original Position: ");
    Serial.println(encoderRaw);

  
  }
}}

void forward(int duty) {
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);
  analogWrite(ENA_PIN, (2.5*duty));  // Set the speed (0-255)
}

// Functioto run the motor in counter-clockwise direction
void backward(int duty) {
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, HIGH);
  analogWrite(ENA_PIN, (2.5*duty));  // Set the speed (0-255)
}

// Function to stop the motor
void stopMotor() {
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  analogWrite(ENA_PIN, 0);  // Set the speed to 0
}

void loop() {
  // put your main code here, to run repeatedly:
//  for(int i=0;i<255;i=i+10){
//    analogWrite(i)}
// digitalWrite(input1Pin,1);
curr=millis();
curr1=millis();
if(curr-prev==5){
  prev=curr;
  if(dir==0){
  dir=1;
  forward(50);}
  else if(dir==1){
  dir=0;
  backward(50);}
  }
  
  
   

if(curr1-prev1==1000){
  prev1=curr1;
  cnt=cnt<7?++cnt:0;
     Send_main(id_ex[cnt]);  
}}
