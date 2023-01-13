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