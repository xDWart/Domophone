#include "config/include.h"

IPAddress ip(192,168,1,128);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

void wifiLoop() {
    ArduinoOTA.handle();
    if (WiFi.status() != WL_CONNECTED) {
        ledStatus(ESP_LED,false);
        DEBUG_MSG("[WIFI] Connecting to WiFi...\n");
        WiFi.mode(WIFI_STA);
        WiFi.begin(SSID, PASSWORD);
        WiFi.config(ip,gateway,subnet);

        if (WiFi.waitForConnectResult() != WL_CONNECTED) return;
        IPAddress myIP = WiFi.localIP();
        DEBUG_MSG("[WIFI] WiFi connected\n");
        DEBUG_MSG("[WIFI] IP address: %d.%d.%d.%d\n",myIP[0],myIP[1],myIP[2],myIP[3]);
        }
}
