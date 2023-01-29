/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
*********/

#include <FS.h> //this needs to be first, or it all crashes and burns...
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          // https://github.com/tzapu/WiFiManager
#include <ArduinoJson.h>          // https://github.com/bblanchon/ArduinoJson
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <UrlEncode.h>
#include <WiFiClient.h>
#include <string.h>

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String outputState = "off";

// Assign output variables to GPIO pins
char output[2] = "5";
char whatappno[15];
char whatappcode[15];
String phoneNumber;
String apiKey ;

//patient code.....
bool flag=0;
int reading,reading2;

int Sensor = D2;  
int LED = LED_BUILTIN; 
bool flg=0;
int current=0,previous=0;
int current1=0,previous1=0;
int sensorval = 0;  // RCWL-0516 Sensor Value
int pbval = 0;      // Push button value
int count;
int det;
const char* host = "script.google.com";
const int httpsPort = 443;

///////////////////

WiFiClientSecure clients; //--> Create a WiFiClientSecure object.

String GAS_ID = "AKfycbztCElvEvW_LeH7VxsHizMAAVCUHJnF5rj2jYsytedyMIz0yDapw8NmttXOwWbRt-obXQ"; 



//flag for saving data
bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

void setup() {
  Serial.begin(115200);
   pinMode (Sensor, INPUT_PULLUP); 
  pinMode (LED, OUTPUT);   
  Serial.println("Waiting for motion");

  //delay(500);
  //clean FS, for testing
  ///SPIFFS_CALL();
  Serial.println("mounting FS...");

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");
          //strcpy(output, json["output"]);
          strcpy(whatappno, json["whatappno"]);
        strcpy(whatappcode, json["whatappcode"]);
        
        
        } else {
          Serial.println("failed to load json config");
        }
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
  //end read
  
  
  WiFiManagerParameter whatapp_no("whatappno", "Whatapp Number", whatappno,15 );
  WiFiManagerParameter whatapp_code("whatappcode", "Whatapp Code", whatappcode, 15);

  // WiFiManager
  // Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  //set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  
  // set custom ip for portal
  //wifiManager.setAPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

  //add all your parameters here
//  wifiManager.addParameter(&custom_output);
  wifiManager.addParameter(&whatapp_no);
  wifiManager.addParameter(&whatapp_code);
  
  // Uncomment and run it once, if you want to erase all the stored information
  //wifiManager.resetSettings();

  //set minimu quality of signal so it ignores AP's under that quality
  //defaults to 8%
  //wifiManager.setMinimumSignalQuality();
  
  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  //wifiManager.setTimeout(120);

  // fetches ssid and pass from eeprom and tries to connect
  // if it does not connect it starts an access point with the specified name
  // here  "AutoConnectAP"
  // and goes into a blocking loop awaiting configuration
  wifiManager.autoConnect("Alpha X");
  // or use this for auto generated name ESP + ChipID
  //wifiManager.autoConnect();
  
  // if you get here you have connected to the Wife
  Serial.println("Connected.");
  
  //strcpy(output, custom_output.getValue());
  if(whatapp_code.getValue()>0){strcpy(whatappno, whatapp_no.getValue());
strcpy(whatappcode, whatapp_code.getValue());
}else{
  
  }
  String phoneNumber = String(whatappno);
String apiKey = String(whatappcode);
Serial.print("the phone no is ");
Serial.println(phoneNumber);
Serial.print("the whatapp code is ");
Serial.println(apiKey);


  //save the custom parameters to FS
  if (shouldSaveConfig) {
    Serial.println("saving config");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    //json["output"] = output;
    json["whatappcode"] = whatappcode;
    json["whatappno"] = whatappno;


    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }

    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close();
    //end save
  }

  // Initialize the output variables as outputs
  pinMode(atoi(output), OUTPUT);
  // Set outputs to LOW
  digitalWrite(atoi(output), LOW);;
  
  server.begin();
  
  clients.setInsecure();
  sendMessage("Hello from ESP8266!");
  sendMessage("its from arduino cloud");
}

void loop(){

   sensorval = digitalRead(Sensor);  // Read Sensor value
       // Read Push button value
 
  if (sensorval ==LOW && count==0) {
   count=1;
   delay(2000);         
  }
  if(sensorval==LOW   && count==1){
      // Read Sensor value
      if(digitalRead(Sensor)==LOW){
        Serial.println("MISSING");
        sendData(0);
        sendMessage("PERSON IS MISSING FROM THE BED");
        delay(2000);
        }
        count=0;
    }
    
  else if (sensorval==HIGH){
    Serial.println("DETECTED");
    sendData(1);
    count=0;
    }
   // Serial.println(sensorval);
delay(1000);
}
 


void SPIFFS_CALL(){ //SPIFFS.format();

  //read configuration from FS json
  Serial.println("mounting FS...");

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");
          //strcpy(output, json["output"]);
          strcpy(whatappno, json["whatappno"]);
        strcpy(whatappcode, json["whatappcode"]);
        
        
        } else {
          Serial.println("failed to load json config");
        }
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
  
  
  }


  void sendMessage(String message){

  // Data to send with HTTP POST
  String url = "http://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);
  WiFiClient client1;    
  HTTPClient http;
  http.begin(client1, url);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  // Send HTTP POST request
  int httpResponseCode = http.POST(url);
  if (httpResponseCode == 200){
    Serial.print("Message sent successfully");
  }
  else{
    Serial.println("Error sending the message");
    Serial.print("HTTP response code: ");
    Serial.println(httpResponseCode);
  }

  // Free resources
  http.end();
}

void sendData(int i) {
  String url=" ";
  Serial.println("==========");
  Serial.print("connecting to ");
  Serial.println(host);
  
  //----------------------------------------Connect to Google host
  if (!clients.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
  //----------------------------------------

  //----------------------------------------Processing data and sending data
 // String string_temperature =  String(tem);
  // String string_temperature =  String(tem, DEC); 
  //String string_humidity =  String(hum);
  
  if(i==1){
    url = "/macros/s/" + GAS_ID + "/exec?person=" + "1" + "&humidity=" + "DETECTED";
    
  }
  if(i==0){
    url = "/macros/s/" + GAS_ID + "/exec?person=" + "MISSING" + "&humidity=" + "0";

  Serial.print("requesting URL: ");
  Serial.println(url);

  clients.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" +
         "User-Agent: BuildFailureDetectorESP8266\r\n" +
         "Connection: close\r\n\r\n");

  Serial.println("request sent");
  //----------------------------------------

  //----------------------------------------Checking whether the data was sent successfully or not
  while (clients.connected()) {
    String line = clients.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = clients.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successfull!");
  } else {
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.print("reply was : ");
  Serial.println(line);
  Serial.println("closing connection");
  Serial.println("==========");
  Serial.println();
  //----------------------------------------
} 
}

void webpage_function(){
   WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /output/on") >= 0) {
              Serial.println("Output on");
              outputState = "on";
              digitalWrite(atoi(output), HIGH);
            } else if (header.indexOf("GET /output/off") >= 0) {
              Serial.println("Output off");
              outputState = "off";
              digitalWrite(atoi(output), LOW);
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP8266 Web Server</h1>");
            
            // Display current state, and ON/OFF buttons for the defined GPIO  
            client.println("<p>Output - State " + outputState + "</p>");
            // If the outputState is off, it displays the ON button       
            if (outputState=="off") {
              client.println("<p><a href=\"/output/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/output/off\"><button class=\"button button2\">OFF</button></a></p>");
            }                  
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
  
  
  
  }
