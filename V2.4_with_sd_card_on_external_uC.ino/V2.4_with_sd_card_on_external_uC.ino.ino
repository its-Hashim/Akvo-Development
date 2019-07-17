//Library for time  module

#include <DS3231.h>
//Library for LCD screen

#include <LiquidCrystal_I2C.h>
#include <Wire.h>

//Library for temperature sensor

#include <OneWire.h>
#include <DallasTemperature.h>

//Library for DHT sensor

#include <DHT.h>
#include <DHT_U.h>


//Pin defination for Temperature sensor

#define ONE_WIRE_BUS 7  // OneWire precision Dallas Sensor

// Pin defination for sonar sensor

#define trigPin 10
#define echoPin 9

//Pin defination for DHT sensors

#define DHT1PIN 5    // what pin we're connected to
#define DHT1TYPE DHT22   // DHT 22

//Pin defination for realy

#define comp A0
#define pump A1

//Pin defination for interrupt pin

#define disPin 2


//addressing of time module

DS3231  rtc(SDA, SCL);

//Addressing of lcd screen

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

//Addressing of temperature sensor

OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire); // Pass our oneWire reference to Dallas Temperature.

//Addressing of DHT sensor

DHT dht1(DHT1PIN, DHT1TYPE);

//Variable assignment for program logic

long duration, distance;   // Veriables to calculate the distance
//DeviceAddress T1, T2, T3, T4, T5, T6, T7, T8; // arrays to hold device addresses
short k = 1;
short l = 0;
short m = 0;
unsigned long lastOffTime = 0;
unsigned long lastChecked = 0;
unsigned long lastLogged = 0;
unsigned long compOnTime = 60000;

boolean highPressure = false;
boolean lowPressure = false;




void setup()
{
  // Start up the library for Serial communication b/w computer and nano

  Serial.begin(9600);

  //Start up the library for lcd screen

  lcd.begin(20, 4);

  sensors.begin();

  dht1.begin();

  //Start up the library for time module
  rtc.begin();

  //Pin mode configuration for sonnar sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //Pin mode configuration for relay
  pinMode(compressor, OUTPUT);
  pinMode(pump, OUTPUT);
  digitalWrite(pump, HIGH);
  pinMode(comp, OUTPUT);
  digitalWrite(comp, HIGH);
  pinMode(disPin, INPUT);
  digitalWrite(disPin, LOW);


  lcd.home();
  lcd.print("      AKVO 36K");
  lcd.setCursor(0, 1);
  lcd.print("     Plese Wait");
  lcd.setCursor(0, 2);
  lcd.print("    Callibrating");
  lcd.setCursor(0, 3);
  delay(200);
  delay(1000);
  //Setting display
  resetDisplay();
}

void loop()
{
  unsigned long interval = millis() - lastChecked;
  if (interval < 0)
    lastChecked = 0;
  if (interval > 3000)
  {
    sensors.requestTemperatures();
    int tempC = sensors.getTempCByIndex(0);
    int temp = dht1.readTemperature();
    int humid = dht1.readHumidity() - 3;
    lcd.setCursor(8, 0); lcd.print(temp);
    lcd.print(" C, "); lcd.print(humid); lcd.print("% RH");
    String t = rtc.getTimeStr();
    String d = rtc.getDateStr();
    distanceMeasure();
    t.remove(5);
    d.remove(5);
    d.replace(".", "/");
    lcd.setCursor(11, 1);  lcd.print(measureHeight(distance)); lcd.print("%"); lcd.print("    ");
    lcd.setCursor(15, 2); lcd.print(d);
    lcd.setCursor(0, 3); lcd.print("WATER:"); lcd.print(tempC); lcd.print(" C  ");
    lcd.setCursor(15, 3); lcd.print(t);
    if (l == 3)
    {
      if (digitalRead(pump))
      {
        digitalWrite(pump, LOW);
      }
      else
      {
        digitalWrite(pump, HIGH);
      }
      l = 0;
    }
    else
    {
      l++;
    }
    compressor(humid, temp, tempC);

    if (m == 15)
    {
      if (digitalRead(comp))
      {
        Serial.print("\nTime is " + t + "  Date is " + d);
        delay(500);
        Serial.print("\nAmbient Temperature " + String(temp) + " C " + + "\t Ambient Humidity " + String(humid) + "%");
        delay(500);
        Serial.print("Water Temperature is "+String(tempC));
        delay(500);
        Serial.println( "\nTank Level is " + String(measureHeight(distance))+"\nCompressor is OFF");
        //store for comp off
        

      
      }
      else
      {
        Serial.print("\nTime is " + t + "  Date is " + d);
        delay(1000);
        Serial.println("\nAmbient Temperature " + String(temp) + " C" + + "\t  Ambient Humidity " + String(humid) + "%");
        delay(500);
        Serial.print("Water Temperature is "+String(tempC));
        delay(500);
        Serial.println( "\nTank Level is " + String(measureHeight(distance))+"\nCompressor is ON");
        
        //store for comp on
      }
      m = 0;
    }
    else
    {
      m++;
    }

    lastChecked = millis();

  }
  //  if (digitalRead(disPin) == 1)
  //  {
  //    dispenseDisplay();
  //    while (digitalRead(disPin) == 1)
  //    {
  //      dispense();
  //    }
  //    resetDisplay();
  //    digitalWrite(pump, HIGH);
  //  }
  //  else if (digitalRead(pump) != 1)
  //  {
  //    digitalWrite(pump, HIGH);
  //  }
  delay(200);
}

short measureHeight(int d)
{
  short measure = (d / 39.00) * 100.00;
  if (measure > 100 || measure < 0)
    return (00);
  else
    return ((100 - measure));
}

void compressor(int humid, int temp, int tempC)
{
  unsigned long interval = millis() - lastOffTime;
  if (interval < 0)
    lastOffTime = 0;
  if (tempC >= 55)
  {
    turnOffComp();
    highPressure = true;
    compOnTime = 600000;
  }
  else if (tempC <= 10)
  {
    turnOffComp();
    lowPressure = true;
    compOnTime = 600000;
  }
  else if (distance > 7 && distance < 40 && digitalRead(comp) != 0 && interval > compOnTime && humid > 30 && temp > 15 && temp < 50) // dalay to turn on the compressor is ten sec for now and will be changed after according to the condition
  {
    //Check last the compresssor
    //if the compressor was turn off for more than 3 min turn On the compressor
    digitalWrite(comp, LOW);
    lcd.setCursor(11, 2);  lcd.print("ON  ");
  }
  else if (digitalRead(comp) != 1 && (distance < 4 || distance > 40  ||  temp >= 50 || (temp <= 25 && humid < 40)) )
  {
    turnOffComp();
    compOnTime = 60000;
  }
  else if (digitalRead(comp) != 1 && (distance < 4 || distance > 40  ||  temp >= 50 || temp <= 15 || humid <= 30) )
  {
    turnOffComp();
    compOnTime = 60000;
  }
}

void dispense()
{
  distanceMeasure();
  if (distance < 32)
    digitalWrite(pump, LOW);
  else
    digitalWrite(pump, HIGH);
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
  digitalWrite(comp, HIGH);
  lcd.setCursor(11, 2);  lcd.print("OFF ");
  lastOffTime = millis();
}

