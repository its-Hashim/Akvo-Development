#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>
#include <DHT_U.h>

SoftwareSerial s(12, 11);

#define temp3 A3
#define temp2 A2
#define DHT1PIN 7
#define DHT1TYPE DHT22
#define wifi_rst_btn 6
#define comp A1
#define fan A0

OneWire oneWire1(temp3);
DallasTemperature sensors1(&oneWire1);
OneWire oneWire2(temp2);
DallasTemperature sensors2(&oneWire2);
DHT dht1(DHT1PIN, DHT1TYPE);

unsigned long lastOffTime = 0;
unsigned long compOnTime = 0;
unsigned long lastChecked = 0;
unsigned long fanOnTime = 0;
unsigned long fanOffTime = 0;
unsigned long dispenseTime = 0;
short m = 150;

short Climate = 0;

int tempC1;
int tempC2;
int temp;
int humid;

void setup() {
  Serial.begin(9600);
  s.begin(9600);
  delay(5000);
  pinMode(wifi_rst_btn, OUTPUT);
  Serial.println("Resetting Wifi");
  digitalWrite(wifi_rst_btn, HIGH);
  delay(300);
  digitalWrite(wifi_rst_btn, LOW);
  delay(25000);
  sensors1.begin();
  sensors2.begin();
  dht1.begin();

  pinMode(comp, OUTPUT);
  pinMode(fan, OUTPUT);
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
  //    double therm = thermocouple.readCelsius();
  //    if (therm >= 60 && therm < 120)
  //    {
  //      turnOffComp();
  //      turnOffComp();
  //      turnOffComp();
  //      Serial.println("The Temperature Was above 65 degree Celcius");
  //      delay(600000);
  //    }
  //    Serial.println(therm);
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
    Serial.print("The ambient temperature "); Serial.println(temp);
    Serial.print("The ambient Humidity "); Serial.println(humid);
    Serial.print("The evaporating temperature "); Serial.println(tempC1);
    Serial.print("The liquid line temperature "); Serial.println(tempC2);
    if (m >= 10)
    {

      s.print("field1=" + String(temp) + "&field2=" + String(humid) + "&field4=" + String(digitalRead(comp))   + "&field6="  + String(tempC1) + "&field7=" + String(tempC2));
      Serial.println("logging data...................");
      m = 0;
    }
    else
    {
      m++;
    }
    compressor();
    lastChecked = millis();
  }
  delay(150);
  // put your main code here, to run repeatedly:
}

void compressor()
{
  Serial.println("Checking Compressor..............");
  unsigned long interval = millis() - lastOffTime;
  if (interval < 0)
  {
    lastOffTime = 0;
  }
  Serial.println(interval);
  if (digitalRead(comp) != 1 && (temp < 35 || (humid > 35 && humid != 0)) && interval > compOnTime ) // dalay to turn on the compressor is ten sec for now and will be changed after according to the condition
  {
    digitalWrite(fan, HIGH);
    Serial.println("Fan getting On");
    delay(120000);
    digitalWrite(comp, HIGH);
    Serial.println("Compressor getting On");
    delay(15000);
  }
  else if (digitalRead(comp) != 0 && temp >= 35 && humid <= 35 && humid != 0)
  {
    turnOffComp();
    compOnTime = 3600000;
    delay(120000);
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
