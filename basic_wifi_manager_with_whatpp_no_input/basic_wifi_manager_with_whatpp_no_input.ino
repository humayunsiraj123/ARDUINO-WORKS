#include <WiFiManager.h>
char Whatapp_no[15];
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
      
    WiFiManager manager;
    manager.setDebugOutput(false);
    manager.resetSettings();
 
    WiFiManagerParameter whatapps_no_paramter("whatappID", "Whatsapp Number", "Add whatapp no", 15);
    manager.addParameter(&whatapps_no_paramter);
     
    manager.autoConnect("ESP32_AP");
 
    Serial.println("whatappno:");
    Serial.println(whatapps_no_paramter.getValue());
}

void loop() {
  // put your main code here, to run repeatedly:

}
