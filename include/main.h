#ifndef main_h
#define main_h

#include <defines.h>
#include <wifi.h>
#include <helperfunction.h>
#include <fanhandler.h>

char ssid[] = "YOUR-SSID";              // your network SSID (name)
char password[] = "YOUR-PASSWORD"; // your network password

#define fanOutput_PWM_PIN 12
#define fanOutput_SENS_PIN 27
#define fanOutput_THRESHOLD 400

#define fanInput_PWM_PIN 11
#define fanInput_SENS_PIN 26
#define fanInput_THRESHOLD 400

#define switchingPower 10

#endif