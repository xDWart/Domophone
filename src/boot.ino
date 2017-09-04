#include "config/include.h"

void bootSetup() {
    ArduinoOTA.begin();
    Serial.begin(SERIAL_BAUDRATE);
    delay(100);
    DEBUG_MSG("\n");
    DEBUG_MSG("[BOOT] %s v.%s\n", (char *) APP_NAME, (char *) APP_VERSION);
    DEBUG_MSG("[BOOT] Memory size: %d bytes\n", ESP.getFlashChipSize());
    DEBUG_MSG("[BOOT] Free heap: %d bytes\n", ESP.getFreeHeap());
    DEBUG_MSG("[BOOT] Firmware size: %d bytes\n", ESP.getSketchSize());
    DEBUG_MSG("[BOOT] Free firmware space: %d bytes\n", ESP.getFreeSketchSpace());
}
