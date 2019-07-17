#include "max6675.h"
#include <Wire.h>
#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>
#include <DHT_U.h>

SoftwareSerial s(12, 11);

#define trigPin 10
#define echoPin 9
#define comp A0
#define pump A1
#define fan A2
#define disPin 2
#define tankHeight 50

long duration, distance;
unsigned long lastOffTime = 0;
unsigned long compOnTime = 0;
unsigned long lastChecked = 0;
unsigned long fanOnTime = 0;
unsigned long fanOffTime = 0;
unsigned long dispenseTime = 0;
short m = 150;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(comp, OUTPUT);
  pinMode(pump, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(disPin, INPUT);
  digitalWrite(disPin, HIGH);
  delay(200);
}

void loop()
{
  unsigned long interval = millis() - lastChecked;
  if (interval < 0)
    lastChecked = 0;
  if (interval > 3000)
  {
    distanceMeasure();
    compressor();
    lastChecked = millis();
  }
  if (digitalRead(disPin) == 0)
  {
    delay(50);
    dispenseTime = millis();
    while (digitalRead(disPin) == 0)
    {
      Serial.println(".");
      distanceMeasure();
      delay(5);
      if (distance >= 45)
        break;
      dispense();
    }
    digitalWrite(pump, LOW);
    digitalWrite(pump, LOW);
    dispenseTime = millis() - dispenseTime;
    dispenseTime = dispenseTime / 1000;
    delay(400);
  }
  delay(150);
  // put your main code here, to run repeatedly:

}

void compressor()
{
  Serial.println("Checking Compressor");
  unsigned long interval = millis() - lastOffTime;
  if (interval < 0)
  {
    lastOffTime = 0;
  }
  Serial.println(interval);
  if (distance > 8 && distance < 65 && digitalRead(comp) != 1) // dalay to turn on the compressor is ten sec for now and will be changed after according to the condition
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
    else if (((millis() - fanOnTime) > 120000) && interval > 120000)
    {
      digitalWrite(comp, HIGH);
      Serial.println("Compressor getting On");
    }
  }
  else if (digitalRead(comp) != 0 && (distance < 5 || distance > 65 ))
  {
    turnOffComp();
    compOnTime = 60000;
  }
  else if (digitalRead(fan) == 1 && digitalRead(comp) == 0 && interval > 120000 && distance < 8)
  {
    digitalWrite(fan, LOW);
    Serial.println("Fan getting off");
  }
  else
  {
    Serial.println("not found any Condition for compresser Its state is unaltered");
  }
}

void dispense()
{
  if (distance < 45)
    digitalWrite(pump, HIGH);
  else
    digitalWrite(pump, LOW);
}

void distanceMeasure()
{
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;
  Serial.println("The Distance is ");
  Serial.println(distance);
}
short measureHeight(int d)
{
  Serial.println("Measuring Tank level");
  Serial.println(d);
  short measure = ((d) / 50.0) * 100.00;
  if (measure > 100 || measure < 0)
    return (00);
  else
    return ((100 - measure));
}


void turnOffComp()
{
  digitalWrite(comp, LOW);
  lastOffTime = millis();
  Serial.println("Compressor getting off");

}
