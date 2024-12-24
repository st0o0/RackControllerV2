#include <devicehandler.h>

std::list<DeviceData>::iterator DeviceHandler::find(const std::string &name)
{
    return std::find_if(devices.begin(), devices.end(), [&name](const DeviceData &device)
                        { return device.name == name; });
}

void DeviceHandler::add(DeviceData data)
{
    devices.push_back(data);
}

void DeviceHandler::update(DeviceData data)
{
    auto it = find(data.name);
    if (it != devices.end())
    {
        *it = data;
    }
}

bool DeviceHandler::exists(const std::string &name)
{
    auto it = find(name);
    return it != devices.end();
}

std::list<std::string> DeviceHandler::getDevices()
{
    std::list<std::string> result;
    return result;
}

DeviceData DeviceHandler::getDeviceData(const std::string &name)
{
    auto it = find(name);
    return *it;
}

float DeviceHandler::getAvgTemp()
{
    return 123.2;
}