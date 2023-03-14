bool setupWifiConnection () {
  unsigned long startInitializeWifiConnection = millis();
  WiFi.begin(ssid, password);

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial1.print(++i + ' ');

    if((millis() - startInitializeWifiConnection) > 10000) {
      Serial1.print("unable to connect retrying in 10 minutes");
      sendWifiErrorToDisplay();
      return false;
    }
  }
  return true;
}

void sendWifiErrorToDisplay() {
  wifiIcon.setPic(7);
}