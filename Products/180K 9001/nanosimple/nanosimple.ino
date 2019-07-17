#include "MQTT.h"
#define tSignal A4

#define comp A0
#define pump A1
#define fan A2

#define disPin 2


unsigned long lastOffTime = 0;
unsigned long compOnTime = 120000;
unsigned long lastChecked = 0;
unsigned long fanOnTime = 0;
unsigned long fanOffTime = 0;
unsigned long dispenseTime = 0;

void setup() {
  Serial.begin(9600);

  pinMode(comp, OUTPUT);
  pinMode(pump, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(disPin, INPUT);
  pinMode(tSignal, INPUT);
  digitalWrite(disPin, HIGH);
  digitalWrite(fan, HIGH);
  delay(200);
}

void loop() {
  unsigned long interval = millis() - lastChecked;
  if (interval < 0)
    lastChecked = 0;
  if (interval > 3000)
  {
    Serial.println("Checking");

    short sig = 0;
    for (int g = 0; g<10 ; g++)
    {
      sig += analogRead(tSignal);
      delay(100);
    }
    sig /=10;
    Serial.print("Tank Signal is ") ;Serial.println(sig);
    //    if (sig > 55)
    //    {
    //      sig = 1;
    //    }
    //    else
    //    {
    //      sig = 0;
    //    }

    compressor(sig);
    lastChecked = millis();
  }
  Serial.println(digitalRead(disPin));
  if (digitalRead(disPin) == 0)
  {
    dispenseTime = millis();
    while (digitalRead(disPin) == 0)
    {
      Serial.println("Got The Command");
      delay(5);
      digitalWrite(pump, HIGH);
    }
    digitalWrite(pump, LOW);
  }
  else if (digitalRead(pump) != 0)
  {
    digitalWrite(pump, LOW);
  }
  delay(200);
  // put your main code here, to run repeatedly:

}

void compressor(short sig)
{
  Serial.println("Checking Compressor");
  unsigned long interval = millis() - lastOffTime;
  if (interval < 0)
  {
    lastOffTime = 0;
  }
  Serial.println(interval);
  if (sig < 10 && digitalRead(comp) != 1) // dalay to turn on the compressor is ten sec for now and will be changed after according to the condition
  {

    //Check last the compresssor
    //if the compressor was turn off for more than 3 min turn On the compressor
    Serial.println("Trying To Turn on Compressor");
    if (digitalRead(fan) != 1)
    {
      digitalWrite(fan, HIGH);
      fanOnTime = millis();
      Serial.println("Fan getting On");
    }
    else if (((millis() - fanOnTime) > 120000) && interval > compOnTime)
    {
      digitalWrite(comp, HIGH);
      Serial.println("Compressor getting On");
    }
  }
  else if (digitalRead(comp) != 0 && sig > 50)
  {
    turnOffComp();
    compOnTime = 600000;
  }
  else if (digitalRead(fan) == 1 && digitalRead(comp) == 0 && interval > 120000)
  {
    digitalWrite(fan, LOW);
    Serial.println("Fan getting off");
  }
  else
  {
    Serial.println("not found any Condition for compresser Its state is unaltered");
  }
}


void turnOffComp()
{
  digitalWrite(comp, LOW);
  lastOffTime = millis();
  Serial.println("Compressor getting off");

}
