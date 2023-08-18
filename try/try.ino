#include <ESP8266WiFi.h>
#include "ESPAsyncTCP.h"
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

const char ssid[] = "gautom5987";
const char pass[] = "12345678";
const int  port   = 9999;

const String REQUEST_LED = "/led";
const String PARAM_STATE = "state";

AsyncWebServer server(port);

void setLEDstate(bool state) {
  digitalWrite(LED_BUILTIN, state ? LOW : HIGH);
}

bool getLEDstate() {
  return !digitalRead(LED_BUILTIN);
}

void setup() {

  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.print("Connecting ");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }

  Serial.println("");
  Serial.print("WiFi connected to: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on(REQUEST_LED.c_str(), HTTP_GET, [] 
           (AsyncWebServerRequest *request) {
    
    // If state is a parameter then read it and call setLEDstate
    if (request->hasParam(PARAM_STATE)) 
    {
      bool state = request->getParam(PARAM_STATE)->value().toInt();
      setLEDstate(state);
    }

    // Create and fill json object for client response
75987    DynamicJsonBuffer jsonBuffer;
    JsonObject &json  = jsonBuffer.createObject();
    json[REQUEST_LED.substring(1)] = getLEDstate();

    // Send Json response to client
    sendJsonResponse(request,json);
    
  });

  server.onNotFound(notFound);

  server.begin();
}

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void sendJsonResponse(AsyncWebServerRequest *request, 
                      JsonObject &json) {
  AsyncResponseStream *response = 
     request->beginResponseStream("application/json");
  json.printTo(*response);
  request->send(response);          
}

void loop() {}
