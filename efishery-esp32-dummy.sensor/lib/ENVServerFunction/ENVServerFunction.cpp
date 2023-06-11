#include <ENVServerFunction.h>

BLECharacteristic TemperatureCharacteristics(TEMP_UUID, BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor TemperatureDescriptor(BLEUUID((uint16_t)0x2902));

BLECharacteristic HumidityCharacteristics(HUMIDITY_UUID, BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor HumidityDescriptor(BLEUUID((uint16_t)0x2902));

BLECharacteristic CO2Characteristics(CO2_UUID, BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor CO2Descriptor(BLEUUID((uint16_t)0x2902));

void sensorNodeCallbacks::onConnect(BLEServer* pServer){
    deviceConnected = true;
}

void sensorNodeCallbacks::onDisconnect(BLEServer* pServer){
    deviceConnected = false;
    pServer->getAdvertising()->start();
}

ENVFunction::ENVFunction() {}

ENVFunction::~ENVFunction() {}

void ENVFunction::serverStart()
{
    BLEDevice::init(BLEServerName);

    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new sensorNodeCallbacks());

    BLEService *envService = pServer->createService(ENVIRONMENT_UUID);

    envService->addCharacteristic(&TemperatureCharacteristics);

    TemperatureDescriptor.setValue("Temperature");
    TemperatureCharacteristics.addDescriptor(&TemperatureDescriptor);

    envService->addCharacteristic(&HumidityCharacteristics);
    HumidityDescriptor.setValue("Humidity");
    HumidityCharacteristics.addDescriptor(&HumidityDescriptor);

    envService->addCharacteristic(&CO2Characteristics);
    CO2Descriptor.setValue("CO2");
    CO2Characteristics.addDescriptor(&CO2Descriptor);

    envService->start();

    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(ENVIRONMENT_UUID);
    pServer->getAdvertising()->start();
    Serial.println("BLE Server Started");
}

void ENVFunction::setServerValue(int sensorProperty, float sensorValue)
{
    Serial.println("Masuk nih");
    static char value[6];
    dtostrf(sensorValue, 6, 2, value);
    if(sensorProperty == 0){
        TemperatureCharacteristics.setValue(value);
        TemperatureCharacteristics.notify();
    } else if (sensorProperty == 1){
        HumidityCharacteristics.setValue(value);
        HumidityCharacteristics.notify();
    } else if (sensorProperty == 2){
        CO2Characteristics.setValue(value);
        CO2Characteristics.notify();
    }
}