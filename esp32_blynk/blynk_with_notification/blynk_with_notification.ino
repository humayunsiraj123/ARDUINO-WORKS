#define BLYNK_TEMPLATE_ID "TMPL2xW1qNNeE"
#define BLYNK_TEMPLATE_NAME "Smart Irrigation System"


 
 
// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial
 
 
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal_I2C.h>
#define  BLYNK_PRINT Serial
#include <DHT.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
 
#define BLYNK_TEMPLATE_ID "TMPL2xW1qNNeE"
#define BLYNK_TEMPLATE_NAME "Smart Irrigation System"

char auth[] = "mdaffpH0i1GG0WSqHStVM8cdHZ34ztJz";
char ssid[] = "hcn sar 03212323255";
char pass[] = "humayunsj789";



//RST (Reset) <-> D0
//MISO (Master Input Slave Output) <-> 19
//MOSI (Master Output Slave Input) <-> 23
//SCK (Serial Clock) <-> 18
//SS/SDA (Slave select) <-> 5

#define SCL 22
#define SDA 21z

#define DHTPIN 17//dht pin

#define SS_PIN 5
#define SCK 18
#define MOSI 23
#define MISO 19
#define RST_PIN 32

//#define GREEN_LED 2
//#define RED_LED 14

#define Rainpin 34     // Pin connected to rain detector sensor
#define Soilpin 35   // Pin connected to soil moisture sensor
#define Relaypin 4           // Pin connected to relay control
//#define lock 10
#define BUZZER 13
#define LED_G 26
#define LED_R 25
const int lock = 2;

bool Relay = 0;
float soil_threshold =40.0;
bool rfid_cntrl =0;

//BlynkTimer timer;

DHT dht(DHTPIN, DHT11);
BlynkTimer timer;
LiquidCrystal_I2C lcd(0x3F, 16, 2);// if lcd not work change address ox3F to 0x27
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance



byte cardUID[] = {0x33, 0xd7, 0x43, 0x16};   //enter your rfid card vlaue

// rfid lock from blynk
BLYNK_WRITE(V6) {
  rfid_cntrl= param.asInt();
  Serial.println(rfid_cntrl);
}



void rfid_func(){
  if(rfid_cntrl){
    digitalWrite(LED_R,0)
    Serial.println("RFID CNTRL IS HIGH");
if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
;
    // Check if card UID matches pre-programmed card UID
    if (memcmp(mfrc522.uid.uidByte, cardUID, mfrc522.uid.size) == 0) {
      // Card matched
      Serial.println("Match found");
      digitalWrite(LED_G, HIGH);  
      digitalWrite(lock, LOW); 
      
     // digitalWrite(lock, LOW); 
     
      delay(3000);                    
      digitalWrite(lock, HIGH);
      digitalWrite(GREEN_LED, LOW);   
    } else {
      Serial.println("Invalid Card");
     // digitalWrite(RED_LED, HIGH);    
      //delay(1000);                    
     // digitalWrite(RED_LED, LOW);     
    }

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }
  else{
    digitalWrite(LED_R,1);

  }
  }
  
  
}

//Get the button value
BLYNK_WRITE(V12) {
  Relay = param.asInt();

  if (Relay == 1) {
    digitalWrite(Relaypin, LOW);
    Serial.println("pump blynk control low");

  } else if (Relay==0) {
    digitalWrite(Relaypin, HIGH);
  Serial.println("pump blynk control high+++++++++");

  }
}

int a=1;
//Get the soil moisture values
void soilMoistureSensor() {
  
//  int value = analogRead(sensor);
//  value = map(value, 0, 1024, 0, 100);
//  value = (value - 100) * -1;

  //int soilMoisture = ++a;//analogRead(Soilpin);
  int soilMoisture = analogRead(Soilpin);//analogRead(Soilpin);
  float soil_per =  100- (  ( (soilMoisture/4095.00) * 100 ) );//
  Serial.print("soil percentage.................... ");
Serial.println(soil_per);
  if(soil_per<threshold){
    Blynk.logEvent("water_level_alert","soil humidity is on low level turn on pump");
  
  }
 // Blynk.email("abbc@.com", "pump_on", "water pump is on ");
    //Blynk.notify("pump_on : water pump is on ");
    Blynk.logEvent("pump_on","water pump is on");
bool isRaining =! digitalRead(Rainpin);
 Serial.print("soil value is  ---------------------");
 Serial.println(soilMoisture);
 
  
  Blynk.virtualWrite(V3, soil_per);     // Send soil moisture to V7 on the Blynk app
  Blynk.virtualWrite(V4, isRaining);         // Send rain status to V8 on the Blynk app
if (!isRaining) 
  {
    digitalWrite(Relaypin, 1);  // Turn off the water pump
    Serial.println("Rain detected. Water pump is off.");
    digitalWrite(BUZZER,1);
    Blynk.virtualWrite(V4, "Rain Detected");  // Send status message to V9 on the Blynk app
  } 
  else if (soil_per> soil_threshold) 
  {  // Adjust the threshold value based on your soil moisture sensor
    digitalWrite(Relaypin, 0);  // Turn on the water pump
    Serial.println("Soil moisture is low. Water pump is on.");
    Blynk.virtualWrite(V12, "Watering");  // Send status message to V9 on the Blynk app
    // Blynk.email("abbc@.com", "pump_on", "water pump is on ");
    //Blynk.notify("pump_on : water pump is on ");
    Blynk.logEvent("pump_on","water pump is on");
  
  }
  else
  {
    digitalWrite(Relaypin, 1);  // Turn off the water pump
    Serial.println("Soil moisture is sufficient. Water pump is off.");
    Blynk.virtualWrite(V12, "Idle");  // Send status
    digitalWrite(BUZZER,0);
  }
//  lcd.setCursor(0, 0);
//  lcd.print("Moisture :");
//  lcd.print(value);
//  lcd.print(" ");
}
float h;
float t;
void DHT11sensor() {
//  h =h>100?0:++h; //dht.readHumidity();
 // t =t>100?0:++t;; //dht.readTemperature();
  h =dht.readHumidity();
  t =dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print(t);

  lcd.setCursor(8, 1);
  lcd.print("H:");
  lcd.print(h);
  Blynk.virtualWrite(V0, t);  // Send temperature to V5 on the Blynk app
  Blynk.virtualWrite(V1, h);     // Send humidity to V6 on the Blynk app
}
 

void setup()
{
   Serial.begin(115200);
  
  pinMode(Rainpin, INPUT);
  pinMode(Soilpin, INPUT);
  pinMode(Relaypin, OUTPUT);
  pinMode(lock, OUTPUT);
  pinMode(LED_G,OUTPUT);
  pinMode(LED_R,OUTPUT);
  pinMode(BUZZER,OUTPUT);
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(Relaypin,1);

  
  SPI.begin();            // Initialize SPI bus
  mfrc522.PCD_Init();      // Initialize MFRC522 RFID reader
  lcd.init();
  dht.begin();
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  
  
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("SMART IRRIGATION");

   timer.setInterval(1000L, DHT11sensor);
  timer.setInterval(1000L, soilMoistureSensor);
  timer.setInterval(2000L, rfid_func);
 
 // Blynk.begin(auth, ssid, pass);
 
}

 
void loop()
{
  Blynk.run();
   timer.run();  //Run the Blynk timer
   //int potvalue=analogRead(36);
  //Blynk.virtualWrite(V1,potvalue);
}