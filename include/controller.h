#ifndef controller_h
#define controller_h

#include <defines.h>
#include <devicehandler.h>
#include <fanhandler.h>

struct controllerconfig
{
    int fan_off_temp;
    int fan_max_temp;
};


class Controller
{
private:
    FanHandler& fanHandler;
    DeviceHandler& deviceHandler;
    int calc_fancurve(float temp);

public:
    Controller(FanHandler& fans, DeviceHandler& devices) : fanHandler(fans), deviceHandler(devices) {}
    void begin();
    void handle();
}


#endif