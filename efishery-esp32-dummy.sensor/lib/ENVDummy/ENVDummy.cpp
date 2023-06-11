#include <ENVDummy.h>

ENVDummy::ENVDummy() {}

ENVDummy::~ENVDummy() {}

float ENVDummy::readTemperature()
{
    temperature = random(19, 37);
    return temperature;
}

float ENVDummy::readHumidity()
{
    humidity = random(30, 60);
    return humidity;
}

float ENVDummy::readCO2()
{
    co2 = random(300, 900);
    return co2;
}