/////////////////////////////////
// Generated with a lot of love//
// with TUNIOT FOR ESP8266     //
// Website: Easycoding.tn      //
/////////////////////////////////
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

#include <ESP8266HTTPClient.h>

WiFiClient client;

String thingSpeakAddress = "http://api.thingspeak.com/update?";
String writeAPIKey = "JKAVT3EOBRAXK6E8";        // For AIS 12
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
  wifiManager.autoConnect("AKVO_AIS_12_WIFI_CONFIG");
//  Serial.println("connected...yeey :)");

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
//      Serial.println(request_string);
      http.begin(request_string);
      http.GET();
      http.end();
    }
    data = "";
  }

}
