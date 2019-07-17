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

#define dispensePin 3


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
short k=1;
unsigned long lastOffTime = 0;
unsigned long lastChecked = 0;
unsigned long lastLogged = 0;



void setup()
{
  // Start up the library for Serial communication b/w computer and nano
  
  Serial.begin(9600);

  //Start up the library for lcd screen
  
  lcd.begin(20,4);

  //Start up the library for temperature sensor

//  sensors.begin();

  //Start up the library for DHT sensor
  
  dht1.begin();

  //Start up the library for time module

  rtc.begin();

  //Interrupt setUp
  attachInterrupt(digitalPinToInterrupt(dispensePin), dispense, RISING);

  //Pin mode configuration for sonnar sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //Pin mode configuration for relay
  pinMode(compressor,OUTPUT);
  pinMode(pump,OUTPUT);
  digitalWrite(pump, HIGH);
  pinMode(comp,OUTPUT);
  digitalWrite(comp,HIGH);

  
  lcd.home();
  lcd.print("      AKVO 36K");
  lcd.setCursor(0,1);
  lcd.print("     Plese Wait");
  lcd.setCursor(0,2);
  lcd.print("    Callibrating");
  lcd.setCursor(0,3);
  delay(200);

  //Setting up temperature

  Serial.print("Found: ");
//  Serial.print(sensors.getDeviceCount(), DEC);  // locate devices on the bus
//  Serial.println(" Devices.");
//
//  if (!sensors.getAddress(T1, 0)) 
//  Serial.println("Not Found Sensor 1");
  delay("100");
  lcd.print("..");
//  if (!sensors.getAddress(T2, 1))
//  Serial.println("Not Found Sensor 2");
  delay("100");
  lcd.print("..");
//  if (!sensors.getAddress(T3, 2)) 
  Serial.println("Not Found Sensor 3");
  delay("100");
  lcd.print("..");
//  if (!sensors.getAddress(T4, 3)) 
//  Serial.println("Not Found Sensor 4");
  delay("100");
  lcd.print("..");
//  if (!sensors.getAddress(T5, 4)) 
//  Serial.println("Not Found Sensor 5");
  delay(100);
  lcd.print("..");
//  if (!sensors.getAddress(T6, 5)) 
//  Serial.println("Not Found Sensor 6");
  delay(100);
  lcd.print("..");
//  if (!sensors.getAddress(T7, 6)) 
//  Serial.println("Not Found Sensor 7");
  delay(100);
  lcd.print("..");
//  if (!sensors.getAddress(T8, 7)) 
//  Serial.println("Not Found Sensor 8");
  delay(100);
  lcd.print("..");
  //set the resolution to 12 bit per device
//  sensors.setResolution(T1, precision);
  lcd.print("..");
//  sensors.setResolution(T2, precision);
  lcd.print("..");
//  sensors.setResolution(T3, precision);
//  sensors.setResolution(T4, precision);
//  sensors.setResolution(T5, precision);
//  sensors.setResolution(T6, precision);
//  sensors.setResolution(T7, precision);
//  sensors.setResolution(T8, precision);
  delay(1000);


  //Setting display
  Serial.println("2 the next part");
  lcd.clear();  lcd.print("Amb Temp:");
  lcd.setCursor(0,1); lcd.print("Amb RH:");
  lcd.setCursor(0,2); lcd.print("Tank Level:");
  lcd.setCursor(0,3); lcd.print("Compressor:");
  lcd.setCursor(11,3);lcd.print("OFF    ");

//  if (!SD.begin(4))
//  {
//    Serial.print("\nInitialising Failed!!");
//    return;
//  }
//  Serial.println("\nInitialising Success!!");
//  f = SD.open("somefile.txt" , FILE_WRITE);
//  f.println("\n\n\n\n BOOTED\n\n\n\n");
//  f.close();
//  Serial.println("somefile.txt created hooooowa");
Serial.println("Going to loop");
}
void loop()
{
  digitalWrite(pump, HIGH);
  delay(500);
  //Requesting temperature
//  sensors.requestTemperatures();

  //claculating distance from sonar sensor
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  Serial.println(distance);

  //Storing time and date to t and d
  String t = rtc.getTimeStr();
  String d = rtc.getDateStr();

  //Seting up t and d for display
  t.remove(5);
  d.remove(5);
  d.replace(".","/");
  unsigned long interval = millis() - lastChecked;
  if(interval < 0)
  lastChecked = 0;
  
  if(interval > 3000)
  {
    lcd.setCursor(9,0); lcd.print((dht1.readTemperature()));
    lcd.setCursor(7,1); lcd.print(int(dht1.readHumidity()-3)); lcd.print("%");
    lastChecked = millis();
  }

  lcd.setCursor(11,2);  lcd.print(measureHeight(distance)); lcd.print("%"); lcd.print("    ");
  lcd.setCursor(15,1);  lcd.print(t);
  lcd.setCursor(15,3);  lcd.print(d);

  interval = millis() - lastLogged;
//  if (interval < 0)
//  lastLogged = 0;
//  if (interval > 10000)
//  {
//    logging();
//  }
    
  compressor();
}

short measureHeight(int d)
{
  short measure=(d/39.00)*100.00;
  if(measure>100||measure<0)
  return(00);
  else
  return((100-measure));
}

void compressor()
{
  distanceMeasure();
  Serial.println("Comp Check");
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  unsigned long interval = millis() - lastOffTime;
  if (interval < 0)
  lastOffTime = 0;
  Serial.print("\n\nInterval: ");
  Serial.println(interval);
  Serial.print("D=");
  Serial.println(distance);
  if(distance > 4 && distance < 40 && digitalRead(comp) != 0 && interval > 60000)  // dalay to turn on the compressor is ten sec for now and will be changed after according to the condition
  {
    Serial.println("Turning On");
    //Check last the compresssor
    //if the compressor was turn off for more than 3 min turn On the compressor
    digitalWrite(comp, LOW);
    lcd.setCursor(11,3);  lcd.print("ON  ");
  }
  else if(digitalRead(comp) != 1 && (distance < 4 || distance > 40) )
  {
    Serial.println("Turning Off");
    digitalWrite(comp,HIGH);
    lcd.setCursor(11,3);  lcd.print("OFF ");
    lastOffTime = millis();
  }
  else
  Serial.println("Not any condition");
}

void dispense()
{
  Serial.print(digitalRead(dispensePin));
  distanceMeasure();
  if (distance < 20)
  digitalWrite(pump, LOW);
  else
  digitalWrite(pump, HIGH);
  Serial.println("Got into Dispense");
}
void distanceMeasure()
{
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
}
//change the relative density display seting
//change the temperature display seting


