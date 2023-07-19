#include <otadrive_esp.h>

void setup()
{pinMode(LED_BUILTIN,OUTPUT);
  WiFi.begin("hcn sar 03212323255","humayunsj789");
  OTADRIVE.setInfo("0e70297f-de48-4ffb-9fa1-c7b5e2cb2436", "v@1.1.1");
  //...
}

void ota()
{
  if(OTADRIVE.timeTick(30))
  {
    OTADRIVE.updateFirmware();
  }
}

void loop()
{ digitalWrite(LED_BUILTIN, 1);
  delay(1050);
  digitalWrite(LED_BUILTIN, 0);
  delay(1050);
  ota();
 
}
