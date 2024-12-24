#ifndef webserver_h
#define webserver_h

#include <defines.h>
#include <wifi.h>
#include <devicehandler.h>
#include <fanhandler.h>

struct RequestData
{
    std::string device;
    std::string timestamp;
    float temperature;
};

class Webserver
{
private:
    WiFiWebServer server = WiFiWebServer(80);
    DeviceHandler &deviceHandler;
    FanHandler &fanHandler;

public:
    Webserver(DeviceHandler &devices, FanHandler &fans) : deviceHandler(devices), fanHandler(fans) {}
    void begin();
    void handle();
}

#endif