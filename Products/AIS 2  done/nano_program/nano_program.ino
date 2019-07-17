#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>
#include <DHT_U.h>

SoftwareSerial s(12, 11);


#define trigPin 9
#define echoPin 8
#define temp3 A3
#define temp2 A2
#define DHT1PIN 7
#define DHT1TYPE DHT22
#define wifi_rst_btn 6
#define comp A1
#define pump 10
#define fan A0
#define disPin 2
#define tHeight 25

OneWire oneWire1(temp3);
DallasTemperature sensors1(&oneWire1);
OneWire oneWire2(temp2);
DallasTemperature sensors2(&oneWire2);

DHT dht1(DHT1PIN, DHT1TYPE);

long duration, distance;
unsigned long lastOffTime = 0;
unsigned long compOnTime = 0;
unsigned long lastChecked = 0;
unsigned long fanOnTime = 0;
unsigned long fanOffTime = 0;
unsigned long dispenseTime = 0;

short m = 150;
int tempC1;
int tempC2;
int temp;
int humid;

void setup() {
  delay(5000);  //for stabilization of power

  Serial.begin(9600);
  s.begin(9600);
  sensors1.begin();
  sensors2.begin();
  dht1.begin();
  delay(500);

  pinMode(wifi_rst_btn, OUTPUT);
  Serial.println("Resetting Wifi");
  digitalWrite(wifi_rst_btn, HIGH);
  delay(300);
  digitalWrite(wifi_rst_btn, LOW);
  delay(20000);

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
  sensors2.requestTemperatures();
  int tempC2 = sensors2.getTempCByIndex(0);
  if (tempC2 >= 70 && tempC2 < 100)
  {
    turnOffComp();
    turnOffComp();
    turnOffComp();
    Serial.println("The Temperature Was above 65 degree Celcius");
    delay(600000);
  }

  unsigned long interval = millis() - lastChecked;
  if (interval < 0)
    lastChecked = 0;
  if (interval > 3000)
  {
    sensors1.requestTemperatures();
    sensors2.requestTemperatures();
    tempC1 = sensors1.getTempCByIndex(0);
    tempC2 = sensors2.getTempCByIndex(0);
    temp = dht1.readTemperature();
    humid = dht1.readHumidity() - 3;
    humid = humid * 0.94;
    distanceMeasure();

    Serial.print("The Ambient Temperature is "); Serial.println(temp);
    Serial.print("The Ambient Humidity is "); Serial.println(humid);
    Serial.print("The Evaporating Temperature is "); Serial.println(tempC1);
    Serial.print("The Liquid Line Temperature is "); Serial.println(tempC2);
    Serial.print("The Tank Level is "); Serial.println(measureHeight(distance));
    if (m >= 2)
    {

      s.print("field1=" + String(temp) + "&field2=" + String(humid) + "&field3=" + String(measureHeight(distance))
              + "&field4=" + String(digitalRead(comp))   + "&field6="  + String(tempC1) + "&field7=" + String(tempC2));
      Serial.println("logging data............");
      m = 0;
    }
    else
    {
      m++;
    }
    compressor();
    lastChecked = millis();
  }
  delay(100);
}

void compressor()
{
  Serial.println("Checking Compressor.........");
  unsigned long interval = millis() - lastOffTime;
  if (interval < 0)
  {
    lastOffTime = 0;
  }
  Serial.println(interval);
  if (distance >= 5 && distance <100 && digitalRead(comp) != 1) // dalay to turn on the compressor is ten sec for now and will be changed after according to the condition
  {
    Serial.println("Trying To Turn on Compressor_________");
    if (digitalRead(fan) != 1)
    {
      digitalWrite(fan, HIGH);
      fanOnTime = millis();
      Serial.println("Fan getting ON________");
    }
    else if (((millis() - fanOnTime) > 120000) && interval > 120000)
    {
      digitalWrite(comp, HIGH);
      Serial.println("Compressor getting ON_______");
    }
  }
  else if (digitalRead(comp) != 0 && (distance <= 3 || distance > 100 ))
  {
    turnOffComp();
    compOnTime = 60000;
  }
  else if (digitalRead(fan) == 1 && digitalRead(comp) == 0 && interval > 120000 && distance < 5)
  {
    digitalWrite(fan, LOW);
    Serial.println("Fan getting OFF________");
  }
  else
  {
    Serial.println("Compressor and Fan state is unaltered");
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
  short measure = ((d) / 25.0) * 100.00;
  if (measure > 100 || measure < 0)
    return (00);
  else
    return ((100 - measure));
}
void turnOffComp()
{
  digitalWrite(comp, LOW);
  lastOffTime = millis();
  Serial.println("Compressor getting OFF_________");

}
