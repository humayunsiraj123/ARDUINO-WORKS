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


// Create a struct_message called myData
struct_message myData,rcv_data;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
 rcv_data.b1 =  mydata.b1;
 rcv_data.b2 =  mydata.b2;
 rcv_data.b3 =  mydata.b3;
 rcv_data.b4 =  mydata.b4;
 rcv_data.b5 =  mydata.b5;
 rcv_data.b6 =  mydata.b6;
 rcv_data.b7 =  mydata.b7;
 rcv_data.b8 =  mydata.b8;
 rcv_data.b9 =  mydata.b9;
 rcv_data.b10  = mydata.b10;
rcv_data.line_no = mydata.line_no;
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() ! =  ESP_OK) {
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

}
