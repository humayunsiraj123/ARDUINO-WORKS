#include <SoftwareSerial.h>



#define GSM_RX_PIN 10
#define GSM_TX_PIN 11
int pin[5]={2,5,7,9,8};
int reset_pin = 12;
int relay;
bool interupt1_state =0;
bool interupt2_state=0;
bool interupt3_state =0;
bool interupt4_state=0;
bool interupt5_state=0;
bool flag;// for interutp indication
uint16_t prev_time ;
uint16_t curr_time ;
uint16_t d_time=1000;
String command;
SoftwareSerial gsmSerial(GSM_RX_PIN, GSM_TX_PIN);
void setup() {
  // put your setup code here, to run once:
for(int i =0;i<5;i++){
pinMode(pin[i],INPUT_PULLUP);
attachInterrupt(digitalPinToInterrupt(pin[i]),Interuptcall(pin[i]),LOW);
}
Serial.begin(9600);
gsmSerial.begin(9600);

}

void Interuptcall(int interupt_no){
  switch(interupt_no){
  case 2:interupt1_state = 1;
    sendmsg(1);
    sendMSG("Iterupt at port 1");
    break;
  case 5:interupt2_state = 1;
    sendmsg(2);
    sendMSG("Iterupt at port 2");
    break;
  case 7:interupt3_state = 1;
    sendmsg(3);
    sendMSG("Iterupt at port 3");
    break;
  case 9:interupt4_state = 1;
    sendmsg(4);
    sendMSG("Iterupt at port 4");
    break;
  case 8:interupt5_state = 1;
    sendmsg(5);
    sendMSG("Iterupt at port 5");
    break;

  case 12:
    interupt5_state = 0;
    interupt4_state = 0;
    interupt3_state = 0;
    interupt2_state = 0;
    interupt1_state = 0;
    sendmsg(12);
    sendMSG("Relay activated");   
    break;
  }
}

void sendmsg(int pin_no){
  
  String txt =(pin_no!=12) ? "Interupt Call on Port no " + String(pin_no) : " Reset call Removing Interupt" ;   
  
  Serial.println("call on 911");
  Serial.println(txt);
  
  }

void gsmCall() {
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
  digitalWrite(12,value);
  }



  String getGSMTimestamp(){
  boolean notConnected = true;
  
  return println("AT+CCLK?",1000);
}


void gsmMsg(const String& message) {
  gsmSerial.println("AT+CMGF=1"); // Set SMS 
  delay(10);
  gsmSerial.println("AT+CMGS=\"+1234567890\""); //mobile on
  delay(10);
  gsmSerial.println(message);
  delay(10);
  gsmSerial.write(26); // End of message character
}


void relay_call(bool states){
  if(states==1){
  Serial.println("Relay Active Interupt  Recived");
  relay_control(states);
  }
  if(states==0){
  Serial.println("Relay Deatcive Mobile Call Recived");
  relay_control(states);
  }
  }
  
bool call() //call from mobile etc
{
  Serial.println("Disabling interupt  ");
  return 1;}


//if(digitalRead(pin[0])){
//  Interuptcall(pin[0]);
//  }
//  
//
//else if(digitalRead(pin[1])){
//  Interuptcall(pin[1]);
//  }
//
//else if(digitalRead(pin[2])){
//  Interuptcall(pin[2]);
//  }
//
//else if(digitalRead(pin[3])){
//  Interuptcall(pin[3]);
//  }
//else if(digitalRead(pin[4])){
//  Interuptcall(pin[4]);
//  }
void loop() {
  // put your main code here, to run repeatedly:

if(gsmSerial.available()) {
 command = gsmSerial.readStringUntil('\n');
    command.trim();
    if (command == "RESET") {
    
      String location = "House no xby";
      sendResetNotification(location);
    }
  }
}



if(interupt5_state||
    interupt4_state||
    interupt3_state||
    interupt2_state||
    interupt1_state ){
      relay_call(1);
      delay(1000);//after call wait for 10 send
      flag=1;
    }

    
    else if(command =="Reset" &&flag ==1){
    Serial.println("the interutp is being disable ");
    relay_call(0);
    String reset_sms="Reset recived at " + String(getGSMTimestamp());
    sendResetNotification(reset_sms);
    flag=0;}
    
    else if(command =="Reset" && flag ==1){
    Serial.println("the interutp is being disable ");
    relay_call(0);
    String reset_sms="Reset recived at " + String(getGSMTimestamp());
    sendResetNotification(reset_sms);
    flag=0;}
    
}
