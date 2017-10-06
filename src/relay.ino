#include "config/include.h"

bool relayStatus(uint8_t id) {
    return digitalRead(id);
}

bool relayStatus(uint8_t id, bool status) {
    digitalWrite(id, status);
    return status;
}

bool relayToggle(uint8_t id) {
    return relayStatus(id, !relayStatus(id));
}

void relaysSetup() {
    pinMode(RELAY2, OUTPUT);
    DOMOPHONE(true);

    pinMode(RELAY1, OUTPUT);
    PICKUP_PHONE(false);

    pinMode(OPEN_RELAY, OUTPUT);
    CLOSE();
}

void DOMOPHONE(bool to) {
    relayStatus(RELAY2, !to);
}

void PICKUP_PHONE() {
    PICKUP_PHONE(true);
    DOMOPHONE(false); //перекинуть реле для перехвата управления
}

void PICKUP_PHONE(bool to) {
    relayStatus(RELAY1, to);
}

void OPEN() {
    relayStatus(OPEN_RELAY, true);
    Serial.println("Open");
}

void CLOSE() {
    relayStatus(OPEN_RELAY, false);
    Serial.println("Close");
}
