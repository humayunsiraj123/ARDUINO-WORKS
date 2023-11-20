#include <Wire.h>
//#include "Time.h"
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>
#include <ezButton.h>



ezButton b1(3);//wheat button
ezButton b2(4);//rice
ezButton b3(5);//cotton
ezButton b4(6);//sugar_cane
ezButton b5(7);//Sandy Loam Soil: 
ezButton b6(8);//Silt Loam soil :
ezButton b7(9);//Sandy clay loam soil:
ezButton b8(10);//Loam soil: 

//Timestamp in milliseconds: 946684800000

//pins assignment
const int level_sensor =2;
const int motor = 12;
const int led= 13;

//aux variable
String crop_type;
String soil_type;
int soil_index;
int crop_index;
int day_index=0;
bool crop_flag=0;
bool soil_flag=0;
bool backup=0;
int  mean_moist=0;
int max_th;
int min_th;
int days_count;
int new_day;
int backup_irrigate=0;
int water_level_th=0;
int volume;
int pre_day =0 ;
volatile long pulse;

DateTime now;
unsigned long start_time;
unsigned long curr_time;

//DATA arrays
int moist_sensor[2] = {A0,A1};
String crops[4]= {"wheat","rice","cotton","sugar"};
String daysOfTheWeek[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
String soil[4]= {"SLS1","STLS2","SCLS3","LS004"};
//int button_array[7] = {2,3,4,5,6,7,8};//buttons
int max_th_array [4]={550,670,650,650};//wheat,cotton,sugar
// soil minimum threshold array for wrt crop and soil type
int min_th_array [4][4] ={{220,330,320,310},
                    {220,330,320,310},
                    {220,330,320,310},
                   {220,220,320,310},};

//3d array index [crop][soil][day_of_irrigation] 
int event_day[4][4][15]={
                      {{15,37,51,73,97,129,185,199,212,229},
                       {27,111,150,187,199,220},
                       {13,21,62,107,133,151,168,183,195,208,221},
                       {19,59,117,148,179,190,224},},

                        {{5,11,18,31,60,96,115},
                       {9,24,63,106},
                       {4,9,15,24,44,79,99,116},
                       {6,14,29,62,100},},
                       
                       {{95,126,148,168,97,191},
                       {112,148,168,179},
                       {91,119,140,159,178},
                       {103,137,161,188},},

                       {{98, 131, 153, 171, 188, 202, 215,228, 253, 337},
                       {116, 153,180, 202, 223, 224,363},
                       {95, 126, 147, 164, 179, 183, 205, 218, 229, 243, 333},
                       {107, 143 , 165, 186, 203,  220, 238, 348},}
                       };
//3d array index [crop][soil][water_level_in mm] 
int event_level[4][4][15]={ 
                        {{42,60,111,139,158,173,175,175,175,175},
                        {79,186,236,263,268,263},
                        {13,21,62,107,133,151,168,183,195,208,221},
                        {19,59,117,148,179,190,224},},
                        
                       {{17,28,39,64,94,94,94},
                       {36,74,142,142},
                       {14,22,30, 44, 73,84,84,84},
                       {23,40,70,115,115},},
                       
                       {{172,172,172,172,172,172},
                       {256,256,256,256},
                       {152,152,152,152,152},
                       {208,208,208,208},},
                       
                       {{194,194, 194, 194, 194, 194, 194,194, 194, 194},
                       {293, 293,293, 293, 293, 293,293},
                       {174, 174, 174, 174, 174, 174, 174, 174, 174, 174,174},
                       {237, 237 , 237, 237, 237,  237, 237, 237},}
                       };

//lib function initialization
LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS1307 rtc;


void TimerOne_setPeriod(long OCRValue) 

{
  TCCR1B = _BV(WGM12)|_BV(CS12)|_BV(CS10);   // CTC mode - 1024 prescale
                                             //  TCCR1B = _BV(WGM12)|_BV(CS12)|_BV(CS11)|_BV(CS10); is the entire command but since CS11=0 its left out
                                             //  manipulates prescale (one of the parameters we can manipulate**we want to use prescales that yeields high ORC values for more reliable signals 
  TCCR1A = _BV(COM1A0);                      //  or use TCCR1A = 0x40;            // Toggle mode 0C1A  
                                             //  This is used to toggle on pin 11 ** pin 11 = 0 x 1 so COM(COM1A1) = 0 and is not in code
                                             //  **Determines pin behavor
  OCR1A = OCRValue;                          //  set the counter
}
unsigned long irrigate_day;
void setup() {
  Serial.begin(9600);
  Wire.begin();
  // wait for Arduino Serial Monitor
  while (!Serial) ;
 //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  //initialization and assignment

  pinMode(11, OUTPUT);
  pinMode(level_sensor,INPUT);
  pinMode(moist_sensor[0],INPUT);
  pinMode(moist_sensor[1],INPUT);
  pinMode(motor,OUTPUT);
  digitalWrite(motor,1);
  TimerOne_setPeriod(781.25);
  b1.setDebounceTime(50);
  b2.setDebounceTime(50);
  b3.setDebounceTime(50);
  b4.setDebounceTime(50);
  b5.setDebounceTime(50);
  b6.setDebounceTime(50);
  b7.setDebounceTime(50);
  b8.setDebounceTime(50);
   
  //interupt for water level_sensor
  attachInterrupt(digitalPinToInterrupt(level_sensor), level_sensor_call, RISING);  

//lcd initliazation
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("     WELCOME     ");
  lcd.setCursor(0,1);
  lcd.print("SMART IRRIGATION");
  delay(2000);
  lcd.clear();
  Serial.println("SELECT CROP");
  delay(1000);
  //now=rtc.now();
  //start_time=now.unixtime();
  analogWrite(11,8);
  delay(2000);
}

unsigned long time;
unsigned long daily_irrigation;

int prex;
void loop() {
//rtc days counting
//analogWriteResolution(4);

if((millis()-time)>3000){
  time=millis();
Serial.println(String(days_count));
now= rtc.now();
curr_time=now.unixtime();
days_count++;

}


//days_count = (rtc.now()-start_time)/3; // millis()/2000;//
// if((curr_time-start_time)>3000){
//   ++days_count;
//   now=rtc.now();
//   start_time=now.unixtime();
// }
//now=rtc.now();
//curr_time  = now.unixtime();
//days_count = (curr_time - start_time)/3 ; 
// 
 //buttons loopings 
  b1.loop();
  b2.loop();
  b3.loop();
  b4.loop();
  b5.loop();
  b6.loop();
  b7.loop();
  b8.loop();

// soilmoisture reading  



if(days_count-pre_day>1){
Serial.print("DAY: ");
Serial.println(days_count);
pre_day = days_count;
}

// if(mean_moist>1024 | mean_moist<0 )
// {backup=1;
//   }
// else 
// {backup=0;
//   }

if(crop_flag==0){
  lcd.setCursor(0,0);
  lcd.println("SELECT CROP");

  //crop selection
  if(b1.isPressed()){
   crop_flag=1;
   crop_type=crops[0]; 
   Serial.println("crop is WHEAT");
   lcd.setCursor(0,0);
   lcd.print("CROP : WHEAT");
   crop_index=0;
   Serial.println("SELECT SOIL");
    }

   if(b2.isPressed()){
    crop_flag=1;
    crop_type=crops[1]; 
    Serial.println("RICE CROP");
    lcd.setCursor(0,0);
    lcd.print("CROP : RICE");
    crop_index=1;
    Serial.println("SELECT SOIL");
   }
    
  if(b3.isPressed()){
   crop_flag=1;
   crop_type=crops[2]; 
   Serial.println("crop is cotton"); 
   lcd.setCursor(0,0);
   lcd.print("CROP : COTTON");
   crop_index=2;
     Serial.println("SELECT SOIL");
    }
    
    if(b4.isPressed()){
    crop_flag=1;
    crop_type=crops[2]; 
    Serial.println("crop is sugar");
    lcd.setCursor(0,0);
    lcd.print("CROP: SUGAR CANE");
   crop_index=3;
   Serial.println("SELECT SOIL");
   }

   }
   
  //selection of soilt type only after crops crop selection  
 if (crop_flag==1 && soil_flag ==0){

  lcd.setCursor(0,1);
  lcd.print("SELECT SOIL");
  
  if(b5.isPressed()){
   soil_flag=1;
   soil_type=soil[0]; 
   lcd.setCursor(0,1);
   Serial.println("SOIL is sandy loam soil");
   lcd.print("SANDY_LOAM_SOIL");
   soil_index=0;
   lcd.setCursor(0,0);
   //lcd.print(String(crops[crop_index]));
   
    }
  if(b6.isPressed()){
   soil_flag=1;
   soil_type=soil[1]; 
   lcd.setCursor(0,1);
   Serial.println("SOIL is SILT loam soil");
   lcd.print("SILT_LOAM_SOIL");
   soil_index=1;
   lcd.setCursor(0,0);
   //lcd.print(String(crops[crop_index]));
    }
    
    if(b7.isPressed()){
   soil_flag=1;
   soil_type=soil[2]; 
   lcd.setCursor(0,1);
   Serial.println("SOIL is sandy clay loam soil");
   lcd.print("SANDY_CLAY_SOIL");
   soil_index=2;
   lcd.setCursor(0,0);
   //lcd.print(String(crops[crop_index]));
   }

   if(b8.isPressed()){
   soil_flag=1;
   soil_type=soil[3]; 
   lcd.setCursor(0,1);
   Serial.println("SOIL is  loam soil");
   lcd.print("LOAM_SOIL");
   soil_index=3;
   lcd.setCursor(0,0);
   //lcd.print(String(crops[crop_index]));
   }
  }

//selection threshold from soil moisture based irrigation
max_th = max_th_array[crop_index];
min_th = min_th_array[crop_index][soil_index];

  // irrigation main system based on soil_mositure reading
if((days_count - daily_irrigation>1)&&(soil_flag==1 && crop_flag==1) ){
int moist_1 = analogRead(A0);
int moist_2 = analogRead(A1);

mean_moist = (moist_1 + moist_2)/2;
//delay(100);
lcd.setCursor(0,0);
lcd.print("DAILY IRRIGATION");
lcd.setCursor(0,1);

if((mean_moist > max_th) && backup==0){
    digitalWrite(motor,0);//motor on
  lcd.print("pump ON          ");
  } 

  else if((mean_moist < min_th)&& backup==0){
    digitalWrite(motor,1);//motor off
    lcd.print("pump OFF        ");
    daily_irrigation = days_count;
    delay(50);
    lcd.setCursor(0,0);
    lcd.print("CROP IRRIGATION");
    lcd.setCursor(0,1);
    lcd.print("DAY : ");
    lcd.print(days_count);
  }
  }

//backup system schedule baseed irrigation
if((soil_flag==1 && crop_flag==1)&&(days_count == event_day[crop_index][soil_index][day_index])){
  day_index++;
  backup_irrigate= 0 ;
  pulse=0;
  } 
 
 //flow meter water threshold selection based on soil and crop type 
water_level_th = event_level[crop_index][soil_index][day_index] ; 

//for debuggin only 
if((soil_flag==1 && crop_flag==1) &&(days_count-prex>2 )){
prex=days_count;

Serial.print("Next irrigation on ");
Serial.println(event_day[crop_index][soil_index][day_index]);
Serial.print("water_level_th ");
Serial.println(water_level_th);
Serial.print("VOLUME ");
Serial.println(volume);

Serial.print("cropIndex ");
 Serial.println(crop_index);
 Serial.print("soilIndex ");
 Serial.println(soil_index);
 Serial.print("max_th ");
 Serial.println(max_th);
 Serial.print("min_th ");
 Serial.println(min_th);
 Serial.print("moist ");
 Serial.println(mean_moist);
 
  delay(1000);
  }

//flow meter volume

volume = (2.66*pulse)/10;//00;//2.66 calibration factor and div by 1000 for mmm
//backup system motor on the scedule days and irrigating for water level threshold

if(volume<water_level_th && backup==1 &&  backup_irrigate==0)
{
  digitalWrite(motor,0);//motor on
  }

else if(volume>water_level_th && backup==1)
{
  digitalWrite(motor,1);//motor off
    //Serial.println("PUMP is off .........");
    backup_irrigate =1;
  } 
 
}

//function for flow meter pulse counting
void level_sensor_call(){
  pulse++;
  }
