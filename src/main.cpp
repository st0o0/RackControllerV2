#include <Arduino.h>
#include <defines.h>
#include <string>
#include <map>
#include <vector>
#include <numeric>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>

int status = WL_IDLE_STATUS;

const int FAN_OFF_TEMP = 0;
const int FAN_MAX_TEMP = 15;

WiFiWebServer server(80);

FanController outputFan(fanOutput_SENS_PIN, SENSOR_THRESHOLD1, fanOutput_PWM_PIN);
FanController inputFan(fanInput_SENS_PIN, SENSOR_THRESHOLD2, fanInput_PWM_PIN);

std::map<std::string, std::vector<int>> data = {};
unsigned maxTempValues = 5;
boolean checked = 0;
int setFanSpeed = 0;
int fanSpeed = 0;

bool to_bool(std::string str)
{
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  std::istringstream is(str);
  bool b;
  is >> std::boolalpha >> b;
  return b;
}

int fan_curve(float temp) {
  int spd = 100 * (temp - FAN_OFF_TEMP) / (FAN_MAX_TEMP - FAN_OFF_TEMP);
  return min(max(0, spd), 100);
}

int calc_PiTemp()
{
  int result = 0;
  for (auto const &x : data)
  {
    result = result + (accumulate(x.second.begin(), x.second.end(), 0.0) / x.second.size());
  }
  return result / data.size();
}

const char *GetTable()
{
  std::string table = "<table style='width:100%'>";
  table.append("<tr>");
  table.append("<th scope='col'>&nbsp;</th>");
  for (auto const &x : data)
  {
    table += "<th scope='col'>" + x.first + "</th>";
  }
  for (int i = 0; i < maxTempValues + 1; ++i)
  {
    table.append("</tr><tr>");
    table.append("<th scope='row'>");
    table.append(String(i).c_str());
    table.append("</th>");
    for (const std::pair<std::string, std::vector<int>> &sv : data)
    {
      table.append("<td>");
      table.append(String(sv.second.at(i)).c_str());
      table.append("</td>");
    }
  }
  table.append("</table></br>");
  return table.c_str();
}

void SetFanSpeed(int spd)
{
  byte speed = byte(spd);
  outputFan.setDutyCycle(speed);
  inputFan.setDutyCycle(speed);
  fanSpeed = spd;
}

void handleNotFound()
{
  String message = F("File Not Found\n\n");

  message += F("URI: ");
  message += server.uri();
  message += F("\nMethod: ");
  message += (server.method() == HTTP_GET) ? F("GET") : F("POST");
  message += F("\nArguments: ");
  message += server.args();
  message += F("\n");

  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, F("text/plain"), message);
}

void handleRoot()
{
#define BUFFER_SIZE 4096

  char temp[BUFFER_SIZE];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;
  int day = hr / 24;

  snprintf(temp, BUFFER_SIZE - 1, "<!DOCTYPE html>\
<html lang='de'>\
<head>\
    <meta http-equiv='refresh' content='60' />\
    <title>%s</title>\
    <style>\
        body {\
            background-color: #cccccc;\
            font-family: Arial, Helvetica, Sans-Serif;\
            Color: #000088;\
        }\
        input[type='range'] {\
            vertical-align: middle;\
        }\
    </style>\
</head>\
<body>\
    <h1>Hello from %s</h1>\
    <h3>running WiFiWebServer</h3>\
    <h3>on %s</h3>\
    <p>Uptime: %d d %02d:%02d:%02d</p>\
    </br>\
    <form oninput='x.value=parseInt(range.value)' method='post' action='/setfanspeed'>\
        <Label for='checkbox'>Disable auto fanspeed: </Label>\
        <input id='checkbox' type='checkbox' value='true' %s name='check'>\
        <input type='hidden' name='check' value='false'>\
        </br>\
        <Label for='range'>manual fanspeed: </Label>\
        <input id='range' type='range' min='0' max='100' value='%d' name='speed'>\
        </br>\
        </br>\
        <Label for='output'>setfanspeed: </Label>\
        <output id='output' name='x' for='range'>%d</output>\
        </br>\
        </br>\
        <input type='submit' value='set FanSpeed'>\
    </form>\
    </br>\
    </br>\
    <Label>FanSpeed: %d</Label>\
    </br>\
    </br>\
    </br>\
    %s\
</body>\
</html>", BOARD_NAME, BOARD_NAME, SHIELD_TYPE, day, hr % 24, min % 60, sec % 60, checked ? "checked='checked'" : "", setFanSpeed, setFanSpeed, fanSpeed, GetTable());

  server.send(200, F("text/html"), F(temp));
}

void handleAbsoluteFanSpeed()
{
  if (server.method() == HTTP_POST)
  {
    setFanSpeed = server.arg("speed").toInt();
    checked = to_bool(server.arg("check").c_str());
    server.sendHeader("Location", "/");
    server.send(303, "");
  }
}

void handleRaspberryTemp()
{
  if (server.method() == HTTP_POST)
  {
    std::string pi = server.arg("pi").c_str();
    int temp = std::stoi(server.arg("temp").c_str());
    if (data.count(pi))
    {
      data[pi].insert(data[pi].begin(), temp);
      if (data[pi].size() > maxTempValues)
      {
        data[pi].pop_back();
      }
    }
    else
    {
      data.insert(std::pair<std::string, std::vector<int>>(pi, std::vector<int>()));
      data[pi].insert(data[pi].begin(), temp);
    }
  }
}

void FanSetup()
{
  pinMode(fanInput_SENS_PIN, INPUT_PULLUP);
  pinMode(fanOutput_SENS_PIN, INPUT_PULLUP);
  pinMode(fanInput_PWM_PIN, OUTPUT);
  pinMode(fanOutput_PWM_PIN, OUTPUT);

  analogWriteResolution(8);
  analogWriteFrequency(fanOutput_PWM_PIN, 25000);
  analogWriteFrequency(fanInput_PWM_PIN, 25000);

  inputFan.begin();
  outputFan.begin();
  SetFanSpeed(10);
}

void WiFiSetup()
{
  EspSerial.begin(115200);
  WiFi.init(EspSerial);
}

void InitSetup()
{
  Serial.print(F("\nStarting AdvancedWebServer on "));
  Serial.print(BOARD_NAME);
  Serial.print(F(" with "));
  Serial.println(SHIELD_TYPE);
  Serial.println(WIFI_WEBSERVER_VERSION);

  WiFiSetup();

  if (WiFi.status() == WL_NO_MODULE)
  {
    Serial.println();
    Serial.println(F("Communication with WiFi module failed!"));
    // don't continue
    while (true)
    {
      delay(1500);
      Serial.println(F("Communication with WiFi module failed!"));
    };
  }

  WiFi.sleepMode(WIFI_NONE_SLEEP);
  WiFi.setHostname("Teensy");
  WiFi.disconnect();
  WiFi.setPersistent();
  WiFi.endAP(true);

  status = WiFi.begin(ssid, pass);
  if (status == WL_CONNECTED)
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

  FanSetup();

  server.on(F("/"), handleRoot);
  server.on(F("/setfanspeed"), handleAbsoluteFanSpeed);
  server.on(F("/pi"), handleRaspberryTemp);
  server.on(F("/favicon.ico"), []()
            { server.send(404, F("text/plain"), F("")); });

  server.onNotFound(handleNotFound);
  server.begin();
}

void setup()
{
  Serial.begin(115200);
  delay(200);

  Serial.println(F("startet InitSetup"));
  InitSetup();
}

void heartBeatPrint()
{
  static int num = 1;

  Serial.print(F("H"));

  if (num == 80)
  {
    Serial.println();
    num = 1;
  }
  else if (num++ % 10 == 0)
  {
    Serial.print(F(" "));
  }
}

void check_status()
{
  static unsigned long checkstatus_timeout = 0;

#define STATUS_CHECK_INTERVAL 60000L

  // Send status report every STATUS_REPORT_INTERVAL (60) seconds: we don't need to send updates frequently if there is no status change.
  if ((millis() > checkstatus_timeout) || (checkstatus_timeout == 0))
  {
    heartBeatPrint();
    checkstatus_timeout = millis() + STATUS_CHECK_INTERVAL;
  }
}

void loop()
{
  server.handleClient();
  check_status();

  if (!checked)
  {
    int temp = calc_PiTemp();
    int speed = fan_curve(temp);
    SetFanSpeed(speed);
  }
  else
  {
    SetFanSpeed(setFanSpeed);
  }
}