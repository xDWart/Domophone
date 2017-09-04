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
    pinMode(SOUND_RELAY, OUTPUT);
    ENABLE_SOUND();

    pinMode(PHONE_RELAY, OUTPUT);
    PICKUP_PHONE(false);

    pinMode(OPEN_RELAY, OUTPUT);
    CLOSE_DOOR();
}

void ENABLE_SOUND() {
    ENABLE_SOUND(true);
}

void ENABLE_SOUND(bool to) {
    relayStatus(SOUND_RELAY, !to);
}

void PICKUP_PHONE() {
    PICKUP_PHONE(true);
    ENABLE_SOUND(); //включить звук (перекинуть реле для ведения разговора)
}

void PICKUP_PHONE(bool to) {
    relayStatus(PHONE_RELAY, to);
}

void OPEN_DOOR() {
    relayStatus(OPEN_RELAY, true);
}

void CLOSE_DOOR() {
    relayStatus(OPEN_RELAY, false);
}
