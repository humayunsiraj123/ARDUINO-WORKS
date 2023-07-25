#include <WiFi.h>
#include <Adafruit_Sensor.h>

// Replace with your network credentials
const char* ssid = "Rohilla";
const char* password = "87654321";

// Set web server port number to 80
WiFiServer server(80);

// Pin definitions
int water_sensor = 34;  // Pin connected to the water sensor
bool isWet = false;     // Flag to indicate if the water sensor is wet
int pir_sensor = 32;    // Pin connected to the PIR motion sensor
bool isAwake = false;   // Flag to indicate if motion is detected

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output26State = "off";  // State of the buzzer

// Assign output variables to GPIO pins
const int output26 = 26;  // Pin connected to the buzzer

void setup() {
  Serial.begin(115200);

  // Initialize the output variables as outputs
  pinMode(output26, OUTPUT);
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

void loop() {
  // Check the state of the sensors
  isWet = ! digitalRead(water_sensor);
  isAwake = digitalRead(pir_sensor) ;

  // Listen for incoming clients
  WiFiClient client = server.available();

  if (client) {
    // Handle the client request
    handleClientRequest(client);
  }
}

void handleClientRequest(WiFiClient& client) {
  // Current time
  unsigned long currentTime = millis();
  // Previous time
  unsigned long previousTime = 0;
  // Define timeout time in milliseconds (example: 2000ms = 2s)
  const long timeoutTime = 2000;

  // Reset the header string
  header = "";

  // Wait for the client to send the complete request or reach the timeout
  while (client.connected() && currentTime - previousTime <= timeoutTime) {
    currentTime = millis();
    if (client.available()) {
      // Read the incoming byte from the client
      char c = client.read();
      // Store the byte in the header string
      header += c;

      if (c == '\n') {
        // Check if the current line is empty, indicating the end of the request
        if (header.length() == 2) {
          // Send the HTTP response to the client
          sendHTTPResponse(client);
          break;
        }
      }
    }
  }

  // Close the client connection
  client.stop();
  Serial.println("Client disconnected.");
  Serial.println("");
}

void sendHTTPResponse(WiFiClient& client) {
  // Send the HTTP response headers
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();

  // Handle the different HTTP routes
  if (isAwake) {
    // Motion is detected
    if (header.indexOf("GET /26/off") >= 0) {
      // Turn off the buzzer
      Serial.println("BUZZER OFF");
      output26State = "off";
      digitalWrite(output26, LOW);
    } else {
      // Turn on the buzzer
      Serial.println("BUZZER ON");
      output26State = "on";
      digitalWrite(output26, HIGH);
    }
  }

  // Generate the HTML response page
  client.println("<!DOCTYPE html><html>");
  client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"icon\" href=\"data:,\">");
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

  client.println("<body><h1>IOT BASED SMART CRADLE</h1>");

  // Display the state of the water sensor
  client.print("<div class=\"block ");
  if (!isWet) {
    client.print("blue\">URINE DETECTED");
  } else {
    client.print("white\">NO URINE DETECTED");
  }
  client.println("</div>");

  // Display the state of the motion sensor
  client.print("<div class=\"block ");
  if (isAwake) {
    client.print("red\">BABY AWAKE");
  } else {
    client.print("green\">BABY SLEEPING");
  }
  client.println("</div>");

  // Display the state of the buzzer
  client.println("<p>BUZZER ALERT: " + output26State + "</p>");
  if (output26State == "on") {
    client.println("<p><a href=\"/26/off\"><button class=\"button button2\">TURN OFF BUZZER</button></a></p>");
  } else {
    client.println("<p><a href=\"/26/on\"><button class=\"button\">TURN ON BUZZER</button></a></p>");
  }

  client.println("</body></html>");
}
