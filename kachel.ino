#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const int oneWireBus = 4;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

String baseUrl = "http://192.168.2.49:80/api/";

unsigned long lastTimeDataSend = 0;
unsigned long sendDataDelay = 5000;
unsigned long refreshSensorsDelay = 1000;
unsigned long toneDelay = 1000;
unsigned long lastTimeToneDelay = 0;
unsigned long lastTimeRefreshSensor = 0;
bool wifiConneciton = 0;
float onTreshHold = 10.00;
float offTreshHold = 5.00;
int pump = D3;
bool pumpState = false;
bool enterdDangerZone = false;

float returnSensor = 0.00;
float boilerSensor = 0.00;

int buzzerPin = D1;

const char* ssid     = "Team_Deventer";
const char* password = "01012018Madrid";

void setup() {
  
  Serial.begin(115200);
  pinMode(pump, OUTPUT);
  digitalWrite(pump, LOW);
  delay(10);
  Serial.println('\n');
  sensors.begin();
  getTemperature();
  if(setupWifiConnection()){
    wifiConneciton = 1;
    Serial.println("wifi connected");
    Serial.println(WiFi.localIP());
  } else {
    wifiConneciton = 0;
  }
  
}

void loop() {
  getTemperature();

  if((millis() - lastTimeRefreshSensor) > toneDelay && enterdDangerZone) {
    tone(buzzerPin, 1000, 500);
    lastTimeRefreshSensor = millis();
  }

  if((millis() - lastTimeRefreshSensor) > refreshSensorsDelay) {

    manageTemperature();
    lastTimeRefreshSensor = millis();
  }

  if((millis() - lastTimeDataSend) > sendDataDelay && wifiConneciton == 1) {
    
    sendData();
    lastTimeDataSend = millis();
  }

}

void manageTemperature() 
{
  float diff = measureDifferenceBetweenSensors();
  Serial.println(diff);
  
  if(returnSensor >= 80.00 || boilerSensor >= 80.00) {
    Serial.print("test");
    turnPumpOn();
    enterdDangerZone = true;
    return;
  }

  if(returnSensor == -127) {
    turnPumpOn();
    return;
  }

  if(returnSensor < 70.00 || boilerSensor < 70.00 && enterdDangerZone) {
    enterdDangerZone = false;
  }
  

  if(diff > onTreshHold && !pumpState) {
    turnPumpOn();
    return;
  }

  if(diff < offTreshHold && pumpState) {
    turnPumpOff();
    return;
  }
}

void getTemperature() {
  
  sensors.requestTemperatures();
  returnSensor = sensors.getTempCByIndex(0);
  boilerSensor = sensors.getTempCByIndex(1);
}

float measureDifferenceBetweenSensors() {
  Serial.println(returnSensor);
  Serial.println(boilerSensor);
  return returnSensor - boilerSensor;
}

void turnPumpOn() {
  digitalWrite(pump, HIGH);
  pumpState = true;
}

void turnPumpOff() {
  digitalWrite(pump, LOW);
  pumpState = false;
}


