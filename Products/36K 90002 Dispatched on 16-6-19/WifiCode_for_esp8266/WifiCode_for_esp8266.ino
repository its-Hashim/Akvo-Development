#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

#include <ESP8266HTTPClient.h>

WiFiClient client;

String thingSpeakAddress = "http://api.thingspeak.com/update?";
String writeAPIKey = "FN9WW4WESTXZ96BH";
String tsfield1Name;
String request_string;

HTTPClient http;



#include <SoftwareSerial.h>
SoftwareSerial s(D6, D5);
String data;


void setup() {
  delay(1000);
  s.begin(9600);
  Serial.begin(9600);
  //    WiFi.begin("it\'smee", "helloworld2");
  WiFiManager wifiManager;
  wifiManager.autoConnect("36K 90002");
  Serial.println("connected...yeey :)");

}

void loop() {
  while (s.available())
  {
    delay(10);
    data += char(s.read());
  }
  if (data != "")
  {
    Serial.println(data);
    delay(100);
    if (client.connect("api.thingspeak.com", 80))
    {
      request_string = thingSpeakAddress;
      request_string += "key=";
      request_string += writeAPIKey;
      request_string += "&";
      request_string += data;
      Serial.println(request_string);
      http.begin(request_string);
      http.GET();
      http.end();
    }
    data = "";
  }

}
