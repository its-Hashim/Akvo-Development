//Library for time  module

#include <DS3231.h>

//Library for LCD screen

#include <LiquidCrystal_I2C.h>
#include <Wire.h>

//Library for temperature sensor

//#include <OneWire.h>
//#include <DallasTemperature.h>

//Library for DHT sensor

#include <DHT.h>
#include <DHT_U.h>


//Pin defination for Temperature sensor

#define ONE_WIRE_BUS 5
#define precision 12     // OneWire precision Dallas Sensor

// Pin defination for sonar sensor

#define trigPin 9
#define echoPin 8

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

//OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
//DallasTemperature sensors(&oneWire); // Pass our oneWire reference to Dallas Temperature.

//Addressing of DHT sensor

DHT dht1(DHT1PIN, DHT1TYPE);

//Variable assignment for program logic

long duration, distance;   // Veriables to calculate the distance
//DeviceAddress T1, T2, T3, T4, T5, T6, T7, T8; // arrays to hold device addresses
short k = 1;
unsigned long lastOffTime = 0;
unsigned long lastChecked = 0;
unsigned long lastLogged = 0;



void setup()
{
  // Start up the library for Serial communication b/w computer and nano

  Serial.begin(115200);

  //Start up the library for lcd screen

  lcd.begin(20, 4);

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

  Serial.print("Start");


  //Setting display
  Serial.println("2 the next part");
  resetDisplay();
}

void loop()
{
  unsigned long interval = millis() - lastChecked;
  if (interval < 0)
    lastChecked = 0;
  if (interval > 3000)
  {
    lcd.setCursor(8, 0); lcd.print(int(dht1.readTemperature()));
    lcd.print(" C, "); lcd.print(int(dht1.readHumidity() - 3)); lcd.print("% RH");
    String t = rtc.getTimeStr();
    String d = rtc.getDateStr();
    distanceMeasure();
    t.remove(5);
    d.replace(".", "/");
    lcd.setCursor(11, 1);  lcd.print(measureHeight(distance)); lcd.print("%"); lcd.print("    ");
    lcd.setCursor(0, 3); lcd.print("");  lcd.print(d); lcd.print("     "); lcd.print(t);
    compressor();
    lastChecked = millis();
  }
  if (digitalRead(disPin) == 1)
  {
    dispenseDisplay();
    while (digitalRead(disPin) == 1)
    {
      //      Serial.println("Button Pressed");
      dispense();
    }
    resetDisplay();
    digitalWrite(pump, HIGH);
  }
  else if (digitalRead(pump) != 1)
  {
    Serial.println("Button not pressed");
    digitalWrite(pump, HIGH);
  }
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

void compressor()
{
  Serial.println("Comp Check");
  unsigned long interval = millis() - lastOffTime;
  if (interval < 0)
    lastOffTime = 0;
  Serial.print("\n\nInterval: ");
  Serial.println(interval);
  Serial.print("D=");
  Serial.println(distance);
  if (distance > 7 && distance < 40 && digitalRead(comp) != 0 && interval > 60000) // dalay to turn on the compressor is ten sec for now and will be changed after according to the condition
  {
    Serial.println("Turning On");
    //Check last the compresssor
    //if the compressor was turn off for more than 3 min turn On the compressor
    digitalWrite(comp, LOW);
    lcd.setCursor(11, 2);  lcd.print("ON  ");
  }
  else if (digitalRead(comp) != 1 && (distance < 4 || distance > 40) )
  {
    Serial.println("Turning Off");
    digitalWrite(comp, HIGH);
    lcd.setCursor(11, 2);  lcd.print("OFF ");
    lastOffTime = millis();
  }
  else
    Serial.println("Not any condition");
}

void dispense()
{
  distanceMeasure();
  if (distance < 32)
    digitalWrite(pump, LOW);
  else
    digitalWrite(pump, HIGH);
  //  Serial.println("Got into Dispense");
  //  Serial.print("Distance: ");
  //  Serial.println(distance);
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

