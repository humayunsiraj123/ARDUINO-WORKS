

int pin[5]={2,5,7,9,8};
int reset_pin 12;
int relay;
bool interupt1_state =0;
bool interupt2_state=0;
bool interupt3_state =0;
bool interupt4_state=0;
bool interupt5_state=0;
bool flag1;// for interutp indication
uint16_t prev_time ;
uint16_t curr_time ;
uint16_t d_time=1000;


void setup() {
  // put your setup code here, to run once:
for(int i =0;i<5;i++){
pinMode(pin[i],INPUT_PULLUP);
attachInterrupt(digitalPinToInterrupt(pin[i]),Interuptcall(pin[i]),LOW);
}
Serial.begin(9600);


}

void Interuptcall(int interupt_no){
  switch(interupt_no){
  case 2:interupt1_state = 1;
    sendmsg(1);
    break;
  case 5:interupt2_state = 1;
    sendmsg(2);
    break;
  case 7:interupt3_state = 1;
    sendmsg(3);
    break;
  case 9:interupt4_state = 1;
    sendmsg(4);
    break;
  case 8:interupt5_state = 1;
    sendmsg(5);
    break;

  case 12:interupt5_state = 0;
    interupt4_state = 0;
    interupt3_state = 0;
    interupt2_state = 0;
    interupt1_state = 0;
    sendmsg(12);   
    break;
  }
}

void sendmsg(int pin_no){
  
  String txt =(pin_no!=12)? "Interupt Call or Port no " + String(pin_no): " Reset call Removing Interupt" ;   
  
  Serial.println("call on 911");
  Serial.println(txt);
  
  }

void relay_control(bool value){
  digitalWrite(12,value);
  }



void callmsg(bool states){
  Serial.println("intertupt recivec  Relay active");
  relay_control(states);
  
  }
  
bool call(){
  Serial.println("Disabling interupt  ");
  return 1;}



void loop() {
  // put your main code here, to run repeatedly:

if(interupt5_state||
  interupt4_state||
    interupt3_state||
    interupt2_state||
    interupt1_state ){
      call_msg();
      delay(1000);
      flag=1;
    }
   if(call()==1 &&flag ==1){
    Serial.println("the interutp is being disable ")
    relay_control(0);
    }
    else {
      flag =0;}
    

}
