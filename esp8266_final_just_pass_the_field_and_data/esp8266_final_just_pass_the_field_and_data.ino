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

void setup()
{
  WiFi.begin("it\'smee", "helloworld2");
  Serial.begin(9600);

}


void loop()
{
  String data = "";
  while (Serial.available())
  {
    delay(10);
    data += char(Serial.read());
  }
  if (data != "")
  { Serial.println(data);
    delay(100);
    if (client.connect("api.thingspeak.com", 80)) {
      request_string = thingSpeakAddress;
      request_string += "key=";
      request_string += "YQ3NO28DEZV8FPPX";
      request_string += "&";
      request_string += "field2";
      request_string += "=";
      request_string += data;
      request_string += "&field1=101";
      Serial.println(request_string);
      http.begin(request_string);
      http.GET();
      http.end();

    }
  }
}
