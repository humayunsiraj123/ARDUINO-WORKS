#include <Arduino.h>
#include <ESP8266httpUpdate.h>

void update();
void setup()
{
  // put your setup code here, to run once:
  pinMode(2, OUTPUT);
  WiFi.begin("hcn sar 03212323255", "humayunsj789");
}

uint32_t updateCounter = 0;

void loop()
{
  // put your main code here, to run repeatedly:
  digitalWrite(2, 1);
  delay(2050);
  digitalWrite(2, 0);
  delay(2250);

  if (WiFi.status() == WL_CONNECTED)
  {
    updateCounter++;
    if (updateCounter > 20)
    {
      updateCounter = 0;
      update();
    }
  }
}

void update()
{
  String url = "http://otadrive.com/deviceapi/update?";
  url += "k=7a1e88e2-68bf-45ec-a153-fa792c514e3f";
  url += "&v=1.0.0.1";
  url += "&s=" + String(ESP.getChipId());

  WiFiClient client;
  ESPhttpUpdate.update(client, url, "1.0.0.2");
}
