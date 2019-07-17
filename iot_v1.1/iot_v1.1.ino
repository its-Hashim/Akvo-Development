#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>
#include <DHT_U.h>

SoftwareSerial s(11, 12);

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

#define trigPin 10
#define echoPin 9
#define tempLow 7
#define tempHigh 8
#define DHT1PIN 5
#define DHT1TYPE DHT22

OneWire oneWire1(tempLow);
DallasTemperature sensors1(&oneWire1);
OneWire oneWire2(tempHigh);
DallasTemperature sensors2(&oneWire2);
DHT dht1(DHT1PIN, DHT1TYPE);


#define comp A0
#define pump A1
#define fan A2

#define disPin 2


long duration, distance;
unsigned long lastOffTime = 0;
unsigned long compOnTime = 0;
unsigned long lastChecked = 0;
unsigned long fanOnTime = 0;
unsigned long fanOffTime = 0;
unsigned long dispenseTime = 0;
short m = 20;

void setup() {
  Serial.begin(9600);
  s.begin(9600);
  lcd.begin(20, 4);
  sensors1.begin();
  sensors2.begin();
  dht1.begin();

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(comp, OUTPUT);
  pinMode(pump, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(disPin, INPUT);
  digitalWrite(disPin, HIGH);
  digitalWrite(fan, HIGH);
  lcd.home();
  lcd.print("      AKVO 36K");
  lcd.setCursor(0, 1);
  lcd.print("     Plese Wait");
  lcd.setCursor(0, 2);
  lcd.print("    Callibrating");
  lcd.setCursor(0, 3);
  delay(200);
  delay(10000);
  resetDisplay();
}

void loop() {
  unsigned long interval = millis() - lastChecked;
  if (interval < 0)
    lastChecked = 0;
  if (interval > 3000)
  {
    //    Serial.println("Checking");
    sensors1.requestTemperatures();
    sensors2.requestTemperatures();
    int tempC1 = sensors1.getTempCByIndex(0);
    int tempC2 = sensors2.getTempCByIndex(0);
    int temp = dht1.readTemperature();
    int humid = dht1.readHumidity() - 3;
    humid = humid*0.9;
    lcd.setCursor(8, 0); lcd.print("");
    lcd.print(" C, "); lcd.print(" "); lcd.print("% RH");
    distanceMeasure();
    lcd.setCursor(11, 1);  lcd.print(measureHeight(distance)); lcd.print("%"); lcd.print("    ");
    lcd.setCursor(15, 2); lcd.print("");
    lcd.setCursor(0, 3); lcd.print("WATER:"); lcd.print(""); lcd.print(" C  ");
    lcd.setCursor(15, 3); lcd.print("");
    compressor();
    if (m >= 15)
    {

      s.print("field1=" + String(temp) + "&field2=" + String(humid) + "&field3=" + String(measureHeight(distance))
              + "&field4=" + String(digitalRead(comp))   + "&field6="  + String(tempC1) + "&field7=" + String(tempC2));
          Serial.println("logging data");
          Serial.println(tempC1);
          Serial.println(tempC2);
          Serial.println(temp);
          Serial.println(humid);
          Serial.println(distance);
          Serial.println(measureHeight(distance));
      m = 0;
    }
    else
    {
      m++;
    }
    lastChecked = millis();
  }
  Serial.println(digitalRead(disPin));
  if (digitalRead(disPin) == 0)
  {
    dispenseDisplay();
    dispenseTime = millis();
    while (digitalRead(disPin) == 0)
    {
      Serial.println("Got The Command");
      distanceMeasure();
      delay(5);
      if (distance > 35)
        break;
      Serial.println("Dispensing");
      dispense();
    }
    resetDisplay();
    digitalWrite(pump, LOW);
    dispenseTime = millis() - dispenseTime;
    dispenseTime = dispenseTime / 1000;
    s.print("field5=" + String(dispenseTime));
  }
  else if (digitalRead(pump) != 0)
  {
    digitalWrite(pump, LOW);
  }
  delay(200);
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
  if (digitalRead(comp) == 0 && interval > 120000 && distance < 8 && distance < 45)
  {
    digitalWrite(fan, LOW);
    Serial.println("Fan getting off");
  }
  else if (distance > 8 && distance < 65 && digitalRead(comp) != 1) // dalay to turn on the compressor is ten sec for now and will be changed after according to the condition
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
      lcd.setCursor(11, 2);  lcd.print("ON  ");
      Serial.println("Compressor getting On");
    }
  }
  else if (digitalRead(comp) != 0 && (distance < 5 || distance > 65 ))
  {
    turnOffComp();
    compOnTime = 60000;
  }
  else 
  {
    Serial.println("not found");
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
void dispenseDisplay()
{
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("     DISPENSING     ");
  lcd.setCursor(0, 3);
  lcd.print("       WATER        ");
}

void resetDisplay()
{
  lcd.clear();  lcd.print("AMBIENT:");
  lcd.setCursor(0, 1); lcd.print("TANK LEVEL:");
  lcd.setCursor(0, 2); lcd.print("COMPRESSOR:");
  lcd.setCursor(11, 2);
  if (digitalRead(comp) == 1)
    lcd.print("OFF ");
  else
    lcd.print("ON ");
}


void turnOffComp()
{
  digitalWrite(comp, LOW);
  lcd.setCursor(11, 2);  lcd.print("OFF ");
  lastOffTime = millis();
  Serial.println("Compressor getting off");

}

