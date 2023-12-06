#include <Wire.h>
//#include "Time.h"
#include "RTClib.h"

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
bool daily_motor=0;
DateTime now;
unsigned long start_time;
unsigned long curr_time;
unsigned char Buffer[9];
//DATA arrays
int moist_sensor[2] = {A0,A1};
String crops[4]= {"wheat","rice","cotton","sugar"};
String daysOfTheWeek[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
String soil[4]= {"SLS1","STLS2","SCLS3","LS004"};
//int button_array[7] = {2,3,4,5,6,7,8};//buttons
int max_th_array [4]={22,33,32,31};//wheat,cotton,sugar
// soil minimum threshold array for wrt crop and soil type
int min_th_array [4][4] ={{15,22,25,22},
                    {14,20,24,20},
                    {14,20,24,21},
                   {14,20,24,20},};

//3d array index [crop][soil][day_of_irrigation] 
int end_event[4]={240,120,204,365};
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



unsigned char tft_days[8]= {0x5a, 0xa5, 0x05, 0x82,0x90 , 0x00, 0x00, 0x00};
unsigned char tft_pump[8]= {0x5a, 0xa5, 0x05, 0x82,0x91 , 0x00, 0x00, 0x00};
unsigned char tft_backup[8]= {0x5a, 0xa5, 0x05, 0x82,0x92 , 0x00, 0x00, 0x00};
unsigned char tft_moisture[8]= {0x5a, 0xa5, 0x05, 0x82,0x93 , 0x00, 0x00, 0x00};

//lib function initialization
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
  Serial.begin(115200);
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
  
  //interupt for water level_sensor
  attachInterrupt(digitalPinToInterrupt(level_sensor), level_sensor_call, RISING);  

  //now=rtc.now();
  //start_time=now.unixtime();
  analogWrite(11,8);
  delay(1000);
}

unsigned long time;
unsigned long daily_irrigation;
bool stop;
int prex;
int air_val = 550;
int water_val =440;
int moist_1 ; 
int moist_2 ; 

int tft_crop;
int tft_soil;
void loop() {
//rtc days counting
//analogWriteResolution(4);
if(stop==0){

if(((millis()-time)>1000)&&(soil_flag==1 && crop_flag==1)){
  time=millis();
now= rtc.now();
curr_time=now.unixtime();
days_count++;

  tft_days[6]=highByte(days_count);
  tft_days[7]=lowByte(days_count);
  Serial.write(tft_days,8);
  
if(days_count> end_event[crop_index]){
  stop=1;
  
  
}

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
 

if(days_count-pre_day>1){
pre_day = days_count;
}



if(crop_flag==0){

if(Serial.available())
  {
    for(int i=0;i<=8;i++)   //TO store whole frame in buffer array. 0X5A A5 06 83 55 00 01 00 01 For ON
    {
    Buffer[i]= Serial.read();
    }
    
    if(Buffer[0]==0X5A)
      {
        if(Buffer[4]==0X55)
        tft_crop =Buffer[8];
        Serial.print("TFT CROP");
        Serial.println(tft_crop);
        
        }
      delay(50);
      }
       

  //crop selection
  if((tft_crop==1)){
   crop_flag=1;
   crop_type=crops[0]; 
   Serial.println("crop is WHEAT");
   crop_index=0;
   Serial.println("SELECT SOIL");
    }

   if((tft_crop==2)){
    crop_flag=1;
    crop_type=crops[1]; 
    Serial.println("RICE CROP");
    crop_index=1;
    Serial.println("SELECT SOIL");
   }
    
  if((tft_crop==3)){
   crop_flag=1;
   crop_type=crops[2]; 
   Serial.println("crop is cotton"); 
   crop_index=2;
     Serial.println("SELECT SOIL");
    }
    
    if((tft_crop==4)){
    crop_flag=1;
    crop_type=crops[2]; 
    Serial.println("crop is sugar");
   crop_index=3;
   Serial.println("SELECT SOIL");
   }

   }
   
  //selection of soilt type only after crops crop selection  
 if (crop_flag==1 && soil_flag ==0){

if(Serial.available())
  {
    for(int i=0;i<=8;i++)   //TO store whole frame in buffer array. 0X5A A5 06 83 55 00 01 00 01 For ON
    {
    Buffer[i]= Serial.read();
    }
    
    if(Buffer[0]==0X5A)
      {
        if(Buffer[4]==0X61)
        tft_soil =Buffer[8];
        Serial.print("TFT soil");
        Serial.println(tft_soil);
        
        }
      delay(50);
      }

  
  if((tft_soil==1)){
   soil_flag=1;
   soil_type=soil[0]; 
   Serial.println("SOIL is sandy loam soil");
   soil_index=0;
   //lcd.print(String(crops[crop_index]));
   
    }
  if((tft_soil==2)){
   soil_flag=1;
   soil_type=soil[1]; 
   Serial.println("SOIL is SILT loam soil");
   soil_index=1;
   //lcd.print(String(crops[crop_index]));
    }
    
    if((tft_soil==3)){
   soil_flag=1;
   soil_type=soil[2]; 
   Serial.println("SOIL is sandy clay loam soil");
   soil_index=2;
   //lcd.print(String(crops[crop_index]));
   }

   if( (tft_soil==4)){
   soil_flag=1;
   soil_type=soil[3]; 
   Serial.println("SOIL is  loam soil");
   soil_index=3;
   //lcd.print(String(crops[crop_index]));
   }
  }

//selection threshold from soil moisture based irrigation
max_th = max_th_array[crop_index];
min_th = min_th_array[crop_index][soil_index];

  // irrigation main system based on soil_mositure reading
if(((days_count - daily_irrigation>1)|daily_motor==0)&&(soil_flag==1 && crop_flag==1) ){
int moist_x1 = analogRead(A0);
int moist_x2 = analogRead(A1);
 moist_1 = map(moist_x1,air_val,water_val,0,100); 
 moist_2 = map(moist_x2,air_val,water_val,0,100); 




mean_moist = (moist_1 + moist_2)/2;
//delay(100);
// lcd.setCursor(0,0);
// lcd.print("DAILY IRRIGATION");
// lcd.setCursor(0,1);

if((mean_moist>1000 | mean_moist<5 ))
{
  backup=1;
  tft_backup[7]=1;
  Serial.write(tft_backup,8);

// lcd.setCursor(0,0);
// lcd.print("ALERT BACKUP    ");
// lcd.setCursor(0,1);
// lcd.print("     SYSTEM     ");
  
  }

else 
{backup=0;
tft_backup[7]=0;
Serial.write(tft_backup,8);

lcd.setCursor(0,0);
lcd.print("DAILY IRRIGATION");
  }

if((mean_moist < min_th) && backup==0){
    digitalWrite(motor,0);//motor on
  daily_motor=1;
  tft_pump[7]=1;
  Serial.write(tft_pump,8);
  tft_moisture[7]=0;
  Serial.write(tft_moisture,8);
  
  } 

  else if((mean_moist > max_th)&& backup==0 && daily_motor==1){
    digitalWrite(motor,1);//motor off
    daily_irrigation = days_count;
    tft_pump[7]=0;
    Serial.write(tft_pump,8);
    tft_moisture[7]=1;
    Serial.write(tft_moisture,8);
  
    delay(50);
    // lcd.setCursor(0,0);
    // lcd.print("CROP IRRIGATION");
    // lcd.setCursor(0,1);
    // lcd.print("DAY : ");
    // lcd.print(days_count);
    daily_motor=0;
  }
  }

//backup system schedule baseed irrigation
if((soil_flag==1 && crop_flag==1)&&(days_count == event_day[crop_index][soil_index][day_index])){
  day_index++;
  backup_irrigate= 1 ;
  pulse=0;
  } 
 
 //flow meter water threshold selection based on soil and crop type 
water_level_th = event_level[crop_index][soil_index][day_index] ; 

//for debuggin only 
if((soil_flag==1 && crop_flag==1) &&(days_count-prex>2 )){
prex=days_count;

//Serial.print("Next irrigation on ");
//Serial.println(event_day[crop_index][soil_index][day_index]);
//Serial.print("water_level_th ");
//Serial.println(water_level_th);
//Serial.print("VOLUME ");
//Serial.println(volume);
//
//Serial.print("cropIndex ");
// Serial.println(crop_index);
// Serial.print("soilIndex ");
// Serial.println(soil_index);
// Serial.print("max_th ");
// Serial.println(max_th);
// Serial.print("min_th ");
// Serial.println(min_th);
// Serial.print("moist ");
// Serial.println(mean_moist);
// Serial.print("percentage sensor 1:");
//Serial.println(moist_1);
//Serial.print("percentage sensor 2 : ");
//Serial.println(moist_2);
// 
//  delay(1000);
  }

//flow meter volume

volume = (2.66*pulse);/////2.66 calibration factor and div by 1000 for mmm
//backup system motor on the scedule days and irrigating for water level threshold

if(volume<water_level_th && backup==1 &&  backup_irrigate==1)
{
digitalWrite(motor,0);//motor on
  tft_pump[7]=1;
  Serial.write(tft_pump,8);
  tft_moisture[7]=0;
  Serial.write(tft_moisture,8);
  }

else if(volume>water_level_th && backup==1)
{
  digitalWrite(motor,1);//motor off
    //Serial.println("PUMP is off .........");
tft_pump[7]=0;
  Serial.write(tft_pump,8);
  tft_moisture[7]=1;
  Serial.write(tft_moisture,8);
    backup_irrigate =1;
  } 
 
 }
 else if(stop==1){
 Serial.println("IRRIGATION COMPLETE");
 
 }
}

//function for flow meter pulse counting
void level_sensor_call(){
  pulse++;
  }
