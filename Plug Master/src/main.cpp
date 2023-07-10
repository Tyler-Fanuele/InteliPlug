#include <ESP8266WiFi.h>
#include <espnow.h>

// Message struct with string for message
typedef struct struct_message {
  String msg;
} struct_message;

// Holds teh mac address of the slave of the system
// This will be changed to accept a list of mac addresses from a
// serial port later to allow for many slaves
uint8_t broadcastAddress[] = {0xC8, 0xC9, 0xA3, 0xB9, 0x4E, 0x2E};

// This is the on send function for the master
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
  
  // Init wifi
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Init esp-now and set role
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  // Register a on send function
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}
 
void loop(){
  // Wait for serial input
  while (Serial.available() == 0) {} 
  String incomingString = Serial.readString();
  incomingString.trim();

  Serial.println("Received Command '" + incomingString + "':");

  if (incomingString == "mac") {
    // 'mac' was seen on serial and will now print mac address
    Serial.print("ESP8266 Board MAC Address:  ");
    Serial.println(WiFi.macAddress());
  } else if (incomingString == "send") {
    // 'send' was seen on serial and will send a message over esp-now
    struct_message myData;
    myData.msg = "in master";
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  } else {
    // error case, unrecognised command seen over serial
    Serial.println("Command: '" + incomingString+ "' is not recognised!");
  }
}