

bool sendData() {

  if(WiFi.status() != WL_CONNECTED) {
    sendConnectionErrorToDisplay();
    return 0;
  }

  WiFiClient client;
  HTTPClient http;

  String postSensorData = baseUrl;

  http.begin(client, baseUrl);
  http.addHeader("Content-Type", "application/json"); 
  int httpResponseCode = http.POST("{\"api_key\":\"tPmAT5Ab3j7F9\",\"sensor\":\"BME280\",\"value1\":\"24.25\",\"value2\":\"49.54\",\"value3\":\"1005.14\"}");

  if (httpResponseCode>0) {
    // Serial.print("HTTP Response code: ");
    // Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);

    DynamicJsonDocument doc(10);
    DeserializationError err = deserializeJson(doc, payload);

    if(err) {
        Serial.print(F("deserializeJson() failed with code: "));
        Serial.println(err.f_str());
    }

    if(doc.containsKey("received")) {
      const char* response = doc["received"];
      Serial.write(response);
    }
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
    // return false;
  }

  http.end();
  return true;
}

void sendConnectionErrorToDisplay() {
  //send connection error to display
}

