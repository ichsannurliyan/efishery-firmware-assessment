#ifndef ENV_CLIENT_FUNCTION_H
#define ENV_CLIENT_FUNCTION_H

#include <main.h>
#include <BLEDevice.h>

#define bleServerName "ESP32_BLE_Server"

#define ENVIRONMENT_UUID "ef2944a8-05b0-11ee-be56-0242ac120002"
#define TEMPERATURE_UUID "ef2944a8-05b1-11ee-be56-0242ac120002"
#define CO2_UUID "ef2944a8-05b2-11ee-be56-0242ac120002"
#define HUMIDITY_UUID "ef2944a8-05b3-11ee-be56-0242ac120002"

#define TEMPERATURE_DATA 0
#define HUMIDITY_DATA 1
#define CO2_DATA 2

static BLEUUID temperatureCharacteristicUUID(TEMPERATURE_UUID);
static BLEUUID humidityCharacteristicUUID(HUMIDITY_UUID);
static BLEUUID co2CharacteristicUUID(CO2_UUID);

extern boolean doConnect;
extern boolean connected;

extern BLEAddress *pServerAddress;

static BLERemoteCharacteristic* temperatureCharacteristic;
static BLERemoteCharacteristic* humidityCharacteristic;
static BLERemoteCharacteristic* co2Characteristic;

const uint8_t notificationOn[] = {0x1, 0x0};
const uint8_t notificationOff[] = {0x0, 0x0};

extern boolean newTemperature, newHumidity, newCo2;

class ENVBLEClient
{
    private:
    char* temperatureChar, humidityChar, co2Char;

    public:
    ENVBLEClient();
    ~ENVBLEClient();

    bool connectToServer(BLEAddress pAddress);
    void init();
    void getDescriptor();
    String getValue(int dataType);

};

class ENVBLECallback
{
    private:
    char *temperatureChar, *humidityChar, *co2Char;

    public:
    ENVBLECallback();
    ~ENVBLECallback();

    static void temperatureNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);
    static void humidityNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);
    static void co2NotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);
    String extractDatatoString(int dataType);
};

class ENVDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.getName() == bleServerName) {
      advertisedDevice.getScan()->stop();
      pServerAddress = new BLEAddress(advertisedDevice.getAddress());
      doConnect = true;
      Serial.println("Device found. Connecting!");
    }
  }
};

#endif