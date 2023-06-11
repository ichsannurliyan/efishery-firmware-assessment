#ifndef ENV_DUMMY_H
#define ENV_DUMMY_H

#include <main.h>

class ENVDummy
{
    private:
    float temperature, humidity, co2;

    public:
    ENVDummy();
    ~ENVDummy();

    float readTemperature();
    float readHumidity();
    float readCO2();
};

#endif