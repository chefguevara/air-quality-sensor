#include <ESP8266WiFi.h>

const char* ssid = "Schnitzel";
const char* password = "LaritenBoniten!2020";

void connectToWiFi(){
  WiFi.begin(ssid, password);
  
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  
}
