#include <WiFi.h>
#include <Adafruit_Sensor.h>


// Replace with your network credentials
const char* ssid = "hcn sar 03212323255";
const char* password = "humayunsj789";

// Set web server port number to 80
WiFiServer server(80);




int water_sensor = 34;
bool isRaining = false;
int pir_sensor =32;
bool fall =false;

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output26State = "off";

// Assign output variables to GPIO pins
const int output26 = 26;
String wet_state ="off";

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;




void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs

  pinMode(output26, OUTPUT);
 // pinMode(output27, OUTPUT);
  pinMode(water_sensor, INPUT);
  pinMode(pir_sensor, INPUT);
  

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}
String text;
unsigned long prev_time=0;
void loop() {
  
  if(millis()-prev_time>5000){
    prev_time=millis();
    isRaining=!isRaining;
    fall=!fall;
    Serial.print("satte::::");
    Serial.println(isRaining);
    Serial.print("fall::::");
   Serial.println(fall);
    }

    //isRaining=digitalRead(water_sensor);
    //fall=digitalRead(pir_sensor);
    
    

 
//  else{
//  isRaining =0;
//    Serial.println("DRYyyyyyyyy");
//    }
  //(digitalRead(rainSensorPin) == HIGH);

  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out in the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g., HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // turns the GPIOs on and off

            if(fall){
              if(header.indexOf("GET /26/off") >= 0){
                Serial.println("BUZZER DISABLE");
                output26State = "off";
                digitalWrite(LED_BUILTIN, 0);
                
                }
              else{
              Serial.println("BUZZER DISABLE");
                output26State = "on";
                digitalWrite(LED_BUILTIN, 1);
                
                }
              
              }
            
//            if (header.indexOf("GET /26/on") >= 0) {
//              Serial.println("BUZZER DISABLE");
//              output26State = "on";
//              digitalWrite(output26, 1);
//            }
//            else if (header.indexOf("GET /26/off") >= 0) {
//              Serial.println("BUZZER ENABLE");
//              output26State = "off";
//              digitalWrite(output26, 0);
//            }
//            if(fall){
//               output26State = "on";
//              digitalWrite(LED_BUILTIN, 1);
//              }
//              
//            

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}");
            client.println(".block { width: 200px; height: 200px; margin: 20px auto; border-radius: 8px; font-size: 24px; font-weight: bold; line-height: 200px; }");
            client.println(".block.green { background-color: #4CAF50; color: white; }");
            client.println(".block.red { background-color: #FF0000; color: white; }");
            client.println(".block.blue {background-color: #0000FF;color: white;}");
            client.println(".block.white {background-color: #FFFFFF; color: black;}");
            
       
            client.println("</style></head>");

            // Web Page Heading
            client.println("<body><h1>IOT BASED SMART CRADLE</h1>");
            
            //client.println("<h3>Temperature: " + String(temperature) + "&deg;C</h3>");
            
            // Display rain sensor block
            client.print("<div class=\"block ");
            if (isRaining) {
              client.print("blue\">Urine");
            }
            else {
              client.print("white\">No Urine");
            }
            client.println("</div>");

             client.print("<div class=\"block ");
            if (fall) {
              client.print("red\">DANGER");
            }
            else {
              client.print("green\">RELAX");
            }
            client.println("</div>");


            // Display current state and ON/OFF buttons for GPIO 26
            client.println("<p>BUZZER ALERT " + output26State + "</p>");
            // If the output26State is off, it displays the ON button
            
            if (output26State == "on") {
              //client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            else {
            //  client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            client.println("<p><a href=\"/26/on\"><button class=\"button\">BACK</button></a></p>");
            
            }

//            // Display current state and ON/OFF buttons for GPIO 27
//            client.println("<p>Door closing - State " + output27State + "</p>");
//            // If the output27State is off, it displays the ON button
//            if (output27State == "off") {
//              client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");
//            }
//            else {
//              client.println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>");
//            }
//
//            // Display current state and ON/OFF buttons for GPIO 28
//            client.println("<p>Valve - State " + output28State + "</p>");
//            // If the output28State is off, it displays the ON button
//            if (output28State == "off") {
//              client.println("<p><a href=\"/28/on\"><button class=\"button\">ON</button></a></p>");
//            }
//            else {
//              client.println("<p><a href=\"/28/off\"><button class=\"button button2\">OFF</button></a></p>");
//            }
            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          }
          else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        }
        else if (c != '\r') {  // if you got anything else but a carriage return character,
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
