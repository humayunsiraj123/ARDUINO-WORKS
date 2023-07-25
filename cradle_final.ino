#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>

// Replace with your network credentials
const char* ssid = "hcn sar 03212323255";
const char* password = "humayunsj789";

// Set web server port number to 80
AsyncWebServer server(80);

int water_sensor = 27;
bool isWet = false;
int pir_sensor = 26;
bool isAwake = false;
bool buzzerOn = false;
bool flag = false;
int buzzer =32;

void setup() {
  Serial.begin(115200);

  pinMode(water_sensor, INPUT);
  pinMode(pir_sensor, INPUT);
  pinMode(buzzer, OUTPUT);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Print local IP address
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String page = "<!DOCTYPE html><html>";
    page += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
    page += "<link rel=\"icon\" href=\"data:,\">";
    page += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}";
    page += ".block { width: 200px; height: 200px; margin: 20px auto; border-radius: 8px; font-size: 24px; font-weight: bold; line-height: 200px; }";
    page += ".block.green { background-color: #4CAF50; color: white; }";
    page += ".block.red { background-color: #FF0000; color: white; }";
    page += ".block.blue {background-color: #0000FF;color: white;}";
    page += ".block.white {background-color: #FFFFFF; color: black;}";
    page += "</style></head>";
    page += "<body>";
    page += "<h1>IOT BASED SMART CRADLE</h1>";
    page += "<div class=\"block ";
    page += (isWet ? "blue\">URINE DETECTED" : "white\">NO URINE DETECTED");
    page += "</div>";
    page += "<div class=\"block ";
    page += (isAwake ? "red\">BABY AWAKE" : "green\">BABY SLEEPING");
    page += "</div>";
    page += "<p>BUZZER ALERT ";
    page += ((buzzerOn) ? "on" : "off");
    page += "</p>";
    page += "<p><a href=\"/toggle\"><button class=\"button\">";
    page += ((buzzerOn)? "TURN OFF BUZZER" : "TURN ON BUZZER");
    page += "</button></a></p>";
    page += "<script>setInterval(function() { location.reload(); }, 1000);</script>"; // Refresh page every 2 seconds
    page += "</body></html>";
    request->send(200, "text/html", page);
    digitalWrite(buzzer,buzzerOn);
      
  });

  // Route to handle the buzzer toggle
  server.on("/toggle", HTTP_GET, [](AsyncWebServerRequest *request) {
    buzzerOn = !buzzerOn;
    //flag=!flag;
    //flag=0;
    Serial.print("flag");
    Serial.println(flag);
    
    //flag =!flag;
     //flag=(!isAwake)?0:flag; 
    //digitalWrite(2, flag);
    request->send(200, "text/plain", buzzerOn ? "Buzzer ON" : "Buzzer OFF");
  });

  // Start the server
  server.begin();
}

void loop() {
  // Update sensor states
  isWet = !digitalRead(water_sensor);
  isAwake = digitalRead(pir_sensor);
  if(isAwake && !flag)
  {buzzerOn =1;
  flag=1;}
  // digitalWrite(2,buzzerOn);
   // }
  else if(!isAwake && flag)
  {
  flag=0;
  // digitalWrite(2,buzzerOn);
   // }
  }
   
    
  delay(1000);

  // Handle incoming client requests
  // Note: The server will handle incoming requests asynchronously
//  server.handleClient();
}
