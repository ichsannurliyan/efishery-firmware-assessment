#ifndef ADL_MODEMPORTAL_H
#define ADL_MODEMPORTAL_H

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
// #include <SSLClient.h>
// #include <ArduinoHttpClient.h>
#include <FS.h>
#include <SPIFFS.h>
#include <Preferences.h>
#include <ArduinoJson.h>
#include <main.h>
// #include <modemlog.h>

#define PREFERENCE_NAME "devicedata"

#define DEVICEMODEL_KEY "devicemodel"
#define SERIALNUM_KEY "serialnumber"
#define DEVICEID_KEY "deviceId"
#define SSID_KEY "ssid"
#define PASS_KEY "wifiPass"
#define GATEWAYID_KEY "gatewayid"
#define TIMEZONE_KEY "timezone"
#define FWVERSION_KEY "fwversion"
#define HWVERSION_KEY "hwversion"
#define DEVICEIP_KEY "deviceip"
#define ACCESSPOINTIP_KEY "gatewayIP"
#define SUBNET_KEY "subnet"
#define UPTIME_KEY "deviceUpTime"
#define INTERNETSTAT_KEY "internetStatus"
#define APN_KEY "apn"
#define INTERNETUSER_KEY "internetuser"
#define INTERNETPASS_KEY "internetpass"
#define BOOTTIMEOUT_KEY "wdtboottime"
#define NETWORKMODE_KEY "networkmode"

#define AUTH_KEY "authorization"

#define PREFERENCE_STATE "devicestate"

#define SECONDON_KEY "secondOnState"
#define VALID_KEY "validState"

#define PREFERENCE_DATA "meterdata"

#define HISTORICAL_KEY "periodhistorical"


#define HARDCODE_MQTT_DATA

extern Preferences preferences;

extern String temperatureData, humidityData, co2Data;

String convertTime();

class AsyncPortal {
private:
    const char* http_username = "admin";
    const char* http_password = "admin"; 

public:
  AsyncPortal();
  ~AsyncPortal();

  void createLoginServer();
  void createMainServer();
  void createDeviceServer();
  void launchWeb();
  void setupAP();
  // bool deviceIdValidation(String deviceId);
};

#endif