/////////////////////////////////
// Generated with a lot of love//
// with TUNIOT FOR ESP8266     //
// Website: Easycoding.tn      //
/////////////////////////////////
#include <ESP8266WiFi.h>

#include <ESP8266HTTPClient.h>

WiFiClient client;

String thingSpeakAddress = "http://api.thingspeak.com/update?";
String writeAPIKey;
String tsfield1Name;
String request_string;

HTTPClient http;



#include <SoftwareSerial.h>
SoftwareSerial s(D6, D5);
String data;


void setup() {
  s.begin(9600);
  Serial.begin(9600);

  //    WiFi.begin("it\'smee", "helloworld2");
  WiFi.begin("5465", "2635");
  Serial.print(".");
  delay(300);

}

void loop() {
  while (s.available())
  {
    delay(10);
    data += char(s.read());
  }
  if (data != "")
  { Serial.println(data);
    delay(100);
    if (client.connect("api.thingspeak.com", 80)) {
      request_string = thingSpeakAddress;
      request_string += "key=";
      request_string += "YQ3NO28DEZV8FPPX";
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
