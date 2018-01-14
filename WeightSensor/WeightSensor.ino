/* This code will calibrate hx711 Load cell Amplifier
ADC -> Analog to Digital Converter


Arduino pin 6 -> HX711 CLK
Arduino pin 5 -> HX711 DOUT



#include "HX711.h" //The library is downloaded from https://github.com/bogde/HX711
#incldue <LiquidCrystal.h>

#define DOUT 5
#define CLK 6

unsigned long readCount(void)
{
  unsigned long Count;
  unsigned char i;
  //pinMode(DT,OUTPUT); //Set Data as Output
  digitalWrite(DT, High); //Set Data port to HIGH
  digitalWrite(SCK, Low); //Set Clock low
  Count=0; 
  pinMode(DT, Input); //Set Data Port as an Input
  while(digitalRead(DT)){
    for (i=0 ; i<24; i++)
    { // hx711 sends 24bits.
      // One tick will transmitte one bit
      digitalWrite(SCK, HIGH); 
      digitalWrite(SCK, LOW);
      Count++;
    }
  }
  digitalWrite(SCK,HIGH);
  Count = Count
}

float kg;
float units;

void setup() {
  // put your setup code here, to run once:
  //Initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("Weight Calibration Process");

}

void loop() {
  // put your main code here, to run repeatedly:
  scale.set_scale(2125); //2125 is the calibration factor
  
}

*/


#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 10, 11, 12, 13); // LiquidCrystal(rs, enable, d4, d5, d6, d7) 

#define DT A0
#define SCK A1
#define sw 2

long sample=0;
float val=0;
long count=0;

//readCount is from the datasheet Hx711,
unsigned long readCount(void)
{
  unsigned long Count;
  unsigned char i;
  
  digitalWrite(DT,HIGH);
  digitalWrite(SCK,LOW); //set clock to zero before starts
  Count=0;
  while(digitalRead(DT));
  for (i=0;i<24;i++)
  {
    digitalWrite(SCK,HIGH);
    Count=Count<<1;
    digitalWrite(SCK,LOW);
    if(digitalRead(DT)) 
    Count++;
  }
  digitalWrite(SCK,HIGH);
  Count=Count^0x800000;
  digitalWrite(SCK,LOW);
  return(Count);
}

void setup()
{
  Serial.begin(9600);
  pinMode(SCK, OUTPUT);
  pinMode(sw, INPUT_PULLUP);
  lcd.begin(0, 1);
  lcd.print("    Weight ");
  lcd.setCursor(0,1);
  lcd.print(" Measurement ");
  delay(500);
  lcd.clear();
  calibrate();
}

void loop()
{
  count= readCount();
  int w=(((count-sample)/val)-2*((count-sample)/val));
  Serial.print("weight:");
  Serial.print((int)w);
  Serial.println("g");
  lcd.setCursor(0,0);
  //lcd.print("Weight            ");
  lcd.setCursor(0,1);
  lcd.print(w);
  lcd.print("g             ");

  if(digitalRead(sw)==0)
  {
    val=0;
    sample=0;
    w=0;
    count=0;
    calibrate();
  }
}

void calibrate()
{
  lcd.clear();
  lcd.print("Calibrating...");
  delay(1000);
  lcd.setCursor(0,1);
  lcd.print("Please Wait...");
  for(int i=0;i<100;i++)
  {
    count=readCount();
    sample+=count;
    Serial.println(count);
  }
  sample/=100;
  Serial.print("Avg:");
  Serial.println(sample);
  lcd.clear();
  lcd.print("Put 100g & wait");
  count=0;
  while(count<100)
  {
    count=readCount();
    count=sample-count;
    Serial.println(count);
  }
  lcd.clear();
  lcd.print("Please Wait....");
  delay(2000);
  for(int i=0;i<100;i++)
  {
    count=readCount();
    val+=sample-count;
    Serial.println(sample-count);
  }
  val=val/100.0;
  val=val/100.0;        // put here your calibrating weight
  lcd.clear();
}
