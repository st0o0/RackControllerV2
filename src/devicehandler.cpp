#include <devicehandler.h>

std::list<DeviceData>::iterator DeviceHandler::find(const std::string &name)
{
    return std::find_if(sensors.begin(), sensors.end(), [&device](const SensorData &sensor)
                        { return sensor.device == device; });
}

void DeviceHandler::add(DeviceData data)
{
    sensors.push_back(data);
}

void DeviceHandler::update(DeviceData data)
{
    auto it = find(data.device);
    if (it != sensors.end())
    {
        *it = data;
    }
}

bool DeviceHandler::exists(const std::string &name)
{
    auto it = find(device);
    return it != sensors.end();
}

std::list<std::string> DeviceHandler::getDevices()
{
    std::list<std::string> result;
    return result;
}

DeviceData DeviceHandler::getDeviceData(const std::string &name)
{
    auto it = find(device);
    return *it;
}

float DeviceHandler::getAvgTemp()
{
    return 123.2;
}