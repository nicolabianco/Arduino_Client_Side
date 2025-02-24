#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

const char* ssid = "XXXXXXXXXXXXX";
const char* password = "XXXXXXXXXXXX";
const char* serverIP = "XXXXXXXXXXXX";
const int serverPort = 80;

WiFiClient client;

#define DHTTYPE    DHT11
#define DHTPIN 2
DHT dht(DHTPIN, DHTTYPE);

float t = 0.0;
float h = 0.0;

unsigned long previousMillis = 0;
const long interval = 43200000;

void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected");
   Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
 unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    float newT = dht.readTemperature();
    if (isnan(newT)) {
      Serial.println("Failed to read temperature from DHT sensor!");
    }
    else {
      Serial.println("temperatura: " + String(newT));
    }

String payload = "{\"temperatura\":" + String(newT) + "}";
String httpRequest = "POST /data/receiveData HTTP/1.1\r\n";
httpRequest += "Host: " + String(serverIP) + ":" + String(serverPort) + "\r\n";
httpRequest += "Content-Type: application/json\r\n";
httpRequest += "Content-Length: " + String(payload.length()) + "\r\n";
httpRequest += "Connection: close\r\n\r\n";

if (client.connect(serverIP, serverPort)) {
    Serial.println("Server online, request starting...");
    client.print(httpRequest);
    client.print(payload);
    delay(10);
    client.stop();
    Serial.println("request finished");
   }else{
    Serial.println("server offline");
   }
 }
}
