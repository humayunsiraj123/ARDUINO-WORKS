int flag1=0;
void setup() {
  // put your setup code here, to run once:
pinMode(A0,INPUT);
  pinMode(D1,INPUT);
  pinMode(D2,OUTPUT);
  Serial.begin(9600);

}

  // put your setup code here, to run once:


void loop() {
  float value=analogRead(A0);
  float value1=digitalRead(D1);


  if(value1==1){
    Serial.println("highe");}
if(value1==0){
    Serial.println("loowww");}


Serial.print ("the READ VALUE IS ");

Serial.println(value);
delay(1000);}
