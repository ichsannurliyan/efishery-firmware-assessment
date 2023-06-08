#ifndef ENV_SERVER_FUNCTION_H
#define ENV_SERVER_FUNCTION_H

#include <main.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define BLEServerName "ESP32_BLE_Server"
#define ENVIRONMENT_UUID "ef2944a8-05b0-11ee-be56-0242ac120002"
#define TEMP_UUID "ef2944a8-05b1-11ee-be56-0242ac120002"
#define CO2_UUID "ef2944a8-05b2-11ee-be56-0242ac120002"
#define HUMIDITY_UUID "ef2944a8-05b3-11ee-be56-0242ac120002"

#define TEMPERATURE_VALUE 0
#define HUMIDITY_VALUE 1
#define CO2_VALUE 2

extern bool deviceConnected;

class ENVFunction
{
    private:

    public:
    ENVFunction();
    ~ENVFunction();

    void serverStart();
    void setServerValue(int sensorProperty, float sensorValue);
};

class sensorNodeCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer);
    void onDisconnect(BLEServer* pServer);
};

#endif