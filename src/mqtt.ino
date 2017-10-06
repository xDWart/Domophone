#include "config/include.h"

//===================vars=================
uint32_t oldtime, newtime;
uint8_t switchCount = 0;
uint8_t changeSt = 0;
uint8_t openCount = 0;
bool changeTo;

WiFiClient wifiClient;
PubSubClient client(wifiClient, mqtt_server, mqtt_port);
WiFiClientSecure httpClient;

void initVar() {
  DynamicJsonBuffer jsonBuffer;

  JsonObject& root = jsonBuffer.createObject();
  Var[CALLING].sTopic = prefix + "/" + deviceID + "/calling";
  setStatusCall(ST_WAITING);
  root["id"] = String(CALLING);
  root["widget"] = "anydata";
  root["topic"]  = Var[CALLING].sTopic;
  root["descr"]  = msg1;
  root["class1"] = "text-center";
  root["style1"] = "padding-top:10px;";
  root["class2"] = "rounded";
  root["style2"] = "background-color:green;color:green;font-size:20px;font-weight:bold;padding:10px 0px;border:2px solid white;border-radius:15px;";
  root["class3"] = "";
  root["style3"] = "display:none;";
  root.printTo(thing_config[CALLING]);

  JsonObject& root1 = jsonBuffer.createObject();
  Var[OPENQUIET].sTopic = prefix + "/" + deviceID + "/openquiet";
  setStatus(&Var[OPENQUIET], false);
  root1["id"] = String(OPENQUIET);
  root1["widget"] = "toggle";
  root1["topic"]  = Var[OPENQUIET].sTopic;
  root1["descr"]  = "Открыть беззвучно";
  root1["style1"] = "padding-top:10px;";
  root1["descrStyle"] = "font-size:20px;";
  root1.printTo(thing_config[OPENQUIET]);

  Var[OPENONCE].sTopic = prefix + "/" + deviceID + "/openonce";
  setStatus(&Var[OPENONCE], false);
  root1["id"] = String(OPENONCE);
  root1["widget"] = "toggle";
  root1["topic"]  = Var[OPENONCE].sTopic;
  root1["descr"]  = "Открыть со звуком";
  root1["style1"] = "padding-top:10px;";
  root1["descrStyle"] = "font-size:20px;";
  root1.printTo(thing_config[OPENONCE]);

  Var[OPENALL].sTopic = prefix + "/" + deviceID + "/openall";
  setStatus(&Var[OPENALL], false);
  root1["id"] = String(OPENALL);
  root1["widget"] = "toggle";
  root1["topic"]  = Var[OPENALL].sTopic;
  root1["descr"]  = "Открывать всем";
  root1["style1"] = "padding-top:10px;";
  root1["descrStyle"] = "font-size:20px;";
  root1.printTo(thing_config[OPENALL]);

  Var[RESETALL].sTopic = prefix + "/" + deviceID + "/resetall";
  setStatus(&Var[RESETALL], false);
  root1["id"] = String(RESETALL);
  root1["widget"] = "toggle";
  root1["topic"]  = Var[RESETALL].sTopic;
  root1["descr"]  = "Сбрасывать всем";
  root1["style1"] = "padding-top:10px;";
  root1["descrStyle"] = "font-size:20px;";
  root1.printTo(thing_config[RESETALL]);

  Var[SOUND].sTopic = prefix + "/" + deviceID + "/sound";
  setStatus(&Var[SOUND], true);
  root1["id"] = String(SOUND);
  root1["widget"] = "toggle";
  root1["topic"]  = Var[SOUND].sTopic;
  root1["descr"]  = "Звук на трубке";
  root1["style1"] = "padding-top:10px;";
  root1["descrStyle"] = "font-size:20px;";
  root1.printTo(thing_config[SOUND]);

  Var[CONNECT].sTopic = prefix + "/" + deviceID + "/connect";
  Var[CONNECT].stat = "";
  root["id"] = String(CONNECT);
  root["widget"] = "anydata";
  root["topic"]  = Var[CONNECT].sTopic;
  root["descr"]  = " Есть контакт";
  root["class1"] = "text-center";
  root["style1"] = "padding-top:20px;";
  root["class2"] = "ion-ios-checkmark";
  root["style2"] = "color:green;font-size:16px;";
  root["class3"] = "";
  root["style3"] = "display:none;";
  root.printTo(thing_config[CONNECT]);
}

void push(String msg) {
  Serial.println("PUSH: try to send push notification...");
  if (ids.length() == 0) {
     Serial.println("PUSH: ids not received, push failed");
     return;
  }
  if (!httpClient.connect(host, httpsPort)) {
     Serial.println("PUSH: connection failed");
     return;
  }
  String data = "{\"app_id\": \"8871958c-5f52-11e5-8f7a-c36f5770ade9\",\"include_player_ids\":[\"" + ids + "\"],\"android_group\":\"IoT Manager\",\"contents\": {\"en\": \"" + msg + "\"}}";
  httpClient.println("POST " + url + " HTTP/1.1");
  httpClient.print("Host:");
  httpClient.println(host);
  httpClient.println("User-Agent: esp8266.Arduino.IoTmanager");
  httpClient.print("Content-Length: ");
  httpClient.println(data.length());
  httpClient.println("Content-Type: application/json");
  httpClient.println("Connection: close");
  httpClient.println();
  httpClient.println(data);
  httpClient.println();
  Serial.println(data);
  Serial.println("PUSH: done.");
}

void setStatusCall (uint8_t call) {
    Var[CALLING].st = call;
    Var[CALLING].stat = "{\"status\":\"";
    switch (call) {
        case ST_CALLING:
            Var[CALLING].stat  = Var[CALLING].stat + msg + "\", \"style2\" : \"display:none;\"," +
                "\"class2\" : \"\"," +
                "\"style3\" : \"background-color:red;color:white;font-size:20px;font-weight:bold;padding:10px 0px;border:2px solid white;border-radius:15px;\"," +
                "\"class2\" : \"rounded\"," +
                "\"beep\" : \"1\"," +
                "\"vibrate\": \"200\"";
        break;
        case ST_WAITING:
            Var[CALLING].stat  = Var[CALLING].stat + msg1 + "\", \"style2\" : \"display:none;\"," +
                "\"class2\" : \"\"," +
                "\"style3\" : \"background-color:green;color:white;font-size:20px;font-weight:bold;padding:10px 0px;border:2px solid white;border-radius:15px;\"," +
                "\"class2\" : \"rounded\"";
        break;
        case ST_OPENING:
            Var[CALLING].stat  = Var[CALLING].stat + msg2 + "\", \"style2\" : \"display:none;\"," +
            "\"class2\" : \"\"," +
            "\"style3\" : \"background-color:yellow;color:black;font-size:20px;font-weight:bold;padding:10px 0px;border:2px solid white;border-radius:15px;\"," +
            "\"class2\" : \"rounded\"";
        break;
        case ST_RESETING:
            Var[CALLING].stat  = Var[CALLING].stat + msg3 + "\", \"style2\" : \"display:none;\"," +
            "\"class2\" : \"\"," +
            "\"style3\" : \"background-color:yellow;color:black;font-size:20px;font-weight:bold;padding:10px 0px;border:2px solid white;border-radius:15px;\"," +
            "\"class2\" : \"rounded\"";
        break;
    }
    Var[CALLING].stat += "}";
}

void setStatus(vars_type * var, bool s) {
    var->st = s;
    var->stat = "{\"status\":\"" + String(s) + "\"}";
}

void pubStatus(String t, String payload) {
    if (client.publish((t + "/status").c_str(), (uint8_t*)payload.c_str(), payload.length(), retained)) {
       Serial.println("Publish new status for " + t + ", value: " + payload);
    } else {
       Serial.println("Publish new status for " + t + " FAIL!");
    }
}

void pubConfig() {
     bool success;
     for (char i = 0; i < WCOUNT-1; i++) {
        success = client.publish((prefix + "/" + deviceID + String(i, DEC) + "/config").c_str(), (uint8_t*)thing_config[i].c_str(), thing_config[i].length(), retained);
        if (success) {
          Serial.println("Publish config: Success (" + thing_config[i] + ")");
        } else {
          Serial.println("Publish config FAIL! ("    + thing_config[i] + ")");
        }
        delay(150);
     }

     for (char i = 0; i < WCOUNT-1; i++) {
        pubStatus(Var[i].sTopic, Var[i].stat);
        delay(100);
     }
}

void pubConfigConnect() {
        bool success;
        success = client.publish((prefix + "/" + deviceID + "/config").c_str(), (uint8_t*)thing_config[CONNECT].c_str(), thing_config[CONNECT].length(), false);
        if (success) {
          Serial.println("Publish config: Success (" + thing_config[CONNECT] + ")");
        } else {
          Serial.println("Publish config FAIL! ("    + thing_config[CONNECT] + ")");
        }
}

void callback(const MQTT::Publish& sub) {
    Serial.print("Message arrived [");
    Serial.print(sub.topic());
    Serial.print("] ");
    Serial.println(sub.payload_string());

    if (sub.topic() == Var[OPENQUIET].sTopic + "/control") {
        changeSt = OPENQUIET;
    } else if (sub.topic() == Var[OPENONCE].sTopic + "/control") {
        changeSt = OPENONCE;
    } else if (sub.topic() == Var[SOUND].sTopic + "/control") {
        changeSt = SOUND;
    } else if (sub.topic() == Var[OPENALL].sTopic + "/control") {
        changeSt = OPENALL;
    } else if (sub.topic() == Var[RESETALL].sTopic + "/control") {
        changeSt = RESETALL;
    } else if (sub.topic() == prefix + "/ids") {
        ids = sub.payload_string();
    } else if (sub.topic() == prefix) {
          if (sub.payload_string() == "HELLO") {
            pubConfigConnect();
          }
    }

    if (changeSt > 0) {
        changeTo = !(sub.payload_string() == "0");
        setStatus(&Var[changeSt], changeTo);
        pubStatus(Var[changeSt].sTopic, Var[changeSt].stat);
        switch (changeSt) {
            case SOUND:
                DOMOPHONE(Var[SOUND].st);
            break;
            case OPENQUIET:
            case OPENONCE:
            case OPENALL:
            case RESETALL:
                if (Var[changeSt].st) {
                    if (changeSt != OPENQUIET) {
                        DOMOPHONE(Var[SOUND].st);
                        setStatus(&Var[OPENQUIET], false);
                        pubStatus(Var[OPENQUIET].sTopic, Var[OPENQUIET].stat);
                    } else {
                        DOMOPHONE(!changeTo);
                        SavedVar[OPENQUIET] = Var[OPENQUIET].st;
                        if (Var[CALLING].st == ST_CALLING) {
                            setStatusCall(ST_OPENING);
                            pubStatus(Var[CALLING].sTopic, Var[CALLING].stat);
                        }
                    }
                    if (changeSt != OPENONCE) {
                        setStatus(&Var[OPENONCE], false);
                        pubStatus(Var[OPENONCE].sTopic, Var[OPENONCE].stat);
                    } else {
                        SavedVar[OPENONCE] = Var[OPENONCE].st;
                        if (Var[CALLING].st == ST_CALLING) {
                            setStatusCall(ST_OPENING);
                            pubStatus(Var[CALLING].sTopic, Var[CALLING].stat);
                        }
                    }
                    if (changeSt != OPENALL) {
                        setStatus(&Var[OPENALL], false);
                        pubStatus(Var[OPENALL].sTopic, Var[OPENALL].stat);
                    } else {
                        SavedVar[OPENALL] = Var[OPENALL].st;
                        if (Var[CALLING].st == ST_CALLING) {
                            setStatusCall(ST_OPENING);
                            pubStatus(Var[CALLING].sTopic, Var[CALLING].stat);
                        }
                    }
                    if (changeSt != RESETALL) {
                        setStatus(&Var[RESETALL], false);
                        pubStatus(Var[RESETALL].sTopic, Var[RESETALL].stat);
                    } else {
                        SavedVar[RESETALL] = Var[RESETALL].st;
                        if (Var[CALLING].st == ST_CALLING) {
                            setStatusCall(ST_RESETING);
                            pubStatus(Var[CALLING].sTopic, Var[CALLING].stat);
                        }
                    }
                }
            break;
        }
        changeSt = 0;
    }
}

void MQTT_loop() {
    if (WiFi.status() == WL_CONNECTED) {
        if (!client.connected()) {
            Serial.print("Connecting to MQTT server ... ");
            if (client.connect(MQTT::Connect("ESP8266Client").set_auth(mqtt_user,mqtt_pass))){
                client.set_callback(callback);
                Serial.println("Success");
                client.subscribe(prefix.c_str());
                client.subscribe((prefix + "/ids").c_str());
                for (char i = 1; i < WCOUNT-1; i++) {
                    client.subscribe((Var[i].sTopic + "/control").c_str());
                }
                pubConfig();
                pubConfigConnect();
                ledStatus(ESP_LED,true);
            } else {
                Serial.print("FAIL");
                delay(5000);
            }
        } else {
            client.loop();
        }
    }

    newtime = millis();
    if (!DomophoneStatus) { //в режиме ожидания
        if (newtime - oldtime > 100) {
            oldtime = newtime;
            switchCount = 0;
        } else if (switchCount % 2 == 0) {
            if (!digitalRead(IN_PIN)) {
                switchCount++;
                oldtime = newtime;
                Serial.println("Call " + String(switchCount));
            }
        } else {
            if (digitalRead(IN_PIN)) {
                switchCount++;
                oldtime = newtime;
                Serial.println("Call " + String(switchCount));
            }
        }
        if (switchCount > 10) {
            DomophoneStatus = CALL;
            openCount = 0;

            SavedVar[OPENQUIET] = Var[OPENQUIET].st;
            SavedVar[OPENONCE] = Var[OPENONCE].st;
            SavedVar[OPENALL] = Var[OPENALL].st;
            SavedVar[RESETALL] = Var[RESETALL].st;

            if (!(SavedVar[OPENQUIET] || SavedVar[OPENONCE] || SavedVar[OPENALL] || SavedVar[RESETALL])) {
                push(msg);
                setStatusCall(ST_CALLING);
            } else {
                if (SavedVar[RESETALL]) setStatusCall(ST_RESETING);
                else setStatusCall(ST_OPENING);
            }

            Serial.println("Incoming call");
            pubStatus(Var[CALLING].sTopic, Var[CALLING].stat);

            SetEvent(EVNT_DOMOPHONE_WAIT,10000);
        }
    } else { //в режиме разговора
        if (SavedVar[OPENQUIET] || SavedVar[OPENONCE] || SavedVar[OPENALL]) { //открыть
            if (openCount < 3) {//Впустить пару раз
                ClrEvent(EVNT_DOMOPHONE_WAIT);
                if (openCount == 0) PICKUP_PHONE(); //Поднять трубку
                if (TestEvent_Clr(EVNT_OPEN_DOOR) && TestEvent_Clr(EVNT_CLOSE_DOOR)) {
                    SetEvent(EVNT_OPEN_DOOR,1500);
                    SetEvent(EVNT_CLOSE_DOOR, 2500);
                    openCount++;
                    Serial.println("Open " + String(openCount));
                }
            } else {
                if (openCount == 3) {
                    if (TestEvent_Clr(EVNT_OPEN_DOOR) && TestEvent_Clr(EVNT_CLOSE_DOOR)) {
                        Serial.println("Putdown phone after open");
                        SetEvent(EVNT_PUTDOWN_PHONE); //Повесить трубку

                        if (SavedVar[OPENONCE]) { //Сбросить флаг открыть разово
                            setStatus(&Var[OPENONCE], false);
                            pubStatus(Var[OPENONCE].sTopic, Var[OPENONCE].stat);
                        }

                        if (SavedVar[OPENQUIET]) { //Сбросить флаг открыть разово
                            setStatus(&Var[OPENQUIET], false);
                            pubStatus(Var[OPENQUIET].sTopic, Var[OPENQUIET].stat);
                        }

                        openCount++;
                    }
                }
            }
        } else if (SavedVar[RESETALL]) { //сбросить
            if (openCount == 0) {
                ClrEvent(EVNT_DOMOPHONE_WAIT);
                PICKUP_PHONE(); //Поднять трубку
                Serial.println("Putdown phone after reset");
                SetEvent(EVNT_PUTDOWN_PHONE,1500); //Повесить трубку
                openCount++;
            }
        } else {
            if (!digitalRead(IN_PIN)) SetEvent(EVNT_DOMOPHONE_WAIT,10000); //низкий уровень больше 5 сек? на ожидание
        }
    }
}
