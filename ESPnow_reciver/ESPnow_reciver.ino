#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4); 

// Structure example to receive data
// Must match the sender structure
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
//

// Create a struct_message called myData
struct_message myData,rcv_data;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len); rcv_data.b1 =  myData.b1;
 rcv_data.b2 =  myData.b2;
 rcv_data.b3 =  myData.b3;
 rcv_data.b4 =  myData.b4;
 rcv_data.b5 =  myData.b5;
 rcv_data.b6 =  myData.b6;
 rcv_data.b7 =  myData.b7;
 rcv_data.b8 =  myData.b8;
 rcv_data.b9 =  myData.b9;
 rcv_data.b10  = myData.b10;
rcv_data.line_no = myData.line_no;
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() !=  ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print(" Diverse Automation ");
  lcd.setCursor(1,0);
  lcd.print("   Home Automation  ");
  lcd.setCursor(2,0);
  lcd.print("   ESPNOW PROTOCOL  ");
   lcd.setCursor(3,0);
  lcd.print("        HS          ");
    
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {

Serial.println("recv data bit 1 ");
Serial.println(myData.b1);
Serial.println("recv data bit2 ");
Serial.println(myData.b2);
Serial.println("recv data bit3");
Serial.println(myData.b3);

Serial.println("recv data bit4");
Serial.println(myData.b3);
Serial.println("recv data bit5");
Serial.println(myData.b4);
Serial.println("recv data bit6");
Serial.println(myData.b5);
Serial.println("recv data bit7");
Serial.println("recv data bit8");
Serial.println(myData.b6);
Serial.println("recv data bit9");
Serial.println(myData.b7);
Serial.println("recv data bit10");
Serial.println(myData.b8);
Serial.println("recv data bit x");
delay(2000);

}
