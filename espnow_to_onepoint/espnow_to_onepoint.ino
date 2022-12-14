/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-many-to-one-esp8266-nodemcu/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <ESP8266WiFi.h>
#include <espnow.h>

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    int id;
   float voltage;
 float current;
  float power ;
  float energy;
  float SOC;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Create a structure to hold the readings from each board
struct_message board1;
struct_message board2;

// Create an array with all the structures
struct_message boardsStruct[2] = {board1, board2};

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac_addr, uint8_t *incomingData, uint8_t len) {
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
  Serial.printf("voltage value: %f \n", boardsStruct[myData.id-1].voltage);
  Serial.printf("current value: %f \n", boardsStruct[myData.id-1].current);
  Serial.printf("SOC value: %f \n", boardsStruct[myData.id-1].SOC);
  Serial.printf("power value: %f \n", boardsStruct[myData.id-1].energy);
  Serial.printf("energy value: %f \n", boardsStruct[myData.id-1].power);
  Serial.println();
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop(){
  // Access the variables for each board
  /*int board1X = boardsStruct[0].x;
  int board1Y = boardsStruct[0].y;
  int board2X = boardsStruct[1].x;
  int board2Y = boardsStruct[1].y;
  */
  float board1voltage=boardsStruct[0].voltage;
   float board1current=boardsStruct[0].current;
    float board1SOC=boardsStruct[0].SOC;
  float board1power=boardsStruct[0].power;
   float board1energy=boardsStruct[0].energy;
  
  float board2voltage=boardsStruct[1].voltage;
   float board2current=boardsStruct[1].current;
    float board2SOC=boardsStruct[1].SOC;
  float board2power=boardsStruct[1].power;
   float board2energy=boardsStruct[1].energy;

  delay(5000);  
}
