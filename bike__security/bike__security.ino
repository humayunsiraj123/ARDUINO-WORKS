#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2,3);
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
const byte numRows= 1; //number of rows on the keypad
const byte numCols= 4; //number of columns on the keypad
char keypressed;
char code[]={'1','2','3','4'}; //Passcode needed you can change it just keep the format "4 digits as char array" 
char c[4]; //Array to get the code from the user
int ij;
//keymap defines the key pressed according to the row and columns just as appears on the keypad
char keymap[numRows][numCols]=
{
{'1', '2', '3', '4'},
};
byte rowPins[numRows] = {4}; //Rows 0 //if you modify your pins you should modify this too
byte colPins[numCols]={8,7,6,5}; //Columns 0 to 3
//initializes an instance of the Keypad class
Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows,numCols); 

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

int a=0;
uint8_t id; 

#define enroll 9
#define del 10
#define up 11
#define down 12

void setup() {
  // put your setup code here, to run once:
pinMode(enroll, INPUT_PULLUP); //enroll button initially set at high 
pinMode(del, INPUT_PULLUP); // del button initially set at high
pinMode(down, INPUT_PULLUP); 
pinMode(up, INPUT_PULLUP);

pinMode (6,OUTPUT); //relay signal pin
digitalWrite(6,LOW);

//lcd.begin(16,2);
//lcd.print("Security System");

Serial.begin(9600);
while (!Serial); // For Yun/Leo/Micro/Zero/...
delay(100);
Serial.println("\n\nAdafruit finger detect test");
 
// set the data rate for the sensor serial port
finger.begin(57600);
 
if (finger.verifyPassword()) {
Serial.println("Found fingerprint sensor!");
} else {
Serial.println("Did not find fingerprint sensor :(");
while (1) { delay(1); }
}
 
finger.getTemplateCount();
Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
Serial.println("Waiting for valid finger...");
}


void loop() 
  // put your main code here, to run repeatedly:
{
getFingerprintIDez();
delay(50);

if(digitalRead(enroll) == 0)
{
    lcd.clear();
    lcd.print("Please Wait");
    delay(1000);
   { //If it's 'A' it triggers "Addingnew template" sequence
ij=0; //ij is set to 0
lcd.clear();
lcd.setCursor(0, 0);
lcd.write("ENROLL NEW USER");
lcd.setCursor(0, 1);
lcd.write("Read mode...");
delay(4000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Enter Password");
getPassword(); //Getting the passcodefunction
if(ij==4){ //If the passcode is correct

Enroll(); //Enrolling function
delay(2000);
lcd.clear();
}
else{ //If the code is wrong wecan't add new users (templates)
lcd.setCursor(0,0);
lcd.print("Wrong code");
delay(2000);
lcd.clear();
}
}
    
  }

else if(digitalRead(del) == 0)
   {
    lcd.clear();
    lcd.print("Please Wait");
    delay(1000);

    ij=0; //ij is set to 0
lcd.clear();
lcd.setCursor(0, 0);
lcd.write("DELET mode");
lcd.setCursor(0, 1);
lcd.write("Read mode...");
delay(4000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Enter Password");
getPassword(); //Getting the passcodefunction
if(ij==4){ //If the passcode is correct

delet(); //Enrolling function
delay(2000);
lcd.clear();
}
else{ //If the code is wrong wecan't add new users (templates)
lcd.setCursor(0,0);
lcd.print("Wrong code");
delay(2000);
lcd.clear();
}
}
   
   } 
//Reading the buttons typed by the keypad


void Enroll()
{
  
   int count=0;
   lcd.clear();
   lcd.print("Enroll Finger    ");
   lcd.setCursor(0,1);
   lcd.print("Location:");
   while(1)
   {
     lcd.setCursor(9,1);
     lcd.print(count);
     if(digitalRead(up) == 0)
     {
       count++;
       if(count>25)
       count=0;
       delay(500);
     }

     else if(digitalRead(down) == 0)
     {
       count--;
       if(count<0)
       count=25;
       delay(500);
     }
     else if(digitalRead(del) == 0)
     {
          id=count;
          getFingerprintEnroll();
          return;
     }

       else if(digitalRead(enroll) == 0)
     {        
          return;
     }
 }
}

void delet()
{
   int count=0;
   lcd.clear();
   lcd.print("Delete Finger    ");
   lcd.setCursor(0,1);
   lcd.print("Location:");
   while(1)
   {
     lcd.setCursor(9,1);
     lcd.print(count);
     if(digitalRead(up) == 0)
     {
       count++;
       if(count>25)
       count=0;
       delay(500);
     }

     else if(digitalRead(down) == 0)
     {
       count--;
       if(count<0)
       count=25;
       delay(500);
     }
     else if(digitalRead(del) == 0)
     {
          id=count;
          deleteFingerprint(id);
          return;
     }

       else if(digitalRead(enroll) == 0)
     {        
          return;
     }
 }
}

uint8_t getFingerprintEnroll()
{
  int p = -1;
  lcd.clear();
  lcd.print("finger ID:");
  lcd.print(id);
  lcd.setCursor(0,1);
  lcd.print("Place Finger");
  delay(2000);
  while (p != FINGERPRINT_OK)
  {
    p = finger.getImage();
    switch (p)
    {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      lcd.clear();
      lcd.print("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No Finger");
      lcd.clear();
      lcd.print("No Finger");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      lcd.clear();
      lcd.print("Comm Error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      lcd.clear();
      lcd.print("Imaging Error");
      break;
    default:
      Serial.println("Unknown error");
       lcd.clear();
      lcd.print("Unknown Error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      lcd.clear();
      lcd.print("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
       lcd.clear();
       lcd.print("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      lcd.clear();
      lcd.print("Comm Error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      lcd.clear();
      lcd.print("Feature Not Found");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      lcd.clear();
      lcd.print("Feature Not Found");
      return p;
    default:
      Serial.println("Unknown error");
      lcd.clear();
      lcd.print("Unknown Error");
      return p;
  }
  
  Serial.println("Remove finger");
  lcd.clear();
  lcd.print("Remove Finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
   lcd.clear();
      lcd.print("Place Finger");
      lcd.setCursor(0,1);
      lcd.print("   Again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      return;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    lcd.clear();
    lcd.print("Stored!");
    delay(2000);
    lcd.clear();
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  }
  else {
    Serial.println("Unknown error");
    return p;
  }   
}

uint8_t deleteFingerprint(uint8_t id) 
{
  uint8_t p = -1;  
  lcd.clear();
  lcd.print("Please wait");
  p = finger.deleteModel(id);
  if (p == FINGERPRINT_OK) 
  {
    Serial.println("Deleted!");
    lcd.clear();
    lcd.print("Figer Deleted");
    lcd.setCursor(0,1);
    lcd.print("Successfully");
    delay(1000);
  } 
  
  else 
  {
    Serial.print("Something Wrong"); 
    lcd.clear();
    lcd.print("Something Wrong");
    lcd.setCursor(0,1);
    lcd.print("Try Again Later");
    delay(2000);
    return p;
  }   
}





uint8_t getFingerprintID() {
uint8_t p = finger.getImage();
switch (p) {
case FINGERPRINT_OK:
Serial.println("Image taken");
break;
case FINGERPRINT_NOFINGER:
Serial.println("No finger detected");
return p;
case FINGERPRINT_PACKETRECIEVEERR:
Serial.println("Communication error");
return p;
case FINGERPRINT_IMAGEFAIL:
Serial.println("Imaging error");
return p;
default:
Serial.println("Unknown error");
return p;
}
 
// OK success!
 
p = finger.image2Tz();
switch (p) {
case FINGERPRINT_OK:
Serial.println("Image converted");
break;
case FINGERPRINT_IMAGEMESS:
Serial.println("Image too messy");
return p;
case FINGERPRINT_PACKETRECIEVEERR:
Serial.println("Communication error");
return p;
case FINGERPRINT_FEATUREFAIL:
Serial.println("Could not find fingerprint features");
return p;
case FINGERPRINT_INVALIDIMAGE:
Serial.println("Could not find fingerprint features");
return p;
default:
Serial.println("Unknown error");
return p;
}
  
// OK converted!
p = finger.fingerFastSearch();
if (p == FINGERPRINT_OK) {
lcd.clear();
lcd.print("Finger Matched");
Serial.println("Found a print match!");
} else if (p == FINGERPRINT_PACKETRECIEVEERR) {
Serial.println("Communication error");
return p;
} else if (p == FINGERPRINT_NOTFOUND) {
lcd.clear();
lcd.print("Did not find a match");
Serial.println("Did not find a match");
return p;
} else {
Serial.println("Unknown error");
return p;
}
 
// found a match!
Serial.print("Found ID #"); Serial.print(finger.fingerID);
Serial.print(" with confidence of "); Serial.println(finger.confidence);
 
return finger.fingerID;
}
 
// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
uint8_t p = finger.getImage();
if (p != FINGERPRINT_OK)  return -1;
p = finger.image2Tz();
if (p != FINGERPRINT_OK)  return -1;
p = finger.fingerFastSearch();
if (p != FINGERPRINT_OK)  return -1;
// found a match!
if (p == FINGERPRINT_OK && a==0){
  a=1;
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
Serial.print(" with confidence of "); Serial.println(finger.confidence);

digitalWrite(6, HIGH);
lcd.begin(16,2);
//lcd.clear();
//lcd.print("Ignition");
//lcd.setCursor(1,1);
//lcd.print("ON");


}
else{
digitalWrite(6, LOW);
//lcd.clear();
//lcd.print("Ignition");
//lcd.setCursor(1,1);
//lcd.print("OFF");
a=0;
}
return finger.fingerID;
  }

void getPassword(){
for (int i=0 ; i<4 ; i++){
c[i]= myKeypad.waitForKey();
lcd.setCursor(i,1);
lcd.print("*");
}
lcd.clear();
for (int j=0 ; j<4 ; j++){ //comparing the code entred with the code stored
if(c[j]==code[j])
ij++; //Everytime the char is correct we increment the ij until it reaches 4 which means the code is correct
} //Otherwise it won't reach 4 and it will show "wrong code" as written above
}
