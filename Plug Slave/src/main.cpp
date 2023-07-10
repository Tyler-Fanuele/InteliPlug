#include <ESP8266WiFi.h>

void setup(){
  Serial.begin(115200);
}
 
void loop(){
  while (Serial.available() == 0) {} 
  String incomingString = Serial.readString();
  incomingString.trim();

  Serial.println("Received Command '" + incomingString + "':");

  if (incomingString == "mac") {
    Serial.print("ESP8266 Board MAC Address:  ");
    Serial.println(WiFi.macAddress());
  } else {
    Serial.println("Command: '" + incomingString+ "' is not recognised!");
  }
}