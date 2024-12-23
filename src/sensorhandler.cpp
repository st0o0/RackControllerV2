#include <sensorhandler.h>

std::list<SensorData>::iterator SensorHandler::find(const std::string &device)
{
    return std::find_if(sensors.begin(), sensors.end(), [&device](const SensorData &sensor)
                        { return sensor.device == device; });
}

void SensorHandler::add(SensorData data)
{
    sensors.push_back(data);
}

void SensorHandler::update(SensorData data)
{
    auto it = find(data.device);
    if (it != sensors.end())
    {
        *it = data;
    }
}

bool SensorHandler::exists(const std::string &device)
{
    auto it = find(device);
    return it != sensors.end();
}

std::list<std::string> SensorHandler::getDevices()
{
    std::list<std::string> result;
    return result;
}

SensorData SensorHandler::getDeviceData(const std::string &device)
{
    auto it = find(device);
    return *it;
}

float SensorHandler::getAvgTemp()
{
    return 123.2;
}