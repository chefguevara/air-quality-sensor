#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// Sensor ID provided by the API
const char* sensorId = "602d193d7e6cea75382a840b";

// URL
String Protocol = "http";
String Host = "192.168.1.12";
String Port = "3000";
String Path = "/api/sensor/";

String serverURL = Protocol + "://" + Host  + ":" + Port + Path + sensorId;

int HTTP_TRIGGER = 6;

void sendHttpRequest(int32_t pressure, int32_t iaq, int32_t iaqAccuracy, float temperature, float humidity, int32_t co2Equivalent) {
  DynamicJsonDocument   root(512);
  JsonObject data = root.createNestedObject("data");

  data["pressure"] = pressure;
  data["iaq"] = iaq;
  data["iaqAccuracy"] = iaqAccuracy;
  data["temperature"] = temperature;
  data["humidity"] = humidity;
  data["co2Equivalent"] = co2Equivalent;
 
  root["sensor"] = sensorId;
  // TODO Move to HTTPS
  String requestBody;
  serializeJson(root, requestBody);

  HTTPClient http;
  http.begin(serverURL);
  http.addHeader("Authorization", "Basic dGVzdDp0ZXN0");
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(requestBody);
  if(httpResponseCode>0){
    String response = http.getString();                       
    Serial.println(httpResponseCode);   
    Serial.println(response);
  }
  else {
    Serial.println("Error occurred while sending HTTP POST: ");
    Serial.println( http.errorToString(httpResponseCode));
  }
}
