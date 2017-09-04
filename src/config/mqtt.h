#ifndef MQTT_H_
#define MQTT_H_

#include <ESP8266WiFi.h>

const char *mqtt_server = "m20.cloudmqtt.com";
const int mqtt_port = 12234;
const char *mqtt_user = "test";
const char *mqtt_pass = "test";

const bool retained = true;

String prefix   = "/IoTmanager";
String deviceID = "Dom";
String ids = "e090cabf-2a89-470b-34fe-a747361a9c7d"; //your phone ids

// Push notifications
const char* host = "onesignal.com";
const int httpsPort = 443;
String url = "/api/v1/notifications";

String msg = "Звонок в домофон";
String msg1 = "Ожидание";
String msg2 = "Открываю";
String msg3 = "Сбрасываю";

typedef struct
{
  String sTopic;
  String stat;
  bool st;
} vars_type;

enum {
  WAIT = false,
  CALL = true
};

enum {
 CALLING = 0,
 OPENONCE,
 OPENALL,
 RESETALL,
 SOUND,
 CONNECT,
 WCOUNT
};

enum {
 ST_WAITING,
 ST_CALLING,
 ST_OPENING,
 ST_RESETING
};

vars_type Var[WCOUNT];
bool SavedVar[WCOUNT];
String thing_config[WCOUNT];
bool DomophoneStatus = WAIT;

#endif //MQTT_H_
