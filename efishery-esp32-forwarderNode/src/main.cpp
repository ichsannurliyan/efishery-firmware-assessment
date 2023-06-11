#include <Arduino.h>
#include <WiFi.h>
#include <ENVClientFunction.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <asyncportal.h>
// #include <MQTTClientFunction.h>

String temperatureData, humidityData, co2Data;
boolean newTemperature, newHumidity, newCo2;
boolean doConnect = false;
boolean connected = false;

// const char * wifiSsid = "TP-Link_7CAA";
// const char * wifiPassword = "49577471";

const char * wifiSsid = "dlink-CCD0";
const char * wifiPassword = "tpbrn33498";

/* if defined Hive MQTT */
// const char * mqttServer = "1e274e07458745289062e09df40ddbc0.s2.eu.hivemq.cloud";
// int mqttPort = 8883;
// String mqttUser = "efisheryassessment";
// String mqttPassword = "Testmqtt12345";
// String mqttPubTopic = "/environment";
// String mqttSubTopic = "/deviceControl";
// String deviceId = "env00001";

const char * mqttServer = "tailor.cloudmqtt.com";
int mqttPort = 11933;
String mqttUser = "sobinwaj";
String mqttPassword = "32GHLBqgoufD";
String mqttPubTopic = "/environment";
String mqttSubTopic = "/deviceControl";
String deviceId = "env00001";

BLEAddress *pServerAddress;
WiFiClient esp32client;
PubSubClient client(esp32client);
ENVBLEClient envBLE;
AsyncPortal webServer;
Preferences preferences;

void callback(char* topic, byte* payload, unsigned int length);
void publish();
void reconnect();
void MQTTinit();
void wifiInit();
String pubMessageStruct();

void setup() 
{
 Serial.begin(115200);
 /* ====== Initializing SPIFFS ====== */
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  } else {
    Serial.println("mounting success");
  }
  wifiInit();
  MQTTinit();
  preferences.begin(PREFERENCE_NAME, false);
  preferences.putString(AUTH_KEY, "-");
  preferences.end();
  webServer.createLoginServer();
  webServer.createMainServer();
  webServer.launchWeb();
  envBLE.init();
}

void loop() {
  if (doConnect == true) {
    if (envBLE.connectToServer(*pServerAddress)) {
      Serial.println("We are now connected to the BLE Server.");
      envBLE.getDescriptor();
      connected = true;
    } else {
      Serial.println("We have failed to connect to the server; Restart your device to scan for nearby BLE server again.");
    }
    doConnect = false;
  }
  if (newTemperature && newHumidity && newCo2){
    // wifiInit();
    newTemperature = false;
    newHumidity = false;
    newCo2 = false;
    temperatureData = envBLE.getValue(TEMPERATURE_DATA);
    humidityData = envBLE.getValue(HUMIDITY_DATA);
    co2Data = envBLE.getValue(CO2_DATA);
    Serial.print("Temperature: ");
    Serial.print(temperatureData);
    Serial.println(" C");
    Serial.print("Humidity: ");
    Serial.print(humidityData);
    Serial.println(" %");
    Serial.print("CO2: ");
    Serial.print(co2Data);
    Serial.println(" ppm");
    publish();
  }
  delay(5000);
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
}

void wifiInit() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifiSsid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiSsid, wifiPassword);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void MQTTinit()
{
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  reconnect();
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String payloadString;
  for (int i=0;i<length;i++) {
    payloadString  += (char)payload[i];
    Serial.print((char)payload[i]);
  }
  Serial.println();

  StaticJsonDocument<96> doc;

  DeserializationError error = deserializeJson(doc, payloadString);

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  const char* eventName = doc["eventName"];

  if (eventName == "deviceReboot")
  {
    int reboot = doc["reboot"];
    if (reboot == 1)
    {
      delay(5000);
      ESP.restart();
    }
  }
}

void reconnect() 
{
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-";
    clientId += deviceId;
    if (client.connect(clientId.c_str(),mqttUser.c_str(),mqttPassword.c_str())) {
      Serial.println("connected");
      client.subscribe(mqttSubTopic.c_str());
    } else {
      wifiInit();
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

String pubMessageStruct()
{
  String message = "{\"eventName\":\"deviceStatus\",\"temperature\":\"";
  message += temperatureData; 
  message += "\",\"humidity\":\"";
  message += humidityData; 
  message += "\",\"co2\":\"";
  message += co2Data; 
  message += "\"}";

  return message;
}

void publish()
{
  String publishMessage = pubMessageStruct();
  if (client.publish(mqttPubTopic.c_str(), publishMessage.c_str())){
    Serial.println("Message successfully published");
  };
}