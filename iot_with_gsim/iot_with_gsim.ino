#include <SoftwareSerial.h>



#define GSM_RX_PIN 10
#define GSM_TX_PIN 11
int _timeout;
String _buffer;
String number = "+923162725716"; // mobile no to call
bool i_1,i_2,i_3,i_4,i_5;
int pin[5]={2,3,4,5,6};
int relay = 12;
int reset_button=8;//
bool interupt1_state =0;
bool interupt2_state=0;
bool interupt3_state =0;
bool interupt4_state=0;
bool interupt5_state=0;
bool flag;// for interutp indication
uint16_t prev_time ;
uint16_t curr_time ;
uint16_t d_time=1000;
uint16_t last_time ;
//uint16_t curr_time ;
//uint16_t d_time=1000;
int  last_state=0;
String command;
String command_s;

int state;
SoftwareSerial sim(GSM_RX_PIN,GSM_TX_PIN );
void setup() {
  // put your setup code here, to run once:
for(int i =0;i<5;i++){
pinMode(pin[i],INPUT_PULLUP);
//attachInterrupt(digitalPinToInterrupt(pin[i]),Interuptcall(pin[i]),CHANGE);
}
pinMode(relay,OUTPUT);
pinMode(reset_button,INPUT_PULLUP);
 Serial.begin(115200);
  _buffer.reserve(50);
  Serial.println("System Started...");
  sim.begin(115200);
  delay(1000);
//delay(2000);
}
// function to send message on mobile from gsm
//void gsmMsg(const String& message) {
//  gsmSerial.println("AT+CMGF=1"); // Set SMS 
//  delay(10);
//  gsmSerial.println("AT+CMGS=\"+1234567890\""); //mobile on
//  delay(10);
//  gsmSerial.println(message);
//  delay(10);
//  gsmSerial.write(26); // End of message character
//}
void gsmMsg(const String& message)
{
  //Serial.println ("Sending Message");
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(50);
  //Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(50);
  String SMS = "Interpt Recived at  " + String(message);
  sim.println(SMS);
  delay(50);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(50);
  _buffer = _readSerial();
}

void Msg(const String& message)
{
  //Serial.println ("Sending Message");
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(50);
  //Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(50);
  String SMS =  String(message);
  sim.println(SMS);
  delay(50);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(50);
  _buffer = _readSerial();
}
String _readSerial() {
  _timeout = 0;
  while  (!sim.available() && _timeout < 12000  )
  {
    delay(13);
    _timeout++;
  }
  if (sim.available()) {
    return sim.readString();
  }
}

void recv_msg()
{
  Serial.println ("SIM800L Read an SMS");
  sim.println("AT+CMGF=1");
  delay (50);
  sim.println("AT+CNMI=1,2,0,0,0"); // AT Command to receive a live SMS
  delay(50);
  Serial.write ("Unread Message done");
}

void callNumber() {
  sim.print (F("ATD"));
  sim.print (number);
  sim.print (F(";\r\n"));
  _buffer = _readSerial();
  Serial.println(_buffer);
}

// interupt function for interupt detection and 
void Interuptcall(int interupt_no){
  switch(interupt_no){
  case 2:interupt1_state = 1;
    sendmsg(1);
    gsmMsg(" port 1");
    //gsmCall();
    break;
  case 3:interupt2_state = 1;
    sendmsg(2);
    gsmMsg(" port 2");
    //gsmCall();
    break;
  case 4:interupt3_state = 1;
    sendmsg(3);
    gsmMsg(" port 3");
    //gsmCall();
    break;
  case 5:interupt4_state = 1;
    sendmsg(4);
    gsmMsg(" port 4");
    //gsmCall();
    break;
  case 6:interupt5_state = 1;
    sendmsg(5);
    gsmMsg(" port 5");
    //gsmCall();
    break;

  case 10:
    interupt5_state = 0;
    interupt4_state = 0;
    interupt3_state = 0;
    interupt2_state = 0;
    interupt1_state = 0;
    i_1=0;
    i_2=0;
    i_3=0;
    i_4=0;
    i_5=0;
    sendmsg(10);
    Msg("Relay Deactivated");   
    break;
  }
}

// to display interupt detection on remote site i.e serial monitor
void sendmsg(int pin_no){
  
  String txt =(pin_no!=10) ? "Interupt Call on Port no " + String(pin_no) : " Reset call Removing Interupt" ;   
  
  Serial.println("call on 911");
  Serial.println(txt);
  
  }

//void gsmCall() {// gsm module call on mobile fuctnion
//  gsmSerial.println("ATD+1234567890;"); // phone number
//}



//void sendResetNotification(const String& location) {
//  gsmSerial.println("AT+CMGF=1"); // Set SMS text mode
//  delay(100);
//  gsmSerial.println("AT+CMGS=\"+1234567890\""); // Replace with recipient's phone number
//  delay(100);
//  gsmSerial.print("Reset notification\n");
//  gsmSerial.print(location);
//  gsmSerial.write(26); // End of message character
//}



void relay_control(bool value){
  digitalWrite(relay,value);
  }



//  String getGSMTimestamp(){
//  boolean notConnected = true;
//  
//  return println("AT+CCLK?",1000);
//}





void relay_call(bool states){
  if(states==1){
  Serial.println("Relay Active Interupt  Recived");
  relay_control(states);
  }
  if(states==0){
    
  Serial.println("Relay Deatcive Mobile Call Recived");
  relay_control(states);
  }
 //delay(500);
 }
  

bool call() //call from mobile etc
{
  Serial.println("Disabling interupt  ");
  return 1;}


void loop() {
  // put your main code here, to run repeatedly:

int rst_rd = digitalRead(reset_button);
//
//if(rst_rd!=last_state){
//  last_time=millis();
//  Serial.print("last time ");
//  Serial.println(last_time);
//  
//  }

 if(!rst_rd && flag==1){
      Interuptcall(10);
       relay_call(1);
   flag =0;
    String reset_sms="Reset Button pressed "; //+ String(getGSMTimestamp());
    Msg(reset_sms);}
//if((millis()-last_time)>2000){ //  reset button should pressed 0.1 sec; to reset to avaoid ambigous reset  
//  if(rst_rd!=state){
//    state=rst_rd;
//    Serial.print("button pressed rst value  ");
//    Serial.println(state);
//    
//    
//    if(!state){
//      Interuptcall(10);
//       relay_call(1);
//   
//    String reset_sms="Reset recived at time x"; //+ String(getGSMTimestamp());
//    sendResetNotification(reset_sms);}
//    }
//  
//  last_state=rst_rd;}


if(digitalRead(pin[0])==0 && interupt1_state==0 && i_1==0 ){
  Interuptcall(pin[0]);
  recv_msg();
  i_1=1;
  relay
  }
  

if(digitalRead(pin[1])==0&& interupt2_state==0 && i_2==0){
  Interuptcall(pin[1]);
  i_2=1;
  recv_msg();
  }

if(digitalRead(pin[2])==0&& interupt3_state==0 && i_3==0){
  Interuptcall(pin[2]);
  i_3=1;
  recv_msg();
  }

if(digitalRead(pin[3])==0&& interupt4_state==0 && i_4==0){
  Interuptcall(pin[3]);
  i_4=1;
  
  recv_msg();}
if(digitalRead(pin[4])==0&& interupt5_state==0 && i_5==0){
  Interuptcall(pin[4]);
  recv_msg();
  i_5=1;
  }


  

if(sim.available())// for gsm reset 
{
 command = sim.readStringUntil('\n');
    command.trim();
    if (command == "RESET") {
    
      String location = "House no xyz";
      Msg(location);
    }
  }



if(Serial.available()) {
 command_s = Serial.readStringUntil('\n');
    command_s.trim();
    Serial.println("SERIAL RESET");
    if(command_s == "RESET") {
      
    
      String location = "House no xyz";
      Msg(location);
  if(command_s =="RESET" &&flag ==1){
    Serial.println("the interutp is being disable ");
    relay_call(1);
    Interuptcall(10);
    String reset_sms="Reset recived at time x"; //+ String(getGSMTimestamp());
    Msg(reset_sms);
    flag=0;}
      
    }
    
  }



if(interupt5_state||interupt4_state||interupt3_state||interupt2_state||interupt1_state)
 {   // recv_msg();
      relay_call(0);
      //delay(500);//after call wait for 10 send
      flag=1;
    
    }

    
if(command =="Reset" && flag ==1){//receive reset from gsm 
    Serial.println("the interutp is being disable ");
    relay_call(1);
    Interuptcall(10);
    String reset_sms="Reset recived at time x"; //+ String(getGSMTimestamp());
    Msg(reset_sms);
    flag=0;}
    
    
    
    
}
