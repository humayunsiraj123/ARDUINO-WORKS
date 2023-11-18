#include <TimeLib.h>
#include <TimeAlarms.h>
#include <Wire.h>
#include <DS1307RTC.h>  // a basic DS1307 library that returns time as a time_t
#include "<LiquidCrystal_I2C.h>"
#include <ezButton.h>

ezButton b1(2);
ezButton b2(3);
ezButton b3(4);
ezButton b4(5);
ezButton b5(6);
ezButton b6(7);
ezButton b7(8);


bool flag_conf=0;
const int motor = 12;
char crop_type[];
char crops[3][]= {"wheat","cotton","sugar"};
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
char soil[4][5]= {"SLS1","STLS2","SCLS3","LS004"};
char soil_type[];
int soil_index;
int crop_index;
//int button_array[7] = {2,3,4,5,6,7,8};//buttons
int max_th_array [3]={550,650,650};//wheat,cotton,sugar
int min_th_array [3][4] ={{220,330,320,310},
                    {220,330,320,310},
                    {220,330,320,310},
                    {220,220,320,310},};
                    
int moist_sensor[2] = {A0,A1};
LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS1307 rtc;
const int led= 13;
void setup() {
  Serial.begin(9600);
  // wait for Arduino Serial Monitor
  while (!Serial) ;

  
  b1.setDebounceTime(50);
  b2.setDebounceTime(50);
  b3.setDebounceTime(50);
  b4.setDebounceTime(50);
  b5.setDebounceTime(50);
  b6.setDebounceTime(50);
  b7.setDebounceTime(50);

  pinMode(moist_sensor_1,INPUT);
  pinMode(moist_sensor_2,INPUT);
  pinMode(motor,OUTPUT);
  digitalWrite(motor,1);
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("     WELCOM     ");
  lcd.setCursor(1,0);
  lcd.print("SMART IRRIGATION");
  delay(2000);
  lcd.clear();
  
  // get and set the time from the RTC
  setSyncProvider(RTC.get);   
  if (timeStatus() != timeSet) 
     Serial.println("Unable to sync with the RTC");
  else
     Serial.println("RTC has set the system time");     
  
  // to test your project, you can set the time manually 
  //setTime(8,29,0,1,1,11); // set time to Saturday 8:29:00am Jan 1 2011

  // create the alarms, to trigger functions at specific times
  Alarm.alarmRepeat(9,0,0,MorningAlarm);  // 9:00am every day
  Alarm.alarmRepeat(19,0,0,EveningAlarm);  // 19:00 -> 7:00pm every day
}

bool crop_flag=0;
bool soil_flag=0;
bool backup=0;
int  mean_moisture=0;

void loop() {
int moist_1 = analogRead(A0);
int moist_2 = analogRead(A1); 
int max_th;
int min_th;
mean_moisture = (moist_1 + moist_2)/2;
if(mean_moisture>1024)
{backup=1;
  }
else 
{backup=0;
  }
if(crop_flag==0){
//  lcd.setCursor(1,0);
//  lcd.print("");
  lcd.setCursor(0,0);
  lcd.print("SELECT CROP");

  if(b1.isPressed()){
   crop_flag==1;
   crop_type=crop[0]; 
   Serial.print("crop is WHEAT");
   lcd.setCursor(1,0);
   lcd.print("CROP : WHEAT");
   crop_index=0;
    }
  if(b2.isPressed()){
   crop_flag==1;
   crop_type=crop[1]; 
   Serial.print("crop is cotton"); 
   lcd.setCursor(1,0);
   lcd.print("CROP : COTTON");
   crop_index=1;
    }
    
    if(b3.isPressed()){
   crop_flag==1;
   crop_type=crop[2]; 
    Serial.print("crop is sugar");
    lcd.setCursor(1,0);
    lcd.print("CROP : SUGAR");
   crop_index=2;
   }
   
   }
   
    
else if (crop_flag==1 && soil_flag ==0){

  lcd.setCursor(0,0);
  lcd.print("SELECT SOIL");

  if(b4.isPressed()){
   soil_flag=1;
   soil_type=soil[0]; 
   lcd.setCursor(1,0);
   Serial.print("SOIL is sandy loam soil");
   lcd.print("SANDY_LOAM_SOIL");
   soil_index=0;
   
    }
  if(b5.isPressed()){
   soil_flag=1;
   soil_type=soil[1]; 
   lcd.setCursor(1,0);
   Serial.print("SOIL is SILT loam soil");
   lcd.print("SILT_LOAM_SOIL");
   soil_index=1;
    }
    
    if(b6.isPressed()){
   soil_flag=1;
   soil_type=soil[2]; 
   lcd.setCursor(1,0);
   Serial.print("SOIL is sandy clay loam soil");
   lcd.print("SANDY_CLAY_SOIL");
   soil_index=2;
   }

   if(b7.isPressed()){
   soil_flag=1;
   soil_type=soil[3]; 
   lcd.setCursor(1,0);
   Serial.print("SOIL is  loam soil");
   lcd.print("LOAM_SOIL");
   soil_index=3;
   }
   setCursor(0,0);
   lcd.print(crop_type);
  }

  max_th = max_th_array[crop_index];
  min_th = min_th_array[crop_index][soil_index];
  Serial.print("cropIndex");
  Serial.println(crop_index);
  Serial.print("soilIndex");
  Serial.println(soil_index);
  Serial.print("max_th");
  Serial.println(max_th);
  Serial.print("min_th");
  Serial.println(min_th);

if((mean_moist > max_th) && backup==0){
    digitalWrite(motor,0);//motor on
    Serial.println("PUMP is on .........");
  } 
  else if((mean_moist < min_th)&& backup==1){
    digitalWrite(motor,1);//motor off
    Serial.println("pump is off");
  }
  
}

// functions to be called when an alarm triggers
void MorningAlarm() {
  // write here the task to perform every morning
  Serial.println("Tturn light off");
  digitalWrite(led, LOW);
}
void EveningAlarm() {
  // write here the task to perform every evening
  Serial.println("Turn light on");
  digitalWrite(led, HIGH);
}

void digitalClockDisplay() {
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.println();
}
void printDigits(int digits) {
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
