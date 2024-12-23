#ifndef sensorhandler_h
#define sensorhandler_h

#include <defines.h>

struct SensorData
{
    std::string device;
    std::string lastUpdate;
    Moving_Average<float_t, float_t, 5> tempAvg;
};

struct RequestData
{
    std::string device;
    std::string timestamp;
    float temperature;
};

class SensorHandler
{
private:
    std::list<SensorData> sensors;
    std::list<SensorData>::iterator find(const std::string& device);

public:
    void add(SensorData data);
    void update(SensorData data);
    bool exists(const std::string& device);
    std::list<std::string> getDevices();
    SensorData getDeviceData(const std::string& device);
    float getAvgTemp();
};

#endif