#include <SD.h>
#include <SPI.h>
#include <DS3231.h>


DS3231  rtc(SDA, SCL);

File f;

unsigned long previousMillis = 0; // last time update
long interval = 1000; // interval at which to do something (milliseconds)

void setup ()
{
  Serial.begin(9600);
  Serial.print("Initialising SD Card...");
  rtc.begin();

  //  attachInterrupt(digitalPinToInterrupt(2), dispence,RISING);

  delay(1000);
  if (!SD.begin(4))
  {
    Serial.println("\nInitialising Failed!!");
    return;
  }
  else
  {
    Serial.println("\nInitialising Success!!");
    f = SD.open("somefile.txt" , FILE_WRITE);
    f.println("\n\n\n\n BOOTED\n\n\n\n");
    f.close();
    Serial.println("somefile.txt created hooooowa");
    store();
  }
  delay(1000);
}
void loop()
{
  delay(500);
  //store();
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis < 0)
    previousMillis = currentMillis;
  else if (currentMillis - previousMillis > interval)
  {
    previousMillis = currentMillis;
    store();
  }
}
void store()
{
  if (SD.exists("somefile.txt"))
  {
    Serial.println("storing...");
    f = SD.open("somefile.txt", FILE_WRITE);
    //    f.println("The time is now " + String(rtc.getTimeStr()));
    //    f.println("Todays date is " + String(rtc.getDateStr()));
    //    Serial.println("The time is now " + String(rtc.getTimeStr()));
    //    Serial.println("Todays date is " + String(rtc.getDateStr()));
    f.close();
  }
  else
  {
    Serial.println("File does not exists");
  }
}

void dispence()
{

  if (false)
    Serial.println("Dispencing");
  Serial.println("done");
}

