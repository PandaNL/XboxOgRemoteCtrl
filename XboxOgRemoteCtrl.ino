#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>

int PIN_XBOX = D1;
int PIN_DRIVE = D5;

ESP8266WebServer server(80);
WiFiManager wifiManager;

void setup() {
  
  pinMode(PIN_XBOX, OUTPUT);
  pinMode(PIN_DRIVE, OUTPUT);

  WiFi.hostname("XboxOgRemoteCtrl");
  wifi_station_set_hostname("XboxOgRemoteCtrl");
  
  wifiManager.setConfigPortalTimeout(180);
  wifiManager.autoConnect("XboxOgRemoteCtrl");
  
  if(WiFi.status() != WL_CONNECTED) {
    ESP.restart();
    return;
  }
  
  ArduinoOTA.setPort(80);
  ArduinoOTA.setHostname("XboxOgRemoteCtrl");
  ArduinoOTA.begin();
  
  if (MDNS.begin("XboxOgRemoteCtrl")) {
    MDNS.addService("http", "tcp", 80);
    MDNS.addService("XboxOgRemoteCtrl", "tcp", 80);
  }

  server.on("/driveToggle", HTTP_GET, _onHttpDriveToggle);
  server.on("/xboxToggle", HTTP_GET, _onHttpXboxToggle);
  server.begin();
  
}

void loop() {
  server.handleClient();
  ArduinoOTA.handle();
}

void _onHttpDriveToggle() {
  driveToggle();
  server.send(200);
}

void driveToggle() {
  digitalWrite(PIN_DRIVE, HIGH);
  delay(500);
  digitalWrite(PIN_DRIVE, LOW);
}

void _onHttpXboxToggle() {
  xboxToggle();
  server.send(200);
}

void xboxToggle() {
  digitalWrite(PIN_XBOX, LOW);
  delay(500);
  digitalWrite(PIN_XBOX, HIGH);
}
