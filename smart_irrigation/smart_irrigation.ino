#include <TimeLib.h>
#include <TimeAlarms.h>
#include <Wire.h>
//#include <DS1307RTC.h>  // a basic DS1307 library that returns time as a time_t
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
#include <ezButton.h>

//
ezButton b1(3);
ezButton b2(4);
ezButton b3(5);
ezButton b4(6);
ezButton b5(7);
ezButton b6(8);
ezButton b7(9);
//const int b1=3;
//const int b2=4;
//const int b3=5;
//const int b4=6;
//const int b5=7;
//const int b6=8;
//const int b7=3;


//Epoch timestamp: 946684800
//Timestamp in milliseconds: 946684800000
//Date and time (GMT): Saturday, 1 January 2000 00:00:00
//Date and time (your time zone): Saturday, 1 January 2000 05:00:00 GMT+05:00
const int level_sensor =2;
volatile long pulse;
unsigned long prev_time =0;
bool flag_conf=0;
const int motor = 12;
String crop_type;
String crops[3]= {"wheat","cotton","sugar"};
String daysOfTheWeek[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
String soil[4]= {"SLS1","STLS2","SCLS3","LS004"};
String soil_type;
int soil_index;
int crop_index;
//int button_array[7] = {2,3,4,5,6,7,8};//buttons
int max_th_array [3]={550,650,650};//wheat,cotton,sugar
int min_th_array [4][4] ={{220,330,320,310},
                    {220,330,320,310},
                    {220,330,320,310},
                    {220,220,320,310},};
                    
int event_day[3][4][15]={
                      {{15,37,51,73,97,129,185,199,212,229},
                       {27,111,150,187,199,220},
                       {13,21,62,107,133,151,168,183,195,208,221},
                       {19,59,117,148,179,190,224},},
                       
                       {{95,126,148,168,97,191},
                       {112,148,168,179},
                       {91,119,140,159,178},
                       {103,137,161,188},},

                       {{98, 131, 153, 171, 188, 202, 215,228, 253, 337},
                       {116, 153,180, 202, 223, 224,363},
                       {95, 126, 147, 164, 179, 183, 205, 218, 229, 243, 333},
                       {107, 143 , 165, 186, 203,  220, 238, 348},}
                       };
                       
//int event_day[4][20]={{172,172,172,172,172,172},
//                       {256,256,256,256},
//                       {152,152,152,152,152},
//                       {208,208,208,208},
//                       };
//                       
//int event_day[4][20]={{194,194, 194, 194, 194, 194, 194,194, 194, 194},
//                       {293, 293,293, 293, 293, 293,293},
//                       {174, 174, 174, 174, 174, 174, 174, 174, 174, 174,174},
//                       {237, 237 , 237, 237, 237,  237, 237, 237},
//                       };


                       

int event_level[3][4][15]={ 
                        {{42,60,111,139,158,173,175,175,175,175},
                        {79,186,236,263,268,263},
                        {13,21,62,107,133,151,168,183,195,208,221},
                        {19,59,117,148,179,190,224},},
                       
                       {{172,172,172,172,172,172},
                       {256,256,256,256},
                       {152,152,152,152,152},
                       {208,208,208,208},},
                       
                       {{194,194, 194, 194, 194, 194, 194,194, 194, 194},
                       {293, 293,293, 293, 293, 293,293},
                       {174, 174, 174, 174, 174, 174, 174, 174, 174, 174,174},
                       {237, 237 , 237, 237, 237,  237, 237, 237},}
                       };
  
                      
int moist_sensor[2] = {A0,A1};
int day_index=0;
LiquidCrystal_I2C lcd(0x20, 16, 2);
RTC_DS1307 rtc;
const int led= 13;
//unsigned long prev_time=0;

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
 pinMode(level_sensor,INPUT);
 pinMode(moist_sensor[0],INPUT);
  pinMode(moist_sensor[1],INPUT);
  pinMode(motor,OUTPUT);
  digitalWrite(motor,1);
  attachInterrupt(digitalPinToInterrupt(level_sensor), level_sensor_call, RISING);  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("     WELCOM     ");
  lcd.setCursor(0,1);
  lcd.print("SMART IRRIGATION");
  delay(2000);
  lcd.clear();
  Serial.println("SELECT CROP");
  
      
}

bool crop_flag=0;
bool soil_flag=0;
bool backup=1;
int  mean_moist=0;
int max_th;
int min_th;
int days_count;
int new_day;
int backup_irrigate=0;
int water_level_th=0;
int volume;
//int day_index;
int pre_day =0 ;
int prex;
void loop() {
  
  b1.loop();
  b2.loop();
  b3.loop();
  b4.loop();
  b5.loop();
  b6.loop();
  b7.loop();
  
int moist_1 = analogRead(A0);
int moist_2 = analogRead(A1);
mean_moist = (moist_1 + moist_2)/2;

//unsigned long theDateWeGotTogether =946684800;//1jan2000 //1194242400   ;  //in unixtime
days_count=millis()/2000;//
if(days_count-pre_day>1){
Serial.print("DAY: ");
Serial.println(days_count);
pre_day =days_count;
}

//if(mean_moist>1024)
//{backup=1;
//  }
//else 
//{backup=0;
//  }
if(crop_flag==0){
//  lcd.setCursor(0,1);
//  lcd.print("");
  lcd.setCursor(0,0);
  lcd.println("SELECT CROP");
  //Serial.println("SELECT CROP");

  if(b1.isPressed()){
   crop_flag=1;
   crop_type=crops[0]; 
   Serial.println("crop is WHEAT");
   lcd.setCursor(0,0);
   lcd.println("CROP : WHEAT");
   crop_index=0;
     Serial.println("SELECT SOIL");
    }
    
  if(b2.isPressed()){
   crop_flag=1;
   crop_type=crops[1]; 
   Serial.println("crop is cotton"); 
   lcd.setCursor(0,0);
   lcd.println("CROP : COTTON");
   crop_index=1;
     Serial.println("SELECT SOIL");
    }
    
    if(b3.isPressed()){
   crop_flag=1;
   crop_type=crops[2]; 
    Serial.println("crop is sugar");
    lcd.setCursor(0,0);
    lcd.println("CROP : SUGAR");
   crop_index=2;
   Serial.println("SELECT SOIL");
   }
   //Serial.println("SELECT SOIL");
   }
   
    
 if (crop_flag==1 && soil_flag ==0){

  lcd.setCursor(0,1);
  lcd.print("SELECT SOIL");
//Serial.println("SELECT SOIL");
  if(b4.isPressed()){
   soil_flag=1;
   soil_type=soil[0]; 
   lcd.setCursor(0,1);
   Serial.println("SOIL is sandy loam soil");
   lcd.print("SANDY_LOAM_SOIL");
   soil_index=0;
   lcd.setCursor(0,0);
   lcd.print(String(crops[crop_index]));
   
    }
  if(b5.isPressed()){
   soil_flag=1;
   soil_type=soil[1]; 
   lcd.setCursor(0,1);
   Serial.println("SOIL is SILT loam soil");
   lcd.print("SILT_LOAM_SOIL");
   soil_index=1;
   lcd.setCursor(0,0);
   lcd.print(String(crops[crop_index]));
    }
    
    if(b6.isPressed()){
   soil_flag=1;
   soil_type=soil[2]; 
   lcd.setCursor(0,1);
   Serial.println("SOIL is sandy clay loam soil");
   lcd.print("SANDY_CLAY_SOIL");
   soil_index=2;
   lcd.setCursor(0,0);
   lcd.print(String(crops[crop_index]));
   }

   if(b7.isPressed()){
   soil_flag=1;
   soil_type=soil[3]; 
   lcd.setCursor(0,1);
   Serial.println("SOIL is  loam soil");
   lcd.print("LOAM_SOIL");
   soil_index=3;
   lcd.setCursor(0,0);
   lcd.print(String(crops[crop_index]));
   }
  }

  max_th = max_th_array[crop_index];
  min_th = min_th_array[crop_index][soil_index];
  

//if((mean_moist > max_th) && backup==0){
//    digitalWrite(motor,0);//motor on
//    Serial.println("PUMP is on .........");
//  } 
//  else if((mean_moist < min_th)&& backup==0){
//    digitalWrite(motor,1);//motor off
//    Serial.println("pump is off");
//  }

if( (soil_flag==1 && crop_flag==1)&&(days_count == event_day[crop_index][soil_index][day_index])){
  day_index++;
  backup_irrigate= 0 ;
  pulse=0;
  } 
 
water_level_th = event_level[crop_index][soil_index][day_index] ; 

if((soil_flag==1 && crop_flag==1) &&(days_count-prex>2 )){
prex=days_count;
Serial.print("day index");
Serial.println(day_index);
Serial.print("water_level_th");
Serial.println(water_level_th);
Serial.print("VOLUME");
Serial.println(volume);

//Serial.print("cropIndex");
//  Serial.println(crop_index);
//  Serial.print("soilIndex");
//  Serial.println(soil_index);
//  Serial.print("max_th");
//  Serial.println(max_th);
//  Serial.print("min_th");
//  Serial.println(min_th);
//  Serial.print("cropIndex");
//  Serial.println(crop_index);
//  Serial.print("soilIndex");
//  Serial.println(soil_index);
//  Serial.print("max_th");
//  Serial.println(max_th);
//  Serial.print("min_th");
//  Serial.println(min_th);
  //delay(000);
  }

volume = 2.5*pulse;

if(volume<water_level_th && backup==1 &&  backup_irrigate==0)
{
  digitalWrite(motor,0);//motor on
    Serial.println("PUMP is on .........");
  }

else if(volume>water_level_th && backup==1 )
{
  digitalWrite(motor,1);//motor off
    //Serial.println("PUMP is off .........");
    backup_irrigate =1;
  }
  
}


void level_sensor_call(){
  pulse++;
  }
