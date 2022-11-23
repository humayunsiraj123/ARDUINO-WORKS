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
uint8_t MOSDisON=0x01;
uint8_t MOSChrgON=0x01;
uint16_t Vt_data;
float Vt;
uint16_t Current;
uint16_t SOC_data;
float SOC;
uint16_t max_cell_v_mv;
float max_cell_v;
uint16_t no_max_cell_v;
uint16_t min_cell_v_mv;
float min_cell_v;
uint16_t no_min_cell_v;
uint8_t max_temp;
uint8_t max_temp_cell_no;
uint8_t min_temp;
uint8_t min_temp_cell_no;
uint8_t state;
uint8_t charge_mos_state;
uint8_t discharge_mos_state;
uint8_t BMS_life;
uint32_t remaining_capacity_mah;
uint32_t test2;
uint32_t test3;
uint64_t test4;


float remaining_capacity_ah;
long unsigned ptime1;
long unsigned ptime2;
long unsigned c_time1;
long unsigned c_time2;
long unsigned rememberTime;
long unsigned rememberTime1;
int led ;
int mos ;
long unsigned int rxId;
uint8_t len = 0;
uint8_t i = 0;
uint8_t rxBuf[8];
uint8_t txBuf[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t DataIDs[9] = {0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98};
uint8_t txBuf1[8] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define CAN0_INT 2      // Set INT to pin 2
MCP_CAN CAN0(10);     // Set CS to pin 10

uint32_t makeCANID(uint8_t dataID, uint8_t BMS_addr, uint8_t PC_addr) {
  uint32_t CANid = 0x18;

  CANid = (CANid << 8) | dataID;
  CANid = (CANid << 8) | BMS_addr;
  CANid = (CANid << 8) | PC_addr;

  return CANid;
}

void mosdischarge(bool test){
  if(test==1){
  CAN0.sendMsgBuf(0x18DA0140,1,8,txBuf1); } 
  else if(test==0){
  CAN0.sendMsgBuf(0x18DA0140,1,8,txBuf); 
  } }
void moscharge(bool test){
  if(test==1){
  CAN0.sendMsgBuf(0x18D90140,1,8,txBuf1); } 
  else if(test==0){
  CAN0.sendMsgBuf(0x18D90140,1,8,txBuf); 
  } }
void most(bool A)
{
  if (A==1){
  digitalWrite(8,1);}
  else if(A==0){
   digitalWrite(8,0);
   }
    }
void setup()
{
  Serial.begin(115200);
pinMode(7,INPUT_PULLUP);
pinMode(8,OUTPUT);
  if (CAN0.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ) == CAN_OK) Serial.println("MCP2515 Initialized Successfully!");
  else Serial.println("Error Initializing MCP2515...");

  CAN0.setMode(MCP_NORMAL);   // Change to normal mode to allow messages to be transmitted
moscharge(true);
mosdischarge(true);

}



void loop()
{
  c_time1=millis();
  c_time2=millis();
  uint32_t canid = makeCANID(DataIDs[i], 0x01, 0x40);
  i = (i < 8) ? ++i : 0;
  
// send data:  ID = canid, Extended CAN Frame, Data length = 8 bytes, 'txbuf' = array of data bytes to send
  byte sndStat = CAN0.sendMsgBuf(canid, 1, 8, txBuf);

  /*else{
    //bms.setDischargeMOS(false);
    Serial.println("MOS ON FLAGG 00000000");
}*/
  if (sndStat == CAN_OK) {
    //Serial.print("Tx ID: ");
    //Serial.println(canid, HEX);
  }
  delay(1000);   // send data per 100ms

  if (!digitalRead(CAN0_INT))                        // If CAN0_INT pin is low, read receive buffer
  {
    CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)

    //Serial.print("Rx ID: ");
    //Serial.print((rxId & 0x1FFFFFFFUL), HEX);
    //Serial.print("  ");
    //Serial.print(", dlc: ");
    //Serial.print(len, HEX);
    //Serial.print(", data: ");

    for (byte i = 0; i < len; i++) {
      //Serial.print(rxBuf[i], HEX);
      //Serial.print(" ");
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
    if(rxId == 2559590401){                 //from data id 0x90
      Vt_data=(byte_1 << 8) | byte_2;
      Vt=float(Vt_data)*0.10;
      Serial.print("\nTotal Voltage : ");
      Serial.print(Vt);
      Serial.print(" v");
      
      Current=(byte_5 << 8) | byte_6;
      Current=Current-30000;
      Serial.print("\nCurrent : ");
      Serial.print(Current);
      Serial.print(" A");

      SOC_data=(byte_7 <<8) | byte_8;
      SOC=float(SOC_data)*0.1;
      Serial.print("\nSOC : ");
      Serial.print(SOC);
      Serial.print("%");
    }
    
    if(rxId == 2559655937){                    //from data id 0x91
      max_cell_v_mv=(byte_1 << 8) | byte_2;
      max_cell_v=float(max_cell_v_mv)/1000.00;
      Serial.print("\nMaximum Cell Voltage : ");
      Serial.print(max_cell_v);
      Serial.print(" v");

      no_max_cell_v=byte_3;
      Serial.print("\nNo of Cells with Maximum Voltage : ");
      Serial.print(no_max_cell_v);
      
      min_cell_v_mv=(byte_4 << 8) | byte_5;
      min_cell_v=float(min_cell_v_mv)/1000.00;
      Serial.print("\nMinimum Cell Voltage : ");
      Serial.print(min_cell_v);
      Serial.print(" v");
      no_min_cell_v=byte_6;
      Serial.print("\nNo of Cells with Minimum Voltage : ");
      Serial.print(no_min_cell_v);
    }
    
    if(rxId == 2559721473){                   //from data id 0x92

      max_temp=byte_1;
      max_temp=max_temp-40;
      Serial.print("\nMaximum Cell Temperature : ");
      Serial.print(max_temp);
      Serial.print(" °C");

      max_temp_cell_no=byte_2;
      Serial.print("\nMaximum Temperature Cell N0 : ");
      Serial.print(max_temp_cell_no);

      min_temp=byte_3;
      min_temp=min_temp-40;
      Serial.print("\nMinimum Cell Temperature : ");
      Serial.print(min_temp);
      Serial.print(" °C");

      min_temp_cell_no=byte_4;
      Serial.print("\nMaximum Temperature Cell N0 : ");
      Serial.print(min_temp_cell_no);
      
    }

    if(rxId == 2559787009){                            //from data id 0x93

      state=byte_1;
      if(state == 0){
        Serial.print("\nState : Stationary");
        }
      else if(state == 1){
        Serial.print("\nState : Charging");
        }
      else if(state == 2){
        Serial.print("\nState : Discharging");
        }
      charge_mos_state=byte_2;
      Serial.print("\nCharge_MOS_State : ");
      Serial.print(charge_mos_state);
      discharge_mos_state=byte_3;
      Serial.print("\nDischarge_MOS_State : ");
      Serial.print(discharge_mos_state);
      BMS_life=byte_4;
      Serial.print("\nCycles : ");
      Serial.print(BMS_life);
      remaining_capacity_mah=(byte_5 << 24) | (byte_6 << 16) | (byte_7 << 8) | (byte_8);
      remaining_capacity_ah=float(remaining_capacity_mah)/1000.00;
      Serial.print("\nremaining_capacity : ");
      Serial.print(remaining_capacity_ah);
      }
      
  }
    Serial.println();
  
  
  if(Serial.available()>0){
  String a = Serial.readString();
  a.trim();
  Serial.print("8888888888888888888888888");
  Serial.println(a);
if((a)=="a"){
  
   //bms.setChargeMOS(true);
   mosdischarge(true);
   Serial.println("MOS chargeON");
   }
 if((a)=="s"){
   //bms.setChargeMOS(false);
   mosdischarge(false);
   Serial.println("MOS chargeOFF");
   }
  
 if((a)=="d"){
   //bms.setDischargeMOS(true);
   moscharge(true);
   Serial.println("MOS chargeON");
   }
 if((a)=="f"){moscharge(true);
   //bms.setDischargeMOS(false);
   moscharge(false);
   Serial.println("MOS chargeOFF");
   }
  if((a)=="z"){
   //bms.setBmsReset();
   Serial.println("MOS chargeOFF");
   }
else{
    Serial.println("not enterde-------------------------------------------------------------------------------");}
  

}

//CONTROL THROUGH BUTTONN
if(mos==HIGH)
{
  if( (millis()- rememberTime) >= 1000){   
    rememberTime=millis();// remember Current millis() time
    CAN0.readMsgBuf(&rxId, &len, rxBuf);
    if(rxId==117965316)
    byte c=0;
    byte_1=rxBuf[c];
    byte_2=rxBuf[c+1];
    byte_3=rxBuf[c+2];
    byte_4=rxBuf[c+3];
    byte_5=rxBuf[c+4];
    byte_6=rxBuf[c+5];
    byte_7=rxBuf[c+6];
    byte_8=rxBuf[c+7];
    
    led=digitalRead(7);
    
    Serial.println("check 111111111111");
    
    if(rxId==117965316){
      test2=(byte_5 << 24) | (byte_6 << 16) | (byte_7 << 8) | (byte_8);
      test3=(byte_1 << 24) | (byte_2 << 16) | (byte_3 << 8) | (byte_4);
      test4=test2+test3;
      
      if (test4==(101190156)){
      digitalWrite(8,1);
    moscharge(true);
    mosdischarge(true);
    Serial.println("MOS ON");
    mos=LOW;
    }
    else if (test4!=(101190156)){
      digitalWrite(8,0);
      moscharge(false);mosdischarge(false);
      Serial.println("MOS OFF");
    }}


    
    
    //Serial.println(led);
    //LEDState = LOW;// change the state of LE 
    }
    
 }

 if( (millis()- rememberTime) >= 5000){   
    rememberTime=millis();// remember Current millis() time
    //led=digitalRead(7);
    Serial.println("check 2222222222222222");
    if (mos==LOW){
      //digitalWrite(8,1);
    mos=HIGH;
    }
    else if(mos==HIGH){
      //digitalWrite(8,1);
    mos=LOW;
    }
      //digitalWrite(8,0);
    }}
    


/*



//flag1=digitalRead(1);
  if((c_time1-ptime1>=5000)&&mos==false){
    ptime1=c_time1;
    //flag1=digitalRead(7);
    if(digitalRead(7)==0 &&
    ){
      
    
   //bms.setDischargeMOS(true);
   mosdischarge(true);
   moscharge(true);
   most(true);
   mos=true;
   Serial.println("MOS ON true");}
  Serial.println("MOS ON FLAGG 11111111111");
  
  }

   else
    {mos=false;
    Serial.println("MOS ON false");} 
 //else{}  

if((c_time2-ptime2>10000)&&(mos==false)){
  //bms.setDischargeMos(false);}
    ptime2=c_time2;
    mosdischarge(false);
    moscharge(false);
    most(false);
    
    Serial.println("MOS off false");
    if(mos==false){
      mos=true;
      Serial.println("MOS true");}
    else if(mos==true){
      mos=false;
      Serial.println("MOS false");}
      
    //mosdischarge(false);
    /*flag2 =0;
    Serial.println("*************");
     Serial.print("flag1 =" );
   Serial.println(flag1);
    flag2=1;
    Serial.print("flag2 =" );
   Serial.println(flag2);}*/

    
  /* Serial.print("flag1 =" );
   Serial.println(flag1);
    flag2=1;
    Serial.print("flag2  =" );
   Serial.println(flag2);*/

  /*else{
    //bms.setDischargeMOS(false);
    mosdischarge(false);
    moscharge(false);
   Serial.println("MOS ON FLAGG 00000000");
*/
    
    /*Serial.println("000000000000");
    Serial.println(ptime);
    Serial.println(c_time);
    while(c_time-ptime<20000){
      delay(1000);
      c_time==millis();
      Serial.print("c_time  ");
   
    Serial.println(c_time);}
    c_time= 0;
  */  
    
    
    /*
    //else{
      //}
      flag1=digitalRead(D4);
  if(flag1==0){
//   bms.setDischargeMOS(true);
    moscharge(true);
  Serial.println("MOS ON FLAGG 11111111111");}}
 // flag}
  */
  
