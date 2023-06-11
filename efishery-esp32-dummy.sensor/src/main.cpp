#include <main.h>
#include <Arduino.h>
#include <ENVServerFunction.h>
#include <ENVDummy.h>

float temp, humi, co2;
unsigned long previousTime = 0, currentTime = 15000;
bool deviceConnected;

ENVFunction BLE;
ENVDummy Sensor;

void setup() {
  Serial.begin(115200);
  BLE.serverStart();
}

void loop() {
  if (deviceConnected) 
  {
    if (millis() - previousTime > currentTime)
    {
      temp = Sensor.readTemperature();
      humi = Sensor.readHumidity();
      co2 = Sensor.readCO2();
      BLE.setServerValue(TEMPERATURE_VALUE, temp);
      BLE.setServerValue(HUMIDITY_VALUE, humi);
      BLE.setServerValue(CO2_VALUE, co2);
      Serial.print("Temperature : ");
      Serial.print(temp);
      Serial.println(" C");
      Serial.print("Humidity : ");
      Serial.print(humi);
      Serial.println(" RH");
      Serial.print("CO2 : ");
      Serial.print(co2);
      Serial.println(" ppm");

      previousTime = millis();
    }
  }
}