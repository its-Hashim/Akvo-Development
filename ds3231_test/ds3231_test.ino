#include <DS3231.h>
DS3231  rtc(SDA, SCL);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  rtc.begin();

}

void loop() {
  Time t = rtc.getTime();
  String d = rtc.getTimeStr();
  Serial.println(t.hour);
  Serial.println(t.min);
  Serial.println(d);
  // put your main code here, to run repeatedly:
  delay(1000);

}
