#include <EEPROM.h>

struct config {
bool flag;
int value;
int price;
int no;
} ;

config param;
int value;
int price;

String readData(int address) {
  String data = "";
  char character = EEPROM.read(address);
  int i = 0;
  while (character != '\0' && i < 255) {
    data += character;
    i++;
    character = EEPROM.read(address + i);
  }
  return data;
}

void writeData(int address, String data) {
  int length = data.length();
  for (int i = 0; i < length; i++) {
    EEPROM.write(address + i, data[i]);
  }
  EEPROM.write(address + length, '\0'); // Null-terminate the string
}

void clearLog() {
  Serial.println("Clearing Logs");
  for (int address = 0; address < EEPROM.length(); address++) {
    EEPROM.write(address, 255);
  }
}

int findNextAvailableAddress() {
  int address = 0;
  // int entrySize = 1; // Size of each data entry (e.g., int)
  
  // Find the next address with the first empty slot
  while (EEPROM.read(address) != 255 && address < EEPROM.length()) {
    // address += entrySize;
    address++;
  }
  if (address == EEPROM.length()) {
    address = 0;
  }
  return address;
}

void report() {
  for (int address = 0; address < findNextAvailableAddress(); address+=25) {
    String data = readData(address);
    if (data.length() > 0) {
      Serial.println(data);
    }
  }
}

void setup() {
  Serial.begin(9600);
  //EEPROM.begin(4096); // Initialize the Mega's EEPROM with 4 KB (4096 bytes)
pinMode(8, INPUT_PULLUP);
  EEPROM.get(0,param);
  if(param.flag==1){
    price= param.price;
    value= param.value;
    Serial.println("ALready config");
    Serial.print("price : ");
    Serial.println(price);
    Serial.print("value : ");
    Serial.println(value);
    delay(2000);
  }
  else{
Serial.println("NOT CONFIGed");
    Serial.print("price : ");
    Serial.println(price);
    Serial.print("value : ");
    Serial.println(value);
  delay(2000);
  }

  // clearLog();
  // int address = 0; // Choose an appropriate address within the 4 KB space
  // String dataToWrite = "01231108040317+20+011LTP\x00";

  // // Write a string to EEPROM
  // writeData(address, dataToWrite);
  // writeData(findNextAvailableAddress(), "01231108040317+20+011LTP\x00");
  // writeData(findNextAvailableAddress(), "01231108040317+20+011LTP\x00");
  // writeData(findNextAvailableAddress(), "01231108040317+20+011LTP\x00");
  // writeData(findNextAvailableAddress(), "01231108040317+20+011LTP\x00");
  // // clearLog();

  // // Read a string from EEPROM
  // String dataRead = readData(address);
  // int nextAddress = findNextAvailableAddress();

  // Serial.print("String written: ");
  // Serial.println(dataToWrite);
  // Serial.print("String read: ");
  // Serial.println(dataRead);
  // Serial.print("Next Available Address: ");
  // Serial.println(nextAddress);
  // Serial.print("entry count: ");
  // Serial.println(nextAddress/dataToWrite.length());

 // report();
}
unsigned long c_time;
unsigned long p_time;

void loop() {

  c_time=millis();

  if(digitalRead(8)==0){
    Serial.println("ERASING DATATA");
    for(int i= 0 ; i< 4095; i++){
      EEPROM.write(i,0);
    }

  }

  if(c_time-p_time>5000){
    param.flag=1;
    param.price+=10;
    param.value+=100;
    Serial.print("param_price : ");
    Serial.println(param.price);
    Serial.print("param_value : ");
    Serial.println(param.value);
    p_time=millis();
    EEPROM.put(0,param);  
  }
  // Your code here
}
