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
//String writeAPIKey = "LR8JSVQW4CPF3OLM";        // For AIS 16
//String writeAPIKey = "MTYJ4XAXJMYT5I46";        // For AIS 17
//String writeAPIKey = "EYED23XYG4AFI82W";        // For AIS 18
//String writeAPIKey = "AAUES1QBD6JU1GCK";        // For AIS 19
//String writeAPIKey = "2F08U29MYW2XYBHM";        // For AIS 20
//String writeAPIKey = "KNS2GJ8NYYO56JTE";        // For AIS 21
//String writeAPIKey = "XLPMM0SMBSS4M9TH";        // For AIS 22
//String writeAPIKey = "RXGYG92RT92HDV0O";        // For AIS 23
//String writeAPIKey = "XHBU3M236XXGUQUK";        // For AIS 24
//String writeAPIKey = "OFDK7HE53L2TZTXT";        // For AIS 25
//String writeAPIKey = "EULUHI373DTKYG14";        // For AIS 26
//String writeAPIKey = "0H0R7WG7017WD3TQ";        // For AIS 27
//String writeAPIKey = "0BSAMJI1O2UO94RC";        // For AIS 28
//String writeAPIKey = "EOFRV498WG4I9KCP";        // For AIS 29
//String writeAPIKey = "YSST8CG91V7B6YHC";        // For AIS 30
//String writeAPIKey = "HP5O76M73JOX078G";        // For AIS 31
//String writeAPIKey = "44AAZMPGAQ3UFMWC";        // For AIS 32
//String writeAPIKey = "XEEUURLJAD4GDRKL";        // For AIS 33
//String writeAPIKey = "MTYJ4XAXJMYT5I46";        // For AIS 34
//String writeAPIKey = "MTYJ4XAXJMYT5I46";        // For AIS 35
//String writeAPIKey = "MTYJ4XAXJMYT5I46";        // For AIS 36
//String writeAPIKey = "MTYJ4XAXJMYT5I46";        // For AIS 37
//String writeAPIKey = "MTYJ4XAXJMYT5I46";        // For AIS 38
//String writeAPIKey = "MTYJ4XAXJMYT5I46";        // For AIS 39
//String writeAPIKey = "MTYJ4XAXJMYT5I46";        // For AIS 40
//String writeAPIKey = "MTYJ4XAXJMYT5I46";        // For AIS 41
//String writeAPIKey = "MTYJ4XAXJMYT5I46";        // For AIS 42
//String writeAPIKey = "MTYJ4XAXJMYT5I46";        // For AIS 43
//String writeAPIKey = "MTYJ4XAXJMYT5I46";        // For AIS 44
//String writeAPIKey = "MTYJ4XAXJMYT5I46";        // For AIS 45
//String writeAPIKey = "MTYJ4XAXJMYT5I46";        // For AIS 46

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
  wifiManager.autoConnect("AKVO_AIS_16_WIFI_CONFIG");
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
