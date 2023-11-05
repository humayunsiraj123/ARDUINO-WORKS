#include <Arduino.h>
#include <otadrive_esp.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

struct configs
{
  uint32_t sign;
  char ssid[33];
  char pass[33];
  uint16_t blink_on;
  uint16_t blink_off;
};

configs local_configs;

void setup()
{
  OTADRIVE.setInfo("ba865ce2-7075-4ace-a7d2-4f6809545476", "v@1.0.0");
  Serial.begin(115200);
  EEPROM.begin(512);
  EEPROM.get(0, local_configs);

  if (local_configs.sign != 0xA123B456)
  {
    Serial.println("Load default configs");
    strcpy(local_configs.ssid, "hcn sar 03212323255");
    strcpy(local_configs.pass, "");
    local_configs.blink_on = 250;
    local_configs.blink_off = 350;
    local_configs.sign = 0xA123B456;
    EEPROM.put(0, local_configs);
    EEPROM.commit();
  }
  else
  {
    Serial.println("Configs are valid");
  }

  WiFi.begin(local_configs.ssid, local_configs.pass);
  pinMode(LED_BUILTIN, OUTPUT);
}

void sync_task()
{
  if (!OTADRIVE.timeTick(30))
    return;

  if (WiFi.status() != WL_CONNECTED)
    return;

  // do sync and update operations here
  OTADRIVE.updateFirmware();

  String c = OTADRIVE.getConfigs();
  Serial.printf("config download: %s\n", c.c_str());
  StaticJsonDocument<250> doc;
  if (deserializeJson(doc, c) != DeserializationError::Ok)
  {
    Serial.println("deserialize error");
    return;
  }

  Serial.printf("configs decode ssid=%s,blink_on=%d\n", (const char *)doc["ssid"], (int)doc["blink_on"]);

  local_configs.blink_on = doc["blink_on"];
  local_configs.blink_off = doc["blink_off"];
  strcpy(local_configs.ssid, doc["ssid"]);
  strcpy(local_configs.pass, doc["pass"]);

  EEPROM.put(0, local_configs);
  EEPROM.commit();
}

void loop()
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(local_configs.blink_on);
  digitalWrite(LED_BUILTIN, LOW);
  delay(local_configs.blink_off);
  sync_task();
}
