byte receive_data[800];
byte receive_decode[8];

uint16_t pressure = 0;
uint16_t voltage=0;
uint16_t acquisition = 0;
uint16_t current = 0;
uint16_t soc = 0;

uint16_t max_mono_V = 0;
uint8_t max_unitV_cellN = 0;
uint16_t mini_mono_V = 0;
uint8_t mini_unitV_cellN = 0;

int16_t max_mono_T = 0;
uint16_t max_temp_cellN = 0;
int16_t min_mono_T = 0;
uint16_t min_temp_cellN = 0;

uint8_t charge_discharge = 0;
uint8_t charge_MOS_STUS = 0;
uint8_t dischage_MOS_STUS = 0;
uint8_t BMS_life = 0;
uint32_t residual_capacity = 0;

uint8_t battery_string = 0;
uint8_t temperature = 0;
uint8_t charger_STUS = 0;
uint8_t load_STUS = 0;
uint16_t charge_discharge_cycle = 0;
uint32_t  capacity_remaining=0;
boolean DI1 = 0, DI2 = 0, DI3 = 0, DI4 = 0;
boolean DO1 = 0, DO2 = 0, DO3 = 0, DO4 = 0;

uint16_t vCells_1_to_16[17] = {0};

uint8_t mono_temp = 0;


union
{
  struct {
    bool bit7: 1;
    bool bit6: 1;
    bool bit5: 1;
    bool bit4: 1;
    bool bit3: 1;
    bool bit2: 1;
    bool bit1: 1;
    bool bit0: 1;
  } fields;
  uint8_t val;
} State;


void send_msg1() {

  byte message1[] = {0xA5, 0x40, 0x90, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7D};
  for (uint8_t i = 0; i < sizeof(message1); i++)
  {
    Serial.write(message1[i]);
  }
}

void MOS_ON_DIS() {

  byte message1[] = {0xA5, 0x40, 0xD9, 0x08, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x84};
  for (uint8_t i = 0; i < sizeof(message1); i++)
  {
    Serial.write(message1[i]);
  }
//Serial.print(message1);
}

void MOS_OFF_DIS() {

  byte message1[] = {0xA5, 0x40, 0xD9, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x84};
  for (uint8_t i = 0; i < sizeof(message1); i++)
  {
    Serial.write(message1[i]);
    
  }
  //Serial.print(message1);
}
void MOS_ON_CHRG() {

  byte message1[] = {0xA5, 0x40, 0xD9, 0x08, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x85};
  for (uint8_t i = 0; i < sizeof(message1); i++)
  {
    Serial.write(message1[i]);
  }
//Serial.print(message1);
}

void MOS_OFF_CHRG() {

  byte message1[] = {0xA5, 0x40, 0xD9, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x85};
  for (uint8_t i = 0; i < sizeof(message1); i++)
  {
    Serial.write(message1[i]);
  }
//Serial.print(message1);

}



void send_msg2() {

  byte message2[] = {0xA5, 0x40, 0x91, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E};
  for (uint8_t i = 0; i < sizeof(message2); i++)
  {
    Serial.write(message2[i]);
  }
}

void send_msg3() {

  byte message3[] = {0xA5, 0x40, 0x92, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F};
  for (uint8_t i = 0; i < sizeof(message3); i++)
  {
    Serial.write(message3[i]);
  }
}

void send_msg4() {

  byte message4[] = {0xA5, 0x40, 0x93, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80};
  for (uint8_t i = 0; i < sizeof(message4); i++)
  {
    Serial.write(message4[i]);
  }
}

void send_msg5() {

  byte message5[] = {0xA5, 0x40, 0x94, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81};
  for (uint8_t i = 0; i < sizeof(message5); i++)
  {
    Serial.write(message5[i]);
  }
}

void send_msg6() {

  byte message8[] = {0xA5, 0x40, 0x95, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x82};
  for (uint8_t i = 0; i < sizeof(message8); i++)
  {
    Serial.write(message8[i]);
  }
}

void send_msg7() {

  byte message7[] = {0xA5, 0x40, 0x96, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x83};
  for (uint8_t i = 0; i < sizeof(message7); i++)
  {
    Serial.write(message7[i]);
  }
}



void setup() {

  Serial.begin(9600);
  Serial.println("Ready");
  send_msg1();
}

void loop() {
  if (Serial.available())
  {
    Serial.readBytesUntil('\n', receive_data, 13);

    switch (receive_data[2])
    {
      case (0x90):
        memcpy (receive_decode, receive_data + 4, 8);

        pressure= (receive_decode[0] << 8) | receive_decode[1];
        acquisition = (receive_decode[2] << 8) | receive_decode[3];
        current = (receive_decode[4] << 8) | receive_decode[5];
        soc = (receive_decode[6] << 8) | receive_decode[7];

        pressure = pressure * 0.1;
        voltage=pressure;
        current = current;
        soc = soc * 0.1;

        Serial.print("0X90: ");
        Serial.print("\n Voltage ");
        Serial.print(voltage);
        Serial.print(" V\n");
        //Serial.print(acquisition);
       // Serial.print(",");*/
        Serial.print("\nCurrent =");
        Serial.print(30000-current);
        Serial.print(" A \n");
        Serial.print("SOC = ");
        Serial.print(soc);
        Serial.println(" %");

        delay(20);
        send_msg2();
        MOS_OFF_CHRG();
MOS_OFF_DIS();

        break;


      case (0x91):
        memcpy (receive_decode, receive_data + 4, 8);

        max_mono_V = (receive_decode[0] << 8) | receive_decode[1];
        max_unitV_cellN = receive_decode[2];
        mini_mono_V = (receive_decode[3] << 8) | receive_decode[4];
        mini_unitV_cellN = receive_decode[5];

        Serial.print("0X91: ");
        Serial.print(" \nMAX Volt =");
        Serial.print(float(max_mono_V)/1000.0);
        Serial.print(" V\n");
        //Serial.print(max_unitV_cellN);
        //Serial.print(",");
        Serial.print("MIN Volt=");
        Serial.print(float(mini_mono_V)/1000.0);
        Serial.print("V \n");
        //Serial.print(mini_unitV_cellN);
        //Serial.println(" ");


        delay(20);
        //send_msg3();
        MOS_ON_DIS();
        MOS_OFF_CHRG();
        send_msg3();
        break;

      case (0x92):
        memcpy (receive_decode, receive_data + 4, 8);

        max_mono_T = receive_decode[0];
        max_temp_cellN = receive_decode[1];
        min_mono_T = receive_decode[2];
        min_temp_cellN = receive_decode[3];
        
        Serial.print("0X92: ");
        Serial.print(" \nMAX TEMP =");
        Serial.print(max_mono_T-40);
        Serial.print(" C\n");
        //Serial.print(max_temp_cellN);
        Serial.print(" MIN TEMP= ");
        Serial.print(min_mono_T-40);
        Serial.print(" C\n");
        //Serial.print(min_temp_cellN);
        //Serial.println(" ");


        delay(20);
        
        send_msg4();
        
        break;

      case (0x93):
        memcpy (receive_decode, receive_data + 4, 8);

        charge_discharge = receive_decode[0];
        charge_MOS_STUS =  receive_decode[1];
        dischage_MOS_STUS =  receive_decode[2];
        BMS_life =  receive_decode[3];
        capacity_remaining=receive_decode[4]<<24|receive_decode[5]<<16|receive_decode[6]<<8|receive_decode[7];
        //residual_capacity = ((receive_decode[4] << 24) | (receive_decode[5] << 16)) | ((receive_decode[6] << 8) | (receive_decode[7]));
        ///Serial.println(((receive_decode[4] << 24) | (receive_decode[5] << 16)));

        //uint16_t myVar = ((receive_decode[6] << 8) | (receive_decode[7]));

        Serial.print("0X93: ");
        Serial.print("/nBattery State ");
        if(charge_discharge==0)
          {Serial.print("Stationery ");}
        else if(charge_discharge==1)
          {Serial.print("charging");}
        else if(charge_discharge==2)
          {Serial.print("Discahrge");}
        
        Serial.print("\nCHARGE MOS STATE  ");  
        Serial.print(charge_MOS_STUS);
        Serial.print("\nDISCHARGE MOS STATE ");
        Serial.print(dischage_MOS_STUS);
        Serial.print("\n BMS_LIFE CYCLE  ");
        Serial.print(BMS_life);
        Serial.println(" Cycles\n");
        Serial.print("\nRemaining capacity =");
        Serial.print(float(capacity_remaining)/1000);
        Serial.print(" AH\n");
        //Serial.println();


        delay(20);
        send_msg1();
        break;

      case (0x94):
        memcpy (receive_decode, receive_data + 4, 8);

        battery_string = receive_decode[0];
        temperature = receive_decode[1];
        charger_STUS = receive_decode[2];
        load_STUS = receive_decode[3];
        State.val = receive_decode[4];

        DI1 = State.fields.bit0;
        DI2 = State.fields.bit1;
        DI3 = State.fields.bit2;
        DI4 = State.fields.bit3;
        DO1 = State.fields.bit4;
        DO2 = State.fields.bit5;
        DO3 = State.fields.bit6;
        DO4 = State.fields.bit7;

        charge_discharge_cycle = (receive_decode[5] << 8) | receive_decode[6];

        Serial.print("0X94: ");
        Serial.print(battery_string);
        Serial.print(", ");
        Serial.print(temperature);
        Serial.print(", ");
        Serial.print(charger_STUS);
        Serial.print(", ");
        Serial.print(load_STUS);
        Serial.print(", ");
        Serial.print("(");
        Serial.print(DI1);
        Serial.print(", ");
        Serial.print(DI2);
        Serial.print(", ");
        Serial.print(DI3);
        Serial.print(", ");
        Serial.print(DI4);
        Serial.print(", ");
        Serial.print(DO1);
        Serial.print(", ");
        Serial.print(DO2);
        Serial.print(", ");
        Serial.print(DO3);
        Serial.print(", ");
        Serial.print(DO4);
        Serial.print(")");
        Serial.print(", ");
        Serial.println(charge_discharge_cycle);

        delay(20);
        //send_msg6();
        break;

      case (0x95):

        memcpy(receive_decode, receive_data + 4, 8);
        uint8_t voltage_flag = 0;

        if (receive_decode[0] == 0x01)
        {
          vCells_1_to_16[0]  = (receive_decode[1] << 8) | receive_decode[2];
          vCells_1_to_16[1]  = (receive_decode[3] << 8) | receive_decode[4];
          vCells_1_to_16[2]  = (receive_decode[5] << 8) | receive_decode[6];
        }
        else if (receive_decode[0] == 0x02)
        {
          vCells_1_to_16[3]  = (receive_decode[1] << 8) | receive_decode[2];
          vCells_1_to_16[4]  = (receive_decode[3] << 8) | receive_decode[4];
          vCells_1_to_16[5]  = (receive_decode[5] << 8) | receive_decode[6];
        }
        else if (receive_decode[0] == 0x03)
        {
          vCells_1_to_16[6]  = (receive_decode[1] << 8) | receive_decode[2];
          vCells_1_to_16[7]  = (receive_decode[3] << 8) | receive_decode[4];
          vCells_1_to_16[8]  = (receive_decode[5] << 8) | receive_decode[6];
        }
        else if (receive_decode[0] == 0x04)
        {

          vCells_1_to_16[9]  = (receive_decode[1] << 8) | receive_decode[2];
          vCells_1_to_16[10]  = (receive_decode[3] << 8) | receive_decode[4];
          vCells_1_to_16[11]  = (receive_decode[5] << 8) | receive_decode[6];
        }
        else if (receive_decode[0] == 0x05)
        {
          vCells_1_to_16[12]  = (receive_decode[1] << 8) | receive_decode[2];
          vCells_1_to_16[13]  = (receive_decode[3] << 8) | receive_decode[4];
          vCells_1_to_16[14]  = (receive_decode[5] << 8) | receive_decode[6];
        }
        else if (receive_decode[0] == 0x06)
        {
          vCells_1_to_16[15]  = (receive_decode[1] << 8) | receive_decode[2];
          voltage_flag = 1;
        }

        if (voltage_flag) {
          for (byte i = 0; i < 16 ; i++)
          {
            Serial.print(vCells_1_to_16[i]);
            Serial.print(",");
          }
          voltage_flag = 0;
          delay(20);
          send_msg7();
        }
        break;

      case (0x96):
        Serial.println("MSG = 0x96");
        memcpy(receive_decode, receive_data + 4, 8);

        if (receive_decode[0] == 0x01)
        {
          //        mono_temp = receive_decode[0];
          //        mono_temp = mono_temp - 40;
          //        Serial.println(mono_temp,HEX);
          for (uint8_t i = 0; i < sizeof(receive_decode); i++)
          {
            Serial.print(receive_decode[i]);
          }
        }

        else if (receive_decode[0] == 0x02)
        {
          for (uint8_t i = 0; i < sizeof(receive_decode); i++)
          {
            Serial.print(receive_decode[i]);
          }
        }
        delay(20);
        send_msg1();
        break;
    }

  }

}
