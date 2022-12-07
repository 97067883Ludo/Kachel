#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

String baseUrl = "http://192.168.2.49:8000/api/";

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;
bool wifiConneciton = 0;
const int pump = D1;

const char* ssid     = "Team_Deventer";
const char* password = "01012018Madrid";

bool setupWifiConnection () {
  unsigned long startInitializeWifiConnection = millis();
  WiFi.begin(ssid, password);

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(++i + ' ');

    if((millis() - startInitializeWifiConnection) > 10000) {
      Serial.print("unable to connect retrying in 10 minutes");
      return false;
    }
  }
  return true;
}

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println('\n');
  pinMode(pump, OUTPUT);

  if(setupWifiConnection()){
    wifiConneciton = 1;
    Serial.println("wifi connected");
    Serial.println(WiFi.localIP());
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  if((millis() - lastTime) > timerDelay) {
    Serial.println(wl_status_to_string(WiFi.status()));

    WiFiClient client;
    HTTPClient http;

    http.begin(client, baseUrl);
    int httpResponseCode = http.GET();

    if (httpResponseCode>0) {
      // Serial.print("HTTP Response code: ");
      // Serial.println(httpResponseCode);
      String payload = http.getString();
      // Serial.println(payload);

      DynamicJsonDocument doc(180);
      DeserializationError err = deserializeJson(doc, payload);

      if(err) {
          Serial.print(F("deserializeJson() failed with code: "));
          Serial.println(err.f_str());
      }

      if(doc.containsKey("ontvangen")) {
        const char* response = doc["ontvangen"];
        Serial.write(response);
        digitalWrite(pump, !digitalRead(pump));
      }

    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }

      http.end();

    lastTime = millis();
  }

}

const char* wl_status_to_string(wl_status_t status) {
  switch (status) {
    case WL_NO_SHIELD: return "WL_NO_SHIELD";
    case WL_IDLE_STATUS: return "WL_IDLE_STATUS";
    case WL_NO_SSID_AVAIL: return "WL_NO_SSID_AVAIL";
    case WL_SCAN_COMPLETED: return "WL_SCAN_COMPLETED";
    case WL_CONNECTED: return "WL_CONNECTED";
    case WL_CONNECT_FAILED: return "WL_CONNECT_FAILED";
    case WL_CONNECTION_LOST: return "WL_CONNECTION_LOST";
    case WL_DISCONNECTED: return "WL_DISCONNECTED";
  }
}