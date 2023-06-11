#ifndef MQTT_CLIENT_FUNCTION_H
#define MQTT_CLIENT_FUNCTION_H

#include <main.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char * mqttServer = "";
int mqttPort;
String mqttMac = "";
String mqttPubTopic = "";
String mqttSubTopic = "";
String mqttUser = "";
String mqttPassword = "";
String deviceId = "";

class MQTTClient 
{
    private:

    public:
    MQTTClient();
    ~MQTTClient();

    void init();
    void connect();
};

class MQTTClientCallback
{
    public:
    MQTTClientCallback();
    ~MQTTClientCallback();

    void callback(char * topic, char * payload, unsigned int length);
};

#endif