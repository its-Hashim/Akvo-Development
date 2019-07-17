#include <DHT.h>
#include <DHT_U.h>

// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain
 
// DHT_dual_test
// Demonstrates multiple sensors
// Modified sketch by DIY-SciB.org
 
#define DHT1PIN 5     // what pin we're connected to
 
// Uncomment whatever type you're using!
#define DHT1TYPE DHT22   // DHT 11 
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
 
// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor
 
DHT dht1(DHT1PIN, DHT1TYPE);
 
void setup() {
  Serial.begin(9600); 
  Serial.println("DHTxx test!");
 
  dht1.begin();

}
 
void loop() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h1 = dht1.readHumidity();
  float t1 = dht1.readTemperature();

 
  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t1) || isnan(h1)) {
    Serial.println("Failed to read from DHT #1");
  } else {
    Serial.print("Humidity 1: "); 
    Serial.print(h1);
    Serial.print(" %\t");
    Serial.print("Temperature 1: "); 
    Serial.print(t1);
    Serial.println(" *C");
  }
  
  Serial.println();
  delay(2000);
}
