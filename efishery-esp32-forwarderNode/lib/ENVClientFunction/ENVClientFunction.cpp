#include <ENVClientFunction.h>

ENVBLECallback cb;

ENVBLEClient::ENVBLEClient(){}

ENVBLEClient::~ENVBLEClient(){}

bool ENVBLEClient::connectToServer(BLEAddress pAddress) {
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
  temperatureCharacteristic->registerForNotify(cb.temperatureNotifyCallback);
  humidityCharacteristic->registerForNotify(cb.humidityNotifyCallback);
  co2Characteristic->registerForNotify(cb.co2NotifyCallback);
  return true;
}

void ENVBLEClient::init()
{
    BLEDevice::init("");
    BLEScan *pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new ENVDeviceCallbacks());
    pBLEScan->setActiveScan(true);
    pBLEScan->start(30, true);
}

void ENVBLEClient::getDescriptor()
{
    temperatureCharacteristic->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)notificationOn, 2, true);
    humidityCharacteristic->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)notificationOn, 2, true);
    co2Characteristic->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)notificationOn, 2, true);
}

String ENVBLEClient::getValue(int dataType)
{
    String data;
    data = cb.extractDatatoString(dataType);
    return data;
}

ENVBLECallback::ENVBLECallback() {}

ENVBLECallback::~ENVBLECallback() {}

void ENVBLECallback::temperatureNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) 
{
    cb.temperatureChar = (char*)pData;
    newTemperature = true;
}

void ENVBLECallback::humidityNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) 
{
    cb.humidityChar = (char*)pData;
    newHumidity = true;
}

void ENVBLECallback::co2NotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) 
{
    cb.co2Char = (char*)pData;
    newCo2 = true;
}

String ENVBLECallback::extractDatatoString(int dataType)
{
    String data;
    if (dataType == 0){
        data = temperatureChar;
    } else if (dataType == 1){
        data = humidityChar;
    } else if (dataType == 2){
        data = co2Char;
    }

    return data;
}