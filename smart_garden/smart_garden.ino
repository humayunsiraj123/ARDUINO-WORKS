#define BLYNK_TEMPLATE_ID "TMPL6h7Ii5HJS"
#define BLYNK_TEMPLATE_NAME "Smart Garden"
#define BLYNK_PRINT Serial
 
 
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal_I2C.h>
#define  BLYNK_PRINT Serial
#include <DHT.h>
#include <Wire.h>
 

char auth[] = "aLs6-kNZsXCpmS_3dnzfKPJcP5-E_ACd";
char ssid[] = "hcn sar 03212323255";
char pass[] = "humayunsj789";


#define SCL 22//for i2c
#define SDA 21//for i2c
#define DHTPIN 25//dht pin
#define soil_sensor_1 34   // Pin connected to soil moisture sensor
#define soil_sensor_2 35   // Pin connected to soil moisture sensor
#define pump_1 19     // Pin connected to relay control
#define pump_2 23
// Pin connected to relay control


bool pump_1_state = 0;
bool pump_2_state = 0;
float soil_threshold =40.0;


DHT dht(DHTPIN, DHT11);
BlynkTimer timer;
LiquidCrystal_I2C lcd(0x3F, 16, 2);// if lcd not work change address ox3F to 0x27


int a=1;

void soilMoistureSensor() {
  a=a+10;
  //int soilMoisture = ++a;//analogRead(soil_sensor_2);
  int soilMoisture_1 =analogRead(soil_sensor_1);//analogRead(soil_sensor_2);
  float soil_per_1 =  (  ( (soilMoisture_1/4095.00) * 100 ) );//
  Serial.print("soil_sensor_1.................... ");
  Serial.println(soil_per_1);

  int soilMoisture_2 = analogRead(soil_sensor_2);//analogRead(soil_sensor_2);
  float soil_per_2 =  (  ( (soilMoisture_2/4095.00) * 100 ) );//
  Serial.print("soil_sensor_2....................  ");
  Serial.println(soil_per_2);

  
  Blynk.virtualWrite(V0, soil_per_1);     // Send soil moisture 1 to V0 on the Blynk app
  Blynk.virtualWrite(V1, soil_per_2);    // Send soil moisture 2 to V1 on the Blynk app

   lcd.setCursor(0,1);
  lcd.print("S_1:");
  lcd.print(soil_per_1);

  lcd.setCursor(8, 1);
  lcd.print("S_2:");
  lcd.print(soil_per_2);

if (soil_per_1>soil_threshold) 
  {
    digitalWrite(pump_1, 0);  // Turn on the water pump
    Serial.println("plant 1 low water ... pump 1 on");
    Blynk.virtualWrite(V4,1);  // Send status message to V9 on the Blynk app
  }

  else if (soil_per_1 < soil_threshold) 
  {  // Adjust the threshold value based on your soil moisture sensor
    digitalWrite(pump_1, 1);  // Turn off pump 1
    Serial.println("plant 1 has enough mositures pump 1 is off.");
    Blynk.virtualWrite(V4,0 );  // Send status message to V9 on the Blynk app
  }

if (soil_per_2>soil_threshold) 
  {
    digitalWrite(pump_2, 0);  // Turn on the water pump
    Serial.println("plant 2 low water ... pump 2 on");
    Blynk.virtualWrite(V4,1);  // Send status message to V9 on the Blynk app
  }

  else if (soil_per_2 < soil_threshold) 
  {  // Adjust the threshold value based on your soil moisture sensor
    digitalWrite(pump_2, 1);  // Turn off pump 1
    Serial.println("plant 2 has enough mositures pump 2 is off.");
    Blynk.virtualWrite(V4,0 );  // Send status message to V9 on the Blynk app
  }


//  lcd.setCursor(0, 0);
//  lcd.print("Moisture :");
//  lcd.print(value);
//  lcd.print(" ");
}


float h;
float t;
void DHT11sensor() {
  h =h>100?0:++h; //dht.readHumidity();
  t =t>100?0:++t;; //dht.readTemperature();
 // h =dht.readHumidity();
 // t =dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(t);

  lcd.setCursor(8, 0);
  lcd.print("H:");
  lcd.print(h);
  Blynk.virtualWrite(V2, t);  // Send temperature to V2 on the Blynk app
  Blynk.virtualWrite(V3, h);     // Send humidity to V3 on the Blynk app
}
 

void setup()
{
   Serial.begin(115200);
  
  pinMode(soil_sensor_1, INPUT);
  pinMode(soil_sensor_2, INPUT);
  pinMode(pump_1, OUTPUT);
  pinMode(pump_2, OUTPUT);
 
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(pump_1,1);
  digitalWrite(pump_2,1);
  
  
  lcd.init();
  dht.begin();
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  
  
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("SMART IRRIGATION");
  timer.setInterval(100L, DHT11sensor);
  timer.setInterval(100L, soilMoistureSensor);
 
 // Blynk.begin(auth, ssid, pass);
 
}

 
void loop()
{
  Blynk.run();
   timer.run();  //Run the Blynk timer
   //int potvalue=analogRead(36);
  //Blynk.virtualWrite(V1,potvalue);
}
