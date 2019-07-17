#include <SD.h>
#include <SPI.h>
int LED = 13;
File f;
String data = "";
unsigned long previousMillis = 0; // last time update
long interval = 1000; // interval at which to do something (milliseconds)

void setup ()
{
  Serial.begin(9600);
  //  attachInterrupt(digitalPinToInterrupt(2), dispence,RISING);
  pinMode(LED, OUTPUT);
  delay(1000);
  if (!SD.begin(4))
  {
    LEDBlink(15);
    Serial.println("Game over");
    return;
  }
  else
  {
    f = SD.open("somefile.txt" , FILE_WRITE);
    f.println("\n\n\n\n BOOTED\n\n\n\n");
    f.close();
    LEDBlink(2);
  }
  delay(1000);
  LEDBlink(2);
}
void loop()
{
  if (Serial.available())
  {
    delay(100);
    while (Serial.available())
    {
      data += ((char(Serial.read())));
    }
    store(data);
    data = "";
  }
}
void store(String data)
{
  if (SD.exists("somefile.txt"))
  {
    f = SD.open("somefile.txt", FILE_WRITE);
    f.print(data);
    //        f.println("Todays date is " + String(rtc.getDateStr()));
    //    Serial.println("The time is now " + String(rtc.getTimeStr()));
    //    Serial.println("Todays date is " + String(rtc.getDateStr()));
    f.close();
    LEDBlink(2);
  }
  else
  {
    LEDBlink(1);
  }
}

void LEDBlink(int k)
{
   for(int i = 0; i<k; i++)
   {
    digitalWrite(LED,HIGH);
    delay(100);
    digitalWrite(LED,LOW);
    delay(100);
   }
}

