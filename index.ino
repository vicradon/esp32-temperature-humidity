#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino.h>
#include <DHT.h>

const char* ssid = "<YOUR_WIFI_SSID>";
const char* password = "<YOUR_WIFI_PASSWORD>";

float temperature, humidity;
DHT dht22_sensor(13, DHT22);

HTTPClient http;
http.begin(directusEndpoint);
http.addHeader("Content-Type", "application/json");
http.addHeader("Authorization", directusToken);

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  dht22_sensor.begin();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Error in WiFi connection");
  }

  temperature = dht22_sensor.readTemperature();
  humidity = dht22_sensor.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Error reading sensor data");
  }

  String jsonPayload = "{\"temperature\":" + String(temperature) + ",\"humidity\":" + String(humidity) + "}";
  int httpResponseCode = http.POST(jsonPayload);

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    String response = http.getString();
    Serial.println(response);
  } else {
    Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpResponseCode).c_str());
  }

  http.end();

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("°C <-> Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

 delay(5000);
}