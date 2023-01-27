

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
  int httpResponseCode = http.POST("{\"api_key\":\"tPmAT5Ab3j7F9\",\"ReturnSensor\":"+ (String)returnSensor +",\"BoilerSensor\":"+ (String)boilerSensor +",\"pumpState\":"+ (String)pumpState +"}");

  if (httpResponseCode == 200) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    Serial.println(http.getString());
    // Serial.println(payload);

    connectionIcon.setPic(4);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
    connectionIcon.setPic(5);

  }

  http.end();
  return true;
}
