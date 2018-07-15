#include <DHTesp.h>


/**
 * BasicHTTPClient.ino
 *
 *  Created on: 24.05.2015
 *
 */

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial


ESP8266WiFiMulti WiFiMulti;

/** Initialize DHT sensor */
DHTesp dht;
/** Pin number for DHT11 data pin */
int dhtPin = 2;

void setup() {

    USE_SERIAL.begin(115200);
   // USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println("init....");
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    WiFiMulti.addAP("SSID", "password");

    // Initialize temperature sensor
    dht.setup(dhtPin, DHTesp::DHT22);

}

void loop() {  
// wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;

        TempAndHumidity lastValues = dht.getTempAndHumidity();
        String temp = String(lastValues.temperature,1);
        String hum = String(lastValues.humidity,0);
        Serial.println("Temperature: " + String(lastValues.temperature,1));
        Serial.println("Humidity: " + String(lastValues.humidity,0));  

        USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("https://192.168.1.12/test.html", "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
        String url = "http://192.168.1.78:8080/json.htm?type=command&param=udevice&idx=23&nvalue=0&svalue=" + temp + ";" + hum + ";0";
        http.begin(url); //HTTP

        USE_SERIAL.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                USE_SERIAL.println(payload);
            }
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }

    delay(5* 60 * 1000);
}

