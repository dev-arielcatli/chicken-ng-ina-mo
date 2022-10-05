#include<ESP8266WiFi.h> 

const char* NETWORK_ID = "ChickenNgInaMo";      // WIFI SSID
const char* PASSWORD = "kagatinMoManokKo"; // WIFI Password

void setup() {
  Serial.begin(115200);
  Serial.println();

  Serial.println("Setting up soft AP...");
  Serial.println(WiFi.softAP(NETWORK_ID, PASSWORD)? "[SUCCESSFUL]" : "[FAILED]");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  Serial.println("OPEN");
  delay(1000);
  Serial.println(WiFi.softAPIP());
}
