#ifndef sensorhandler_h
#define sensorhandler_h

#include <defines.h>

struct SensorData
{
    String deviceName;
    String lastUpdate;
    Moving_Average<float_t, float_t, 5> tempAvg;
};

struct RequestData
{
    String device;
    String timestamp;
    float temperature;
}

class SensorHandler
{
private:
public:
    void add(SensorData data);
    void update(SensorData data);
    String[] getDevices();
    any getDeviceData(String device);
    float getAvgTemp();
}

#endif