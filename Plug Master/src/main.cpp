#include <ESP8266WiFi.h>
#include <espnow.h>

typedef struct struct_message {
  String msg;
} struct_message;

uint8_t broadcastAddress[] = {0xC8, 0xC9, 0xA3, 0xB9, 0x4E, 0x2E};

void OnDataSent(uint8_t* mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}

void setup(){
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}
 
void loop(){
  while (Serial.available() == 0) {} 
  String incomingString = Serial.readString();
  incomingString.trim();

  Serial.println("Received Command '" + incomingString + "':");

  if (incomingString == "mac") {
    Serial.print("ESP8266 Board MAC Address:  ");
    Serial.println(WiFi.macAddress());
  } else if (incomingString == "send") {
    struct_message myData;
    myData.msg = "in master";
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  } else {
    Serial.println("Command: '" + incomingString+ "' is not recognised!");
  }
}