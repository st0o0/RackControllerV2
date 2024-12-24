#include <webserver.h>

void Webserver::begin()
{
    EspSerial.begin(115200);
    WiFi.init(EspSerial);
    if (WiFi.status() == WL_NO_MODULE)
    {
        wifi_module_failed();
    }
    WiFi.sleepMode(WIFI_NONE_SLEEP);
    WiFi.setHostname("ClusterControl ler");
    WiFi.disconnect();
    WiFi.endAP(true);
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println(F("Connecting to WiFi..."));
        int wifiStatus = WiFi.begin(ssid, password);

        if (wifiStatus == WL_CONNECTED)
        {
            WiFi.setAutoConnect(true);
            Serial.println();
            Serial.println(F("Connected to WiFi network."));
            IPAddress ip = WiFi.localIP();
            Serial.print("IP Address: ");
            Serial.println(ip);
        }
        else
        {
            WiFi.disconnect();
            Serial.println();
            Serial.println(F("Connection to WiFi network failed."));
        }
    }
    else
    {
        Serial.println(F("Already connected to WiFi."));
        IPAddress ip = WiFi.localIP();
        Serial.print("IP Address: ");
        Serial.println(ip);
    }
    server.on(F("/"), handle_root);
    server.on(F("/api/speed"), handle_api_speed);
    server.on(F("/api/device"), handle_api_device);
    server.on(F("/favicon.ico"), []()
              { server.send(404, F("text/plain"), F("") });
    server.onNotFound(handle_notfound);
    server.begin();
}

void Webserver::handle()
{
    server.handleClient();
}

void handle_root()
{
}

void handle_api_speed()
{
}

void handle_api_device()
{
}

void handle_notfound()
{
}

void wifi_module_failed()
{
    Serial.println();
    Serial.println(F("Communication with WiFi module failed!"));
    while (true)
    {
        delay(1500);
        Serial.println(F("Communication with WiFi module failed!"));
    };
}