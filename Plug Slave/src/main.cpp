/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com/esp8266-nodemcu-access-point-ap-web-server/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

// Import required libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>

const char* ssid     = "ESP8266-Access-Point";
const char* password = "123456789";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

WebSocketsServer webSocket = WebSocketsServer(81);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
  if (type == WStype_TEXT) {
    String message = String((char*)payload);
    Serial.println("Message: " + message);
    if (message == "ON") {
      digitalWrite(16, HIGH);
    } else if (message == "OFF") {
      digitalWrite(16, LOW);
    }
    
  }
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
  <input type="checkbox">
  <h2>ESP8266 %DHT% Server</h2>
</body>
</html>)rawliteral";

void setup(){
  // Serial port for debugging purposes
  Serial.begin(9600);
  
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());

    // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Start server
  server.begin();

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  pinMode(16, OUTPUT);
}
 
void loop(){  
  webSocket.loop();
}