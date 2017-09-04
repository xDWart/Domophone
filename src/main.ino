#include "config/include.h"

void setup() {
  bootSetup();
  ledSetup();
  relaysSetup();
  initVar();
  eventsSetup();
}

void loop() {
  eventsloop();
  wifiLoop();
  MQTT_loop();
}
