#include <ESP8266WiFi.h>
#include <espnow.h>

typedef struct struct_message {
  String msg;
} struct_message;


uint8_t broadcastAddress[] = {0x48, 0x55, 0x19, 0xDF, 0x29, 0xAD};

void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  struct_message myData;
  memcpy(&myData, incomingData, sizeof(myData) );
  Serial.println(myData.msg);
}

void setup(){
  Serial.begin(115200);
    WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop(){
  /**/
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