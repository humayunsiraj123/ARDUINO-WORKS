#include "arduino_secrets.h"
// PZEM004Tv30 - Version: Latest 
#include <PZEM004Tv30.h>

#include <SoftwareSerial.h>
#if defined(ESP32)
    #error "Software Serial is not supported on the ESP32"                
#endif

/* Use software serial for the PZEM
 * Pin 12 Rx (Connects to the Tx pin on the PZEM)
 * Pin 13 Tx (Connects to the Rx pin on the PZEM)
*/
#if !defined(PZEM_RX_PIN) && !defined(PZEM_TX_PIN)
#define PZEM_RX_PIN D6
#define PZEM_TX_PIN D5
#endif


SoftwareSerial pzemSWSerial(PZEM_RX_PIN, PZEM_TX_PIN);
PZEM004Tv30 pzem(pzemSWSerial);
/* 
  Sketch generated by the Arduino IoT Cloud Thing "Untitled"
  https://create.arduino.cc/cloud/things/d155f868-a3c3-402e-aee7-74fd01357dd3 

  Arduino IoT Cloud Variables description

  The following variables are automatically generated and updated when changes are made to the Thing

  float current1;
  float energy1;
  float power1;
  float voltage1;
  CloudLight switche;

  Variables which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/

#include "thingProperties.h"
int incomingByte = 0; // for incoming serial data


int state;





void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500); 
  pinMode(D7,OUTPUT);

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
 */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
   if (Serial.available()) {
    // read the incoming byte:
     state =Serial.read();
    Serial.print("thee  byte");
    Serial.println(state);}
    if(state=='1'){
      digitalWrite(D7,1);}
    
    if(state=='0'){
      digitalWrite(D7,0);}
    
  ArduinoCloud.update();
  onSwitcheChange();
  pzemupdate();

  }
    
    
  // Your code here 
  
  





/*
  Since Switche is READ_WRITE variable, onSwitcheChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onSwitcheChange()  {
  if(switche==true){digitalWrite(D0,1);}
  else if(switche==false){digitalWrite(D0,0);}
   
  // Add your code here to act upon Switche change
}

void pzemupdate(){
  float voltage = pzem.voltage();
    float current = pzem.current();
    float power = pzem.power();
    float energy = pzem.energy();
    float frequency = pzem.frequency();
    float pf = pzem.pf();
current1=current;
voltage1=voltage;
power1=power;
energy1=energy;
    // Check if the data is valid
    if(isnan(voltage1)){
        Serial.println("Error reading voltage");
    } else if (isnan(current1)) {
        Serial.println("Error reading current");
    } else if (isnan(power1)) {
        Serial.println("Error reading power");
    } else if (isnan(energy1)) {
        Serial.println("Error reading energy");
    } else if (isnan(frequency)) {
        Serial.println("Error reading frequency");
    } else if (isnan(pf)) {
        Serial.println("Error reading power factor");
    } else {

        // Print the values to the Serial console
        Serial.print("Voltage: ");      Serial.print(voltage1);      Serial.println("V");
        Serial.print("Current: ");      Serial.print(current1);      Serial.println("A");
        Serial.print("Power: ");        Serial.print(power1);        Serial.println("W");
        Serial.print("Energy: ");       Serial.print(energy1,3);     Serial.println("kWh");
        Serial.print("Frequency: ");    Serial.print(frequency, 1); Serial.println("Hz");
        Serial.print("PF: ");           Serial.println(pf);
    }

    Serial.println();
}
