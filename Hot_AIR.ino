#include<PID_v1.h>
#include <LiquidCrystal.h>
int pv,sp;
int Otemp = 26;
double Setpoint, Input, Output;
PID myPID(&Input, &Output, &Setpoint,1,1,0, REVERSE);
float temp = 0;

LiquidCrystal lcd(3, 4, 5, 6, 7, 8);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pv=100;
  sp=100;
  pinMode(9,INPUT_PULLUP);
  pinMode(12,INPUT_PULLUP);
  pinMode(10,OUTPUT); // Fan PWM
  pinMode(11,OUTPUT); // TRIAC Driver
  pinMode(13,OUTPUT); // Heating Element Relay

  Input = analogRead(A0) + Otemp;
  Setpoint = sp;
  myPID.SetOutputLimits(10,100);
   myPID.SetMode(AUTOMATIC);
  attachInterrupt(0, ZC_interrupt, RISING); 

  
}

void loop() {
int fan,fp;
int tcpl;
int plot;
if(temp >= 750)
{
 
   while(1)
  {
    digitalWrite(13,LOW);
    
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("OVER TEMP");
      lcd.setCursor(0,1);
      lcd.print(temp);
      delay(100);
  }
}

else
{
  digitalWrite(13,HIGH);
  
}
int ana,te;
te = 0;
for(int i=0;i<=20;i++)
{
  ana =analogRead(A0);
  te=te+ana;
}
tcpl = te/20;
delay(3);
fan = analogRead(A1);
fan = map(fan,0,1023,255,150);
analogWrite(10,fan);
temp = tcpl+Otemp;
Input = temp;
Setpoint = sp;
myPID.Compute();

fp = map(fan,0,255,0,100);
lcd.setCursor(0,0);
lcd.print("PV:");
lcd.setCursor(3,0);
lcd.print(Input);
lcd.setCursor(8,0);
lcd.print("C");
lcd.setCursor(10,0);
lcd.print(Output);
lcd.setCursor(0,1);
lcd.print(fp);
lcd.setCursor(3,1);
lcd.print("%");
lcd.setCursor(8,1);
lcd.print("SP:");
lcd.setCursor(11,1);
lcd.print(sp);
lcd.setCursor(14,1);
lcd.print("C");
if(digitalRead(12)== LOW)
{
  if(sp>100)
  {
    sp=sp-2;
  }
}


if(digitalRead(9)== LOW)
{
  if(sp<700)
  {
    sp=sp+2;
  }
}

delay(10);
lcd.clear();
plot = map(Output,5,125,100,0);
Serial.println(String(Input)+","+String(Setpoint));


}


void ZC_interrupt()
{
  if(Input < Setpoint)
  {
  int dimtime = (75*Output);    // For 60Hz =>65   
  delayMicroseconds(dimtime);    // Off cycle
  digitalWrite(11,HIGH);   // triac firing
  delayMicroseconds(10);         // triac On propagation delay (for 60Hz use 8.33)
  digitalWrite(11,LOW);     // triac Off
 }
 else
 {
  digitalWrite(11,LOW);
 }
}

