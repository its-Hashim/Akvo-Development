//Library for temperature sensor

#include <OneWire.h>
#include <DallasTemperature.h>

//Library for LCD screen

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//Library for DHT sensor

#include <DHT.h>
#include <DHT_U.h>


//Pin defination for Temperature sensor

#define ONE_WIRE_BUS 4
#define precision 12     // OneWire precision Dallas Sensor

//Pin defination for DHT sensors

#define DHT1PIN 5     // what pin we're connected to
#define DHT1TYPE DHT22   // DHT 22
#define DHT2PIN 6     // what pin we're connected to
#define DHT2TYPE DHT22   // DHT 22


// Pin defination for sonar sensor

#define trigPin 3
#define echoPin 2

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire); // Pass our oneWire reference to Dallas Temperature. 
DHT dht1(DHT1PIN, DHT1TYPE);
DHT dht2(DHT2PIN, DHT2TYPE);

long duration, distance;   // Veriables to calculate the distance
int sen_number = 0;        // Counter of Dallas sensors 
DeviceAddress T1, T2, T3, T4, T5, T6, T7, T8; // arrays to hold device addresses
short k=1;

void setup()
{
  // Start up the library
  Serial.begin(9600);
  lcd.begin(20,4);
  sensors.begin();
  dht1.begin();
  dht2.begin();

  //Set pins of arduino no
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  Serial.print("Found: ");
  Serial.print(sensors.getDeviceCount(), DEC);  // locate devices on the bus
  Serial.println(" Devices.");
  lcd.home();
  lcd.print("      AKVO 36K");
  lcd.setCursor(0,1);
  lcd.print("     Plese Wait");
  lcd.setCursor(0,2);
  lcd.print("    Callibrating");
  lcd.setCursor(0,3);
  delay(1000);
  //storing the adress of all the temperature sensor
  if (!sensors.getAddress(T1, 0)) 
  Serial.println("Not Found Sensor 1");
  delay("100");
  lcd.print("..");
  if (!sensors.getAddress(T2, 1))
  Serial.println("Not Found Sensor 2");
  delay("100");
  lcd.print("..");
  if (!sensors.getAddress(T3, 2)) 
  Serial.println("Not Found Sensor 3");
  delay("100");
  lcd.print("..");
  if (!sensors.getAddress(T4, 3)) 
  Serial.println("Not Found Sensor 4");
  delay("100");
  lcd.print("..");
  if (!sensors.getAddress(T5, 4)) 
  Serial.println("Not Found Sensor 5");
  delay(100);
  lcd.print("..");
  if (!sensors.getAddress(T6, 5)) 
  Serial.println("Not Found Sensor 6");
  delay(100);
  lcd.print("..");
  if (!sensors.getAddress(T7, 6)) 
  Serial.println("Not Found Sensor 7");
  delay(100);
  lcd.print("..");
  if (!sensors.getAddress(T8, 7)) 
  Serial.println("Not Found Sensor 8");
  delay(100);
  lcd.print("..");
  //set the resolution to 12 bit per device
  sensors.setResolution(T1, precision);
  lcd.print("..");
  sensors.setResolution(T2, precision);
  lcd.print("..");
  sensors.setResolution(T3, precision);
  sensors.setResolution(T4, precision);
  sensors.setResolution(T5, precision);
  sensors.setResolution(T6, precision);
  sensors.setResolution(T7, precision);
  sensors.setResolution(T8, precision);
  delay(1000);
  lcd.clear();
}

void loop()
{
  sensors.requestTemperatures(); 
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  Serial.print(distance);
  

  lcd.setCursor(0,0);
  lcd.print("T1="); lcd.print(sensors.getTempC(T1));
  lcd.print(" T2="); lcd.print(sensors.getTempC(T2));
  
  lcd.setCursor(0,2);
  lcd.print("Tank Filled="); lcd.print(measureHeight(distance)); lcd.print("%"); lcd.print("    ");
  
  
  if(k==1)
  {
    lcd.setCursor(0,1);
    lcd.print("T3="); lcd.print(dht1.readTemperature());
    lcd.print(" T4="); lcd.print(dht2.readTemperature());
    lcd.setCursor(0,3);
    lcd.print("H1="); lcd.print(dht1.readHumidity()+3); lcd.print("%");lcd.print(" H2="); lcd.print((dht2.readHumidity())); lcd.print("%");
    k=3;
  }
  else
  k--;
  Serial.println(sensors.getTempC(T1));
  Serial.println(sensors.getTempC(T2));
  Serial.println((distance));
  Serial.println(measureHeight(distance));
  delay(1000);
}

short measureHeight(int d)
{
  short measure=(d/39.00)*100.00;
  if(measure>100||measure<0)
  return(00);
  else
  return((100-measure));
}

