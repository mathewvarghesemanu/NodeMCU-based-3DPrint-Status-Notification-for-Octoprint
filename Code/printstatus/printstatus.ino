/**
   Authorization.ino
    Created on: 09.12.2015
*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>
#include <ArduinoJson.h>
int ledpin = D0;
int buzzpin = D1;
ESP8266WiFiMulti WiFiMulti;
char apiurl[] = "http://192.168.1.50/api/job";// change the ip address to Raspberry pi IP Address
char authkey[] = "401D82B1BD4B47D6817DBD5C8A6AD297";// find authkey in octoprint application keys in settings
char id[] = "manu";//your wifi
char password[] = "ambalathil1234"; //your password
int state = true;
void setup() {
  pinMode(ledpin, OUTPUT);
  pinMode(buzzpin, OUTPUT);
  digitalWrite(buzzpin , 0);
  
  const int jsonSize = JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(2)
                       + JSON_OBJECT_SIZE(1);

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(id, password);

}

void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    WiFiClient client;

    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    // configure traged server and url


    http.begin(client, apiurl);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("X-Api-Key", authkey);
    //    http.setAuthorization(authkey);
    //    http.setAuthorization("X-Api-Key", authkey);


    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
        if (payload.indexOf("Printing") == -1)
        {
          digitalWrite(ledpin , 0);
          

          if (state == true)
          {
            digitalWrite(buzzpin , 1);
            delay(500);
            digitalWrite(buzzpin , 0);
            state = false;
          }

        }
        else
        {
          digitalWrite(ledpin , 1);
          Serial.print("Printing");
          state = true;
        }
        delay(5000);
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  delay(500);
}
