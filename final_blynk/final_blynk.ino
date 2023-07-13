#include <LiquidCrystal_I2C.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);


#define BLYNK_TEMPLATE_ID "TMPL2xW1qNNeE"
#define BLYNK_TEMPLATE_NAME "Smart Irrigation System"

char auth[] = "mdaffpH0i1GG0WSqHStVM8cdHZ34ztJz";
char ssid[] = "hcn sar 03212323255";
char pass[] = "humayunsj789";
#define DHTPIN D3//dht pin

BlynkTimer timer;
bool Relay = 0;

DHT dht(DHTPIN, DHT11);
#define Rainpin D7     // Pin connected to rain detector sensor
#define Soilpin A0   // Pin connected to soil moisture sensor
#define Relaypin D4           // Pin connected to relay control
void setup() {
  Serial.begin(115200);
   Serial.begin(115200);
  pinMode(Rainpin, INPUT);
  pinMode(Soilpin, INPUT);
  pinMode(Relaypin, OUTPUT);
  digitalWrite(Relaypin,1);

  lcd.init();
  lcd.backlight();
  dht.begin();

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  lcd.setCursor(0, 0);
  lcd.print("Smart Irrigation");
//  lcd.setCursor(1, 0);
//  lcd.print("System Loading");
//  for (int a = 0; a <= 15; a++) {
//    lcd.setCursor(a, 1);
//    lcd.print(".");
//    delay(500);
//  }
//  lcd.clear();

  timer.setInterval(100L, DHT11sensor);
  timer.setInterval(100L, soilMoistureSensor);
}

//Get the button value
//BLYNK_WRITE(V7) {
//  Relay = param.asInt();
//
//  if (Relay == 1) {
//    digitalWrite(waterPump, LOW);
//
//  } else {
//    digitalWrite(waterPump, HIGH);
//  }
//}
int a=1;
//Get the soil moisture values
void soilMoistureSensor() {
  
//  int value = analogRead(sensor);
//  value = map(value, 0, 1024, 0, 100);
//  value = (value - 100) * -1;

  int soilMoisture = ++a;//analogRead(Soilpin);
  bool isRaining = digitalRead(Rainpin);
  
  Blynk.virtualWrite(V3, soilMoisture);     // Send soil moisture to V7 on the Blynk app
  Blynk.virtualWrite(V4, isRaining);         // Send rain status to V8 on the Blynk app
if (isRaining) 
  {
    digitalWrite(Relaypin, 1);  // Turn off the water pump
    Serial.println("Rain detected. Water pump is off.");
    Blynk.virtualWrite(V4, "Rain Detected");  // Send status message to V9 on the Blynk app
  } 
  else if (soilMoisture > 500) 
  {  // Adjust the threshold value based on your soil moisture sensor
    digitalWrite(Relaypin, 0);  // Turn on the water pump
    Serial.println("Soil moisture is low. Water pump is on.");
    Blynk.virtualWrite(V12, "Watering");  // Send status message to V9 on the Blynk app
  }
  else
  {
    digitalWrite(Relaypin, 1);  // Turn off the water pump
    Serial.println("Soil moisture is sufficient. Water pump is off.");
    Blynk.virtualWrite(V12, "Idle");  // Send status
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

void loop() {
  Blynk.run();  //Run the Blynk library
  timer.run();  //Run the Blynk timer
}
