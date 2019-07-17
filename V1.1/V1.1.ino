//Include libraries
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define trigPin 3
#define echoPin 2

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 4
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

long duration, distance;
void setup(void)
{
  Serial.begin(9600); //Begin serial communication
  sensors.begin();
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  lcd.begin(20,4); 
  lcd.setCursor(3,0);
  lcd.print("Hello Brother!!");
  lcd.setCursor(8,1);
  lcd.print("****");
  lcd.setCursor(0,2);
  lcd.print("This is a demo text");
  lcd.setCursor(0,3);
  lcd.print("I say hello to akvo");
  delay(1000);
  Serial.println("Arduino Digital Temperature // Serial Monitor Version"); //Print a message
  Serial.print("Total Temperature sensor: ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" Devices.");
}

void loop(void)
{ 
  delay(1000);
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Distance is ");
  lcd.print(distance);  
  lcd.setCursor(0,1);
  lcd.print("Temperature is ");
  sensors.requestTemperatures();
  lcd.print(sensors.getTempCByIndex(0));
  // Send the command to get temperatures
  
}
  /**sensors.requestTemperatures();  
  Serial.print("Temperature is: ");
  Serial.println(sensors.getTempCByIndex(0)); // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
  Update value every 1 sec.
  delay(1000);
  lcd.setCursor(0,3);
  lcd.print("                    ");
  lcd.setCursor(0,3);
  lcd.print("Temperature is ");
  lcd.print(sensors.getTempCByIndex(0));*/
 
