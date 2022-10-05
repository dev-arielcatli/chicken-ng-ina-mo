#include <SoftwareSerial.h>

SoftwareSerial nodeMCU(2, 3); // RX, TX

void setup() {
  Serial.begin(115200);
  while(!Serial) {
    ;
  }

  Serial.println("DEBUGGING CONNECTED");
  nodeMCU.begin(115200);
}

void loop() {
  if(nodeMCU.available()) {
    Serial.write(nodeMCU.read());
  }
}
