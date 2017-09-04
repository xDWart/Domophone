#include "config/include.h"

bool ledStatus(uint8_t pin) {
    return !digitalRead(pin);
}

bool ledStatus(uint8_t pin, bool status) {
    digitalWrite(pin, !status);
    return status;
}

bool ledToggle(uint8_t pin) {
    return ledStatus(pin, !ledStatus(pin));
}

void ledSetup() {
    pinMode(ESP_LED, OUTPUT);
    ledStatus(ESP_LED, false);

    pinMode(NODE_LED, OUTPUT);
    ledStatus(NODE_LED, false);
}
