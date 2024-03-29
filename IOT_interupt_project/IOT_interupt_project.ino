#include <SoftwareSerial.h>



#define GSM_RX_PIN 10
#define GSM_TX_PIN 11
int pin[5]={2,3,4,5,6};
int relay = LED_BUILTIN;
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
SoftwareSerial gsmSerial(GSM_RX_PIN, GSM_TX_PIN);
void setup() {
  // put your setup code here, to run once:
for(int i =0;i<5;i++){
pinMode(pin[i],INPUT_PULLUP);
//attachInterrupt(digitalPinToInterrupt(pin[i]),Interuptcall(pin[i]),CHANGE);
}
pinMode(relay,OUTPUT);
pinMode(reset_button,OUTPUT);
Serial.begin(9600);
gsmSerial.begin(9600);

}
// function to send message on mobile from gsm
void gsmMsg(const String& message) {
  gsmSerial.println("AT+CMGF=1"); // Set SMS 
  delay(10);
  gsmSerial.println("AT+CMGS=\"+1234567890\""); //mobile on
  delay(10);
  gsmSerial.println(message);
  delay(10);
  gsmSerial.write(26); // End of message character
}

// interupt function for interupt detection and 
void Interuptcall(int interupt_no){
  switch(interupt_no){
  case 2:interupt1_state = 1;
    sendmsg(1);
    gsmMsg("Iterupt at port 1");
    //gsmCall();
    break;
  case 3:interupt2_state = 1;
    sendmsg(2);
    gsmMsg("Iterupt at port 2");
    //gsmCall();
    break;
  case 4:interupt3_state = 1;
    sendmsg(3);
    gsmMsg("Iterupt at port 3");
    //gsmCall();
    break;
  case 5:interupt4_state = 1;
    sendmsg(4);
    gsmMsg("Iterupt at port 4");
    //gsmCall();
    break;
  case 6:interupt5_state = 1;
    sendmsg(5);
    gsmMsg("Iterupt at port 5");
    //gsmCall();
    break;

  case 10:
    interupt5_state = 0;
    interupt4_state = 0;
    interupt3_state = 0;
    interupt2_state = 0;
    interupt1_state = 0;
    sendmsg(10);
    gsmMsg("Relay activated");   
    break;
  }
}

// to display interupt detection on remote site i.e serial monitor
void sendmsg(int pin_no){
  
  String txt =(pin_no!=10) ? "Interupt Call on Port no " + String(pin_no) : " Reset call Removing Interupt" ;   
  
  Serial.println("call on 911");
  Serial.println(txt);
  
  }

void gsmCall() {// gsm module call on mobile fuctnion
  gsmSerial.println("ATD+1234567890;"); // phone number
}



void sendResetNotification(const String& location) {
  gsmSerial.println("AT+CMGF=1"); // Set SMS text mode
  delay(100);
  gsmSerial.println("AT+CMGS=\"+1234567890\""); // Replace with recipient's phone number
  delay(100);
  gsmSerial.print("Reset notification\n");
  gsmSerial.print(location);
  gsmSerial.write(26); // End of message character
}



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
  delay(500);}
  

bool call() //call from mobile etc
{
  Serial.println("Disabling interupt  ");
  return 1;}


void loop() {
  // put your main code here, to run repeatedly:

int rst_rd = digitalRead(reset_button);

if(rst_rd!=last_state){
  last_time=millis();
  Serial.print("last time ");
  Serial.println(last_time);
  
  }

if((millis()-last_time)>2000){ //  reset button should pressed 0.1 sec; to reset to avaoid ambigous reset  
  if(rst_rd!=state){
    state=rst_rd;
    Serial.print("button pressed rst value  ");
    Serial.println(state);
    
    
    if(state){
      Interuptcall(10);
       relay_call(0);
   
    String reset_sms="Reset recived at time x"; //+ String(getGSMTimestamp());
    sendResetNotification(reset_sms);}
    }
  
  last_state=rst_rd;}


if(digitalRead(pin[0])==0 && interupt1_state==0 ){
  Interuptcall(pin[0]);
  }
  

if(digitalRead(pin[1])==0&& interupt2_state==0){
  Interuptcall(pin[1]);
  }

if(digitalRead(pin[2])==0&& interupt3_state==0){
  Interuptcall(pin[2]);
  }

if(digitalRead(pin[3])==0&& interupt4_state==0){
  Interuptcall(pin[3]);
  }
if(digitalRead(pin[4])==0&& interupt5_state==0){
  Interuptcall(pin[4]);
  }


  

if(gsmSerial.available())// for gsm reset 
{
 command = gsmSerial.readStringUntil('\n');
    command.trim();
    if (command == "RESET") {
    
      String location = "House no xyz";
      sendResetNotification(location);
    }
  }



if(Serial.available()) {
 command_s = Serial.readStringUntil('\n');
    command_s.trim();
    Serial.println("SERIAL RESET");
    if(command_s == "RESET") {
      
    
      String location = "House no xyz";
      sendResetNotification(location);
  if(command_s =="RESET" &&flag ==1){
    Serial.println("the interutp is being disable ");
    relay_call(0);
    Interuptcall(10);
    String reset_sms="Reset recived at time x"; //+ String(getGSMTimestamp());
    sendResetNotification(reset_sms);
    flag=0;}
      
    }
    
  }



if(interupt5_state||
    interupt4_state||
    interupt3_state||
    interupt2_state||
    interupt1_state ){
      relay_call(1);
      delay(500);//after call wait for 10 send
      flag=1;
    }

    
    else if(command =="RESET" &&flag ==1){//receive reset from gsm 
    Serial.println("the interutp is being disable ");
    relay_call(0);
    Interuptcall(10);
    String reset_sms="Reset recived at time x"; //+ String(getGSMTimestamp());
    sendResetNotification(reset_sms);
    flag=0;}
    
    
    
    
}
