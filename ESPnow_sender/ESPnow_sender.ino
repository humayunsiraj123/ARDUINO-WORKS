#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH YOUR RECEIVER MAC Address 9C:9C:1F:EA:D4:B0  9C:9C:1F:EA:D4:B0

uint8_t broadcastAddress[] = {0x9C, 0x9C, 0x1F, 0xEA, 0xD4, 0xB0};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
int b1;
int b2;
int b3;
int b4;
int b5;
int b6;
int b7;
int b8;
int b9;
int b10;
int line_no;
} struct_message;

// Create a struct_message called myData
struct_message myData,state;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
  // Set values to send


  // Send message via ESP-NOW
  myData.b1=random();
  myData.b2=random();
  myData.b3=random();
  myData.b4=random();
  myData.b5=random();
  myData.b6=random();
  myData.b7=random();
  myData.b8=random();
  myData.b9=random();
  
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(2000);
}
