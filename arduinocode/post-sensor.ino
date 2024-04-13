#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "DHT.h"

#define DHTPIN 2     
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE); 

const char* ssid = "Tilincito";
const char* password =  "tzoclp10";
const char* serverName = "http://192.168.205.162:3000/sensor";

int cont = 300;
String ip;

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

void setup() {
  Serial.begin(115200);
  analogReadResolution(10);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(3000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");

  dht.begin(); 
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    float temperatura = dht.readTemperature();
    float humedad = dht.readHumidity();

    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;

      DynamicJsonDocument doc(1024);
      doc["nombre"] = "Lizani";
      doc["correo"] = "lruanom5@gmail.com";
      doc["carnet"] = "0902-21-18568";
      doc["latitud"] = 15.47083;
      doc["longitud"] = -90.37083;
      doc["temperatura"] = temperatura;
      doc["humedad"] = humedad;

      String jsonData;
      serializeJson(doc, jsonData);

      http.begin(client, serverName);
      http.addHeader("Content-Type", "application/json");
      int httpResponseCode = http.POST(jsonData);
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      http.end();
    } else {
      Serial.println("WiFi Disconnected");
    }

    lastTime = millis();
  }
}