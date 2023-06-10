#include <Arduino.h>
#include <WiFi.h>
#include <BLEDevice.h>

#define bleServerName "ESP32_BLE_Server"

#define ENVIRONMENT_UUID "ef2944a8-05b0-11ee-be56-0242ac120002"
#define TEMPERATURE_UUID "ef2944a8-05b1-11ee-be56-0242ac120002"
#define CO2_UUID "ef2944a8-05b2-11ee-be56-0242ac120002"
#define HUMIDITY_UUID "ef2944a8-05b3-11ee-be56-0242ac120002"

static BLEUUID temperatureCharacteristicUUID(TEMPERATURE_UUID);
static BLEUUID humidityCharacteristicUUID(HUMIDITY_UUID);
static BLEUUID co2CharacteristicUUID(CO2_UUID);

static boolean doConnect = false;
static boolean connected = false;

static BLEAddress *pServerAddress;

static BLERemoteCharacteristic* temperatureCharacteristic;
static BLERemoteCharacteristic* humidityCharacteristic;
static BLERemoteCharacteristic* co2Characteristic;

const uint8_t notificationOn[] = {0x1, 0x0};
const uint8_t notificationOff[] = {0x0, 0x0};

char* temperatureChar;
char* humidityChar;
char* co2Char;

boolean newTemperature, newHumidity, newCo2;

static void temperatureNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);
static void humidityNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);
static void co2NotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify);

bool connectToServer(BLEAddress pAddress) {
  BLEClient *pClient = BLEDevice::createClient();

  pClient->connect(pAddress);
  Serial.println("Connected to the server");

  BLERemoteService* pRemoteService = pClient->getService(ENVIRONMENT_UUID);
  if (pRemoteService == nullptr) {
    Serial.print("Failed to find the Service UUID");
    Serial.println(ENVIRONMENT_UUID);
    return (false);
  }

  temperatureCharacteristic = pRemoteService->getCharacteristic(temperatureCharacteristicUUID);
  humidityCharacteristic = pRemoteService->getCharacteristic(humidityCharacteristicUUID);
  co2Characteristic = pRemoteService->getCharacteristic(co2CharacteristicUUID);

  if (temperatureCharacteristic == nullptr || humidityCharacteristic == nullptr || co2Characteristic == nullptr) {
    Serial.print("Failed to find our characteristic UUID");
    return false;
  }
  Serial.println(" - Found our characteristics");
 
  //Assign callback functions for the Characteristics
  temperatureCharacteristic->registerForNotify(temperatureNotifyCallback);
  humidityCharacteristic->registerForNotify(humidityNotifyCallback);
  co2Characteristic->registerForNotify(co2NotifyCallback);
  return true;
}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.getName() == bleServerName) {
      advertisedDevice.getScan()->stop();
      pServerAddress = new BLEAddress(advertisedDevice.getAddress());
      doConnect = true;
      Serial.println("Device found. Connecting!");
    }
  }
};

static void temperatureNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
  temperatureChar = (char*)pData;
  newTemperature = true;
}

static void humidityNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) 
{
  humidityChar = (char*)pData;
  newHumidity = true;
}

static void co2NotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) 
{
  co2Char = (char*)pData;
  newCo2 = true;
}

void setup() {
 // put your setup code here, to run once:
 Serial.begin(115200);
 
 BLEDevice::init("");

 BLEScan *pBLEScan = BLEDevice::getScan();
 pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
 pBLEScan->setActiveScan(true);
 pBLEScan->start(30, true);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (doConnect == true) {
    if (connectToServer(*pServerAddress)) {
      Serial.println("We are now connected to the BLE Server.");
      
      temperatureCharacteristic->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)notificationOn, 2, true);
      humidityCharacteristic->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)notificationOn, 2, true);
      co2Characteristic->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)notificationOn, 2, true);
      connected = true;
    } else {
      Serial.println("We have failed to connect to the server; Restart your device to scan for nearby BLE server again.");
    }
    doConnect = false;
  }
  if (newTemperature && newHumidity && newCo2){
    newTemperature = false;
    newHumidity = false;
    newCo2 = false;
    Serial.print("Temperature: ");
    Serial.print(temperatureChar);
    Serial.println(" C");
    Serial.print("Humidity: ");
    Serial.print(humidityChar);
    Serial.println(" %");
    Serial.print("CO2: ");
    Serial.print(co2Char);
    Serial.println(" ppm");
  }
  delay(5000);
}