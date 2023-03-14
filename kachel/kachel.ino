#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Nextion.h"
SoftwareSerial HMISerial(D5, D6);

const int oneWireBus = 4;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

NexText boiler = NexText(0, 2, "boiler");
NexText kachel = NexText(0, 3, "kachel");
NexPicture auto_man = NexPicture(0, 7, "auto_man");
NexPicture wifiIcon = NexPicture(0, 5, "p1");
NexPicture connectionIcon = NexPicture(0, 5, "p2");
NexPicture pumpNex = NexPicture(0, 6, "pump");
NexText EMControl = NexText(0,8, "EMControl");

NexTouch *nex_listen_list[] = 
{
    &auto_man,
    NULL
};

String baseUrl = "http://192.168.2.49:3000/post";

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
bool emControl = false;

float returnSensor = 0.00;
float boilerSensor = 0.00;

int buzzerPin = D1;
bool buzzerState = false;

bool Auto = true;
bool Auto2 = true;

const char* ssid     = "Team_Deventer";
const char* password = "01012018Madrid";

void auto_manPopCallBack(void *ptr)
{

  Serial.println("start pop");
  Auto = !Auto;
    if(Auto) {
      auto_man.setPic(11);
      Serial.println("aan");
      return;
    }
    auto_man.setPic(10);
    Serial.println("uit");
}

void setup() {
  
  Serial.begin(9600);

  nexInit();
  auto_man.attachPop(auto_manPopCallBack);
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
    wifiIcon.setPic(6);
  } else {
    wifiConneciton = 0;
  }
  
}

void loop() {
  nexLoop(nex_listen_list);
  getTemperature();

  if((millis() - lastTimeToneDelay) > toneDelay && emControl) {
    tone(buzzerPin, 2000, 500);
    lastTimeToneDelay = millis();
  }

  if((millis() - lastTimeRefreshSensor) > refreshSensorsDelay && Auto) {
    manageTemperature();
    lastTimeRefreshSensor = millis();
    sendDataToDisplay();
  }

  if((millis() - lastTimeDataSend) > sendDataDelay && wifiConneciton == 1) {
    
    sendData();
    lastTimeDataSend = millis();
  }

  if(!Auto) {
    turnPumpOn();
  }

  delay(500);
}

void manageTemperature() 
{
  float diff = measureDifferenceBetweenSensors();
  Serial.println(diff);

  
  if(returnSensor >= 80.00 || boilerSensor >= 80.00) {
    EMControl.setText("EMControl");
    turnPumpOn();
    emControl = true;
    return;
  }

  if(returnSensor == -127) {
    turnPumpOn();
    return;
  }

  if(returnSensor < 70.00 || boilerSensor < 70.00 && emControl) {
    EMControl.setText(" ");
    emControl = false;
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
  return returnSensor - boilerSensor;
}

void turnPumpOn() {
  pumpNex.setPic(8);
  digitalWrite(pump, HIGH);
  pumpState = true;
}

void turnPumpOff() {
  pumpNex.setPic(9);
  digitalWrite(pump, LOW);
  pumpState = false;
}

void sendDataToDisplay() {
  char Boiler[5];
  char Return[5];
  dtostrf(returnSensor, 4, 1, Return);
  dtostrf(boilerSensor, 4, 1, Boiler);
  boiler.setText(Boiler);
  kachel.setText(Return);
  Serial.println(Return); 
}
