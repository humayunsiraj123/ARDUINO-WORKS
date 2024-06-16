#define BLYNK_TEMPLATE_ID "TMPL2AlnR7HDX"
#define BLYNK_TEMPLATE_NAME "Toy project"
#define BLYNK_AUTH_TOKEN "2iRHbojavTwoVNlzMzbDS4UeUSjxaTB3"

#define BLYNK_PRINT Serial

#include <WiFi.h>

#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"




#define servo_pin 18
#define forward_dir 25
#define rev_dir 14
#define forward_btn 17
#define rev_btn 19
#define PIR 12
#define CAM 21
#define speed 23
static const uint8_t PIN_MP3_TX = 26; // Connects to module's RX 
static const uint8_t PIN_MP3_RX = 27; // Connects to module's TX 



Servo servo;
char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "hcn sar 03212323255";
char pass[] = "humayunsj789";

BlynkTimer timer;
DFRobotDFPlayerMini player;
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);

bool forward_blynk;
bool rev_blynk;

bool forward_state;
bool rev_state;
bool active;



//byte ledPin = 13;

int statostart;
//bool forward_state;
//bool rev_state;
int relay1_state = 0;

//#define button1_vpin    V9
bool toy_active = false; //to disarm and arm secutiry system
//#define PIR 25      //D5 PIR Motion Sensor


// BLYNK_CONNECTED()
// {
//   Blynk.syncVirtual(button1_vpin);

// }

#define timeSeconds 10

// Set GPIOs for LED and PIR Motion Sensor
const int led = 2;
//const int motionSensor = 16;

// Timer: Auxiliary variables
unsigned long now = millis();
unsigned long lastTrigger = 0;
boolean startTimer = false;
boolean motion = false;

// Checks if motion was detected, sets LED HIGH and starts a timer
void IRAM_ATTR detectsMovement() {
  digitalWrite(led, HIGH);
  startTimer = true;
  lastTrigger = millis();
}




//forwards control
BLYNK_WRITE(V1) {
  forward_blynk = param.asInt();
  Serial.print("forward BLYNK:");
  Serial.println(forward_blynk);
  //digitalWrite(forward,(!forward_state));
}



//rev control
BLYNK_WRITE(V2) {
  rev_blynk = param.asInt();
  Serial.print("rev BLYNK:");
  Serial.println(rev_blynk);
  //digitalWrite(rev,(!rev_state));
}


//to get the disarm/arm of toy system use var toy_active
BLYNK_WRITE(V0) {
  toy_active = param.asInt();
  Serial.print("toy_active: ");
  Serial.println(toy_active);
}






void setup()
{

  Serial.begin(115200);
  softwareSerial.begin(9600);
  // pinMode(PIR, INPUT_PULLUP);
  pinMode(forward_btn, INPUT_PULLUP);
  pinMode(rev_btn, INPUT_PULLUP);
  pinMode(rev_dir, OUTPUT);
  digitalWrite(rev_dir, LOW);
  pinMode(forward_dir, OUTPUT);
  digitalWrite(forward_dir, LOW);
  pinMode(speed,OUTPUT);
  pinMode(CAM,OUTPUT);
  // PIR Motion Sensor mode INPUT_PULLUP
  pinMode(PIR, INPUT_PULLUP);
  // Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
  attachInterrupt(digitalPinToInterrupt(PIR), detectsMovement, RISING);
  // Set LED to LOW
  analogWrite(speed,100);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
 
  servo.attach(servo_pin);
  if (player.begin(softwareSerial)) {
   Serial.println("OK");

    // Set volume to maximum (0 to 30).
    player.volume(30);
    // Play the first MP3 file on the SD card
    //player.play(1);
  }
  else {
    Serial.println("Connecting to DFPlayer Mini failed!");
  }
  Blynk.begin(auth, ssid, pass);
  
}


void loop()

{
  Blynk.run();
  timer.run();
  now = millis();
  if ((digitalRead(led) == HIGH) && (motion == false)) {
    Serial.println("MOTION DETECTED!!!");
    motion = true;
    if (toy_active == 1) {
       player.play(1);
       digitalWrite(CAM,1);
      if (forward_blynk & rev_blynk) {
        digitalWrite(forward_dir, 0);
        digitalWrite(rev_dir, 0);
      }
      else if (!forward_blynk & rev_blynk) {
        digitalWrite(forward_dir, 0);
        digitalWrite(rev_dir, 1);
      }
      else if (forward_blynk & !rev_blynk) {
        digitalWrite(forward_dir, 1);
        digitalWrite(rev_dir, 0);
      }
      else if (!forward_blynk & rev_blynk) {
        digitalWrite(forward_dir, 0);
        digitalWrite(rev_dir, 1);
      }
      
      digitalWrite(CAM,0);
    }
  }
  // Turn off the LED after the number of seconds defined in the timeSeconds variable
  if (startTimer && (now - lastTrigger > (timeSeconds * 300))) {
    Serial.println("Motion stopped...");
    digitalWrite(led, LOW);
    digitalWrite(forward_dir, 0);
    digitalWrite(rev_dir, 0);

    startTimer = false;
    motion = false;
  }


  servo_cntrl();
}



void servo_cntrl()
{
  if (digitalRead(forward_btn) == LOW) {
    delay(200);
    if (toy_active) {
      servo.write(90);
      Serial.println("SERVO WRITE +++");

    }
    //Serial.print("forward = ");
    //Serial.println();
    //delay(50);
  }

  if (digitalRead(rev_btn) == LOW) {
    delay(200);
    if (toy_active) {
      Serial.println("SERVO WRITE ---");
      servo.write(-90);
    }
    //Serial.print("rev = ");
    //Serial.println();
    //delay(50);
  }



}
