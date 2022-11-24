/*============================ EG LABS ===================================//

Demonstration on how to use 16x2 LCD display with an arduino board

* LED anode attached to digital output 6
* LED cathode attached to ground through a 1K resistor

//============================ EG LABS ===================================*/

// include the library code:

// initialize the library with the numbers of the interface pins


long randNumber;                                                            // the variable which is supposed to hold the random number

void setup()
{
  // set up the LCD's number of columns and rows:
  // initialize the serial port
  Serial.begin(9600);
  // initialize the pseudo-random number generator
 // randomSeed(analogRead(0));
}

void loop()
{ byte test[8]={};

  //randNumber = random(0x00, 0xff);                                              // generate a random number
  for (int i=0;i<8;i++){
    
  randNumber = random(0x00, 0xff); 
    test[i]=randNumber;
    }

    delay(500);
    
  for (int i=0;i<8;i++){
    Serial.print("the index is at");
    Serial.println(i); 
    Serial.println(test[i]);  
   Serial.println("---------");  
   Serial.println(test[i],HEX); 
   }
    
    
                                               // send the random number to the serial port
 // analogWrite(analogOutPin, randNumber);                                    // vary the brightness of the LED according to the random number
  delay(1000);
}
