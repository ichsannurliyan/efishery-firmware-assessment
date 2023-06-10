#include <MQTTClientFunction.h>

PubSubClient mqtt;

MQTTClient::MQTTClient() {}

MQTTClient::~MQTTClient() {}

void MQTTClient::init()
{
    mqtt.setServer(mqttServer, mqttPort);
    // mqtt.setCallback(callback);
}

MQTTClientCallback::MQTTClientCallback() {}

MQTTClientCallback::~MQTTClientCallback() {}