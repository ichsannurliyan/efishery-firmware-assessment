#include <asyncportal.h>

AsyncWebServer server(80);

String auth;

String printRandomString(int n);
bool deviceIdValidation(String deviceId);
void deleteAuth();

hw_timer_t *loginSession = NULL;

AsyncPortal::AsyncPortal()
{

}

AsyncPortal::~AsyncPortal()
{

}

String processor(const String& var)
{
    String temperature, humidity, co2, upTime, fwVersion, hwVersion, serialNum, connectionMode, connection,connectionStatus, address, Gateway, DNS;

    if (var == "TEMPERATURE"){
      temperature = temperatureData;
      return temperature;
    }
    else if (var == "HUMIDITY"){
      humidity = humidityData;
      return humidity;
    }
    else if (var == "CO2"){
      co2 = co2Data;
      return co2;
    }

    else if (var == "CONNECTION_MODE"){
      connectionMode = "WiFi";
      return connectionMode;
    }

    else if (var == "CONNECTION"){
      connection = "-";
      return connection;
    }

    else if (var == "CONNECTION_STATUS"){
      connectionStatus = "Connected";
      return connectionStatus;
    }
    else if (var == "IPADDRESS"){
      address = WiFi.localIP().toString();
      return address;
    }
    else if (var == "GATEWAY"){
        Gateway = WiFi.gatewayIP().toString();
        // Gateway = "192.168.101.1";
        return String(Gateway);
    }
    else if (var == "SYSTEM_UPTIME"){
      // upTime = convertTime();
      upTime = "00:00:00";
        return upTime;
    }
    else if (var == "DNS"){
        return DNS;
    }
    return String();
}

void AsyncPortal::createLoginServer(){
    server.on("/", HTTP_GET, [&](AsyncWebServerRequest *request){
      preferences.begin(PREFERENCE_NAME, false);
      if (preferences.getString(AUTH_KEY).length() == 0){
        auth = printRandomString(14);
      }
      else if (auth != preferences.getString(AUTH_KEY)){
        auth = printRandomString(14);
      } else {
        preferences.end();
        request->redirect("/deviceStatus");
        
      }

      preferences.end();
      // request->requestAuthentication();
      request->send(SPIFFS, "/index.html", String(), false);
    });

    server.on("/index.html/loginHandler",HTTP_GET, [&](AsyncWebServerRequest *request){
      // AsyncWebHandler handler;
      // handler.setAuthentication();
      String userParam, userValue;
      String passParam, passValue;

      if(request->hasParam("username") && request->hasParam("password")){
        String authSession;
        userValue = request->getParam("username")->value();
        passValue = request->getParam("password")->value();
        if (userValue.length() > 0 && passValue.length() > 0){
          preferences.begin(PREFERENCE_NAME, false);
          authSession = preferences.getString(AUTH_KEY);
          preferences.end();
          if (userValue == http_username && passValue == http_password && authSession != auth){
            preferences.begin(PREFERENCE_NAME, false);
            preferences.putString(AUTH_KEY, auth);
            preferences.end();
            loginSession = timerBegin(3,80,true);
            timerAttachInterrupt(loginSession, &deleteAuth, true);
            timerAlarmWrite(loginSession, 60000 * 1000, false);
            timerAlarmEnable(loginSession);
            request->redirect("/deviceStatus");
          } else if(userValue == http_username && passValue == http_password && authSession == auth) {
            request->send(404, "text/plain", "Someone is login with this account, try again later");
          } else {
            request->send(404, "text/plain", "username or password incorrect");
          }
        }
      }

    });
}

void AsyncPortal::createMainServer()
{
    server.on("/deviceStatus", HTTP_GET, [&](AsyncWebServerRequest *request){
      preferences.begin(PREFERENCE_NAME, false);
        if (auth != preferences.getString(AUTH_KEY)){
          preferences.end();
          request->send(401, "text/plain", "Please log in to continue");
          request->redirect("/");
        } else {
          preferences.end();
          request->send(SPIFFS, "/status.html", String(), false, processor);
        }
    });

    // Route to load style.css file
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/style.css", "text/css");
    });

    server.on("/deviceCommand", HTTP_GET, [&](AsyncWebServerRequest *request){
      preferences.begin(PREFERENCE_NAME, false);
        if (auth != preferences.getString(AUTH_KEY)){
          preferences.end();
          request->send(401, "text/plain", "Please log in to continue");
          request->redirect("/");
        } else {
          preferences.end();

          request->send(SPIFFS, "/setting.html", String(), false);
        }  
    });

    server.on("/voiceCall", HTTP_GET, [&](AsyncWebServerRequest *request){
      preferences.begin(PREFERENCE_NAME, false);
        if (auth != preferences.getString(AUTH_KEY)){
          preferences.end();
          request->send(401, "text/plain", "Please log in to continue");
          request->redirect("/");
        } else {
          preferences.end();
          String voiceCheck, voiceParam;
          if (request->hasParam("Voice")){
            voiceCheck = request->getParam("Voice")->value();
            voiceParam = "Voice";
            Serial.println(voiceCheck);
            if (voiceCheck == "1"){
            
            } else {
                // modem.sendAT(GF("+GSMBUSY=1"));
                // String ros = modem.stream.readStringUntil('"');
                // Serial.println(ros);
            }
          }
          request->send(SPIFFS, "/setting.html", String(), false);
        }
    });

    server.on("/setting.html/reboot", HTTP_GET, [&](AsyncWebServerRequest *request){
      preferences.begin(PREFERENCE_NAME, false);
        if (auth != preferences.getString(AUTH_KEY)){
          preferences.end();
          request->send(401, "text/plain", "Please log in to continue");
          request->redirect("/");
        } else {
          preferences.end();
          delay(5000);
          ESP.restart();
          // request->send(SPIFFS, "/deviceStatus", String(), false);
          request->send(200, "text/plain", "ESP32 will restart in 5 second");
        }
    });

    server.on("/efishery", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/efishery.png", "image/png");
    });
}

void AsyncPortal::launchWeb()
{
    server.begin();
    Serial.println("HTTP Server started");
    delay(100);
}

String printRandomString(int n){
  char alphabet[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
  char alphabetBig[26] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
  char number[10] = {'0','1','2','3','4','5','6','7','8','9'};

  String res = "";

  for (int i = 0; i < n; i++){
    res = res + alphabet[rand() % 26];
    res += alphabetBig[rand() % 26];
    res += number[rand() % 10];
    res += alphabetBig[rand() % 26];
    res += number[rand() % 10];
    res += number[rand() % 10];
    res += alphabet[rand() % 26];
    res += number[rand() % 10];
    res += alphabet[rand() % 26];
    res += alphabetBig[rand() % 26];
    res += alphabet[rand() % 26];
  }

  return res;
}

void deleteAuth(){
  preferences.begin(PREFERENCE_NAME, false);
  preferences.putString(AUTH_KEY, "");
  preferences.end();
  // timerEnd(loginSession);
}