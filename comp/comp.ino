
void setup() {
  pinMode(9, OUTPUT); //output A
  pinMode(10, OUTPUT); //output B

  TCCR1A = 0; //clear timer registers
  TCCR1B = 0;
  TCNT1 = 0;

  //ICR1 and Prescaler sets frequency
  //no prescaler .0625 us per count @ 16Mh
  //80 counts x .0625 = 5 us = 200Khz

  TCCR1B |= _BV(CS10); //no prescaler
  ICR1 = 40;//PWM mode counts up and back down for 80 counts

  OCR1A = 21; //Pin 9 match
  //output A set rising/clear falling
  //Rise at TCNT 21 upslope, High 38 counts, Fall at TCNT 21 downslope
  //47,5% Duty Cycle Pulse centered on TCNT 40. High 38 Low 42
  TCCR1A |= _BV(COM1A1) | _BV(COM1A0); //output A set rising/clear falling

  OCR1B = 19; //Pin 10 match
  //output B clear rising/set falling
  //Fall at TCNT 19 upslope, Low 42, Rise at TCNT 19 downslope
  //47.5% Duty Cycle Pulse centered on TCNT 0. High 38 Low 42
  TCCR1A |= _BV(COM1B1); //output B clear rising/set falling

  TCCR1B |= _BV(WGM13); //PWM mode with ICR1 Mode 10
  TCCR1A |= _BV(WGM11); //WGM13:WGM10 set 1010

}

void loop() {}
