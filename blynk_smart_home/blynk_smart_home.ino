#define BLYNK_TEMPLATE_ID "TMPL2chctRnR8"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "VqhnSc-3wCOPqsI8WZk3tMmopAiBtQNX"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>
#include <DHT.h>
Servo servo;
char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "Wokwi-GUEST";
char pass[] = "";

BlynkTimer timer;

#define button1_pin 26
#define relay1_pin 13
#define servo_pin 18
#define DHTPIN 19
int relay1_state = 0;

#define button1_vpin    V1
bool sec_cntrl=false;//to disarm and arm secutiry system 
//#define PIR 25      //D5 PIR Motion Sensor
DHT dht(DHTPIN, DHT22);

BLYNK_CONNECTED()
{
  Blynk.syncVirtual(button1_vpin);
  
}

BLYNK_WRITE(button1_vpin) 
{
  relay1_state = param.asInt();
  digitalWrite(relay1_pin, relay1_state);
}

//cntrl shed servo
BLYNK_WRITE(V2){
  int Degree = param.asInt();
  servo.write(Degree);
}

//to get the disarm/arm of security system use var sec_cntrl 
BLYNK_WRITE(V4) {
  sec_cntrl= param.asInt();
  Serial.println(sec_cntrl);
}

float h;
float t;
void DHT11sensor() {
 // h =h>100?0:++h; //dht.readHumidity();
  //t =t>100?0:++t;; //dht.readTemperature();
  h =dht.readHumidity();
  t =dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Blynk.virtualWrite(V0, t);  // Send temperature to V0 on the Blynk app
  Blynk.virtualWrite(V1, h);     // Send humidity to V1 on the Blynk app
}


void setup()
{
  
  Serial.begin(115200);
  pinMode(25, INPUT_PULLUP);
  pinMode(button1_pin, INPUT_PULLUP);
  dht.begin();
 servo.attach(servo_pin);
  pinMode(relay1_pin, OUTPUT);
  
  digitalWrite(relay1_pin, HIGH);
  
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, DHT11sensor);

}

void loop()
 
{
  Blynk.run();
  timer.run();
  listen_push_buttons();
}
 
 



void listen_push_buttons()
{
    if(digitalRead(button1_pin) == LOW){
      delay(200);
      control_relay(1);
      Blynk.virtualWrite(button1_vpin, relay1_state); //update button state
    }
    
}

void control_relay(int relay)
{
  if(relay == 1){
    relay1_state = !relay1_state;
    digitalWrite(relay1_pin, relay1_state);
    Serial.print("Light = ");
    Serial.println(relay1_state);
    delay(50);
  }
  

}
