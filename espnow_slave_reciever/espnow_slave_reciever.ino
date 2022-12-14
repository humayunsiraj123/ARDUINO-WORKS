/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-many-to-one-esp32/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

#include <esp_now.h>
#include <ESP8266WiFi.h>

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  int id;
  float voltage;
  float current;
  float power ;
  float energy;
  float SOC;
}struct_message;

// Create a struct_message called myData
struct_message myData;

// Create a structure to hold the readings from each board
struct_message board1;
struct_message board2;
struct_message board3;
struct_message board4;
// Create an array with all the structures
struct_message boardsStruct[4] = {board1, board2, board3,board4};

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.printf("Board ID %u: %u bytes\n", myData.id, len);
  // Update the structures with the new incoming data
  boardsStruct[myData.id-1].voltage = myData.voltage;
  boardsStruct[myData.id-1].current = myData.current;
  boardsStruct[myData.id-1].SOC = myData.SOC;
  boardsStruct[myData.id-1].energy = myData.energy;
  boardsStruct[myData.id-1].power = myData.power;
  Serial.printf("voltage value: %d \n", boardsStruct[myData.id-1].voltage);
  Serial.printf("current value: %d \n", boardsStruct[myData.id-1].current);
  Serial.printf("SOC value: %d \n", boardsStruct[myData.id-1].SOC);
  Serial.printf("power value: %d \n", boardsStruct[myData.id-1].energy);
  Serial.printf("energy value: %d \n", boardsStruct[myData.id-1].power);
  
  Serial.println();
}
 
void setup() {
  //Initialize Serial Monitor
  Serial.begin(115200);
  
  //Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  //Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  // Acess the variables for each board
 
  float board1voltage=boardsStructure[0].voltage;
   float board1current=boardsStructure[0].current;
    float board1SOC=boardsStructure[0].SOC;
  float board1power=boardsStructure[0].power;
   float board1energy=boardsStructure[0].energy;
  
  float board2voltage=boardsStructure[1].voltage;
   float board2current=boardsStructure[1].current;
    float board2SOC=boardsStructure[1].SOC;
  float board2power=boardsStructure[1].power;
   float board2energy=boardsStructure[1].energy;
  float board3voltage=boardsStructure[2].voltage;
   float board3current=boardsStructure[2].current;
    float board3SOC=boardsStructure[2].SOC;
  float board3power=boardsStructure[2].power;
   float board3energy=boardsStructure[2].energy;
  delay(5000);  
}
