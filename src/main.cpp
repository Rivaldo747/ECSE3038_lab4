#include <Arduino.h>
#include <Wifi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "env.h"

char * endpoint = "https://ecse-three-led.netlify.app/";
int led_pins []= {18,19,21};
bool led_states[] = {false, false, false}; //boolean variables to show whether the value is true or false.
void setup() {
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  Serial.println("Connecting to wifi");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop(){
  // making the Get request to the server
   HTTPClient http;
  http.begin(server_address);
  http.addHeader("X-API-Key", "Eugenie#7012");
  int http_status = http.GET();

  // put your main code here, to run repeatedly:
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;

    String http_response;


    http.begin(endpoint);

    int httpResponseCode = http.GET();

    if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);

        Serial.print("Response from server: ");
        http_response = http.getString();
        Serial.println(http_response);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
  }
  if (http_status == HTTP_CODE_OK) {
    // Parse JSON response
    String response = http.getString();
  StaticJsonDocument<1024> doc;

      DeserializationError error = deserializeJson(doc, http_response);
      

 if (error) {
      Serial.print("JSON parsing error: ");
      Serial.println(error.c_str());
      return;
    }

    // Update LED states
    led_states[0] = doc["led1"];
    led_states[1] = doc["led2"];
    led_states[2] = doc["led3"];

    // Set LED states in circuit
    for (int i = 0; i < 3; i++) {
      digitalWrite(led_pins[i], led_states[i]);
    }
   }else {
    Serial.print("HTTP error: ");
    Serial.println(http_status);
  }

  delay(1000);
}



