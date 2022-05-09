//In the Arduino IDE
// Use this Board: ESP32 Dev Module
//  Partition Scheme: Default 4MB with spiffs
//  Other defaults are OK
//Actual board used for testing: Clone of DOIT ESP32 DEVKIT V1
//
//Original Source of Example Code (Retrieved April 2022):
// https://microcontrollerslab.com/esp32-web-server-spi-flash-file-system-spiffs/
// https://github.com/me-no-dev/ESPAsyncWebServer
//
//Tutorial on how to load SPI Flash File System (SPIFFS)
// https://microcontrollerslab.com/install-esp32-filesystem-uploader-in-arduino-ide-spiffs/
// https://github.com/etherfi/arduino-esp32fs-plugin-esp32s2
//
// Some errors corrected from the example code:
//  The HTML header was fixed. I think it was a copy-paste error.
//  The function template_processor() needs to return a default value.
//  The SPIFFS filenames need to match lowercase/uppercase exactly.
//
// Changes:
//  Example code extended to show 3 buttons, to control 3 outputs. Intended to connect to an RGB LED.
//  LED colors help with WiFi connection diagnosis, instead of the LED defaulting to being off.
//  Remove anonymous function from server.on(), as I feel it is an inappropriate use that is confusing.
//  Print the local IP address to Serial every 10 seconds for later on Serial Monitor connections.
//  Add Arduino_secrets.h file.
//

// Import required libraries
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include "Arduino_secrets.h"

// Replace network credentials with your own WiFi details
const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;

const int Pin_GreenLed = 12;
const int Pin_RedLed = 13;
const int Pin_BlueLed = 14;

bool GPIO12_State = 1;  //Green LED will turn on when setup has completed okay.
bool GPIO13_State = 0;
bool GPIO14_State = 0;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

//unused, from original example code.
//void notifyClients() {
//  ws.textAll(String(GPIO_State));
//}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "toggle12") == 0) {
      GPIO12_State = !GPIO12_State;
      //notifyClients();
      ws.textAll(String("GPIO12=") + String(GPIO12_State));
    }
    if (strcmp((char*)data, "toggle13") == 0) {
      GPIO13_State = !GPIO13_State;
      ws.textAll(String("GPIO13=") + String(GPIO13_State));
    }
    if (strcmp((char*)data, "toggle14") == 0) {
      GPIO14_State = !GPIO14_State;
      ws.textAll(String("GPIO14=") + String(GPIO14_State));
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  //Handle WebSocket event
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

String template_processor(const String& var){
  //this is mainly so that the first static page load contains the current values
  Serial.println(var);
  
  if(var == "STATE12"){
    //encountered %STATE12% in the html file.
    if (GPIO12_State) {return "ON";} else {return "OFF";}
  }

  if(var == "STATE13"){
    if (GPIO13_State) {return "ON";} else {return "OFF";}
  }

  if(var == "STATE14"){
    if (GPIO14_State) {return "ON";} else {return "OFF";}
  }

  //by default just give back the unhandled template variable unaltered so that debugging is easier.
  return var;
}

void http_get_callback(AsyncWebServerRequest *request) {
  //filename case matters. lowercase 'p' in "page.html" will not work if named "Page.html".
  request->send(SPIFFS, "/Page.html", "text/html", false, template_processor);
}

void http_not_found(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void initWebSocketServer() {
  // attach AsyncWebSocket
  ws.onEvent(onEvent);
  server.addHandler(&ws);

  // callback route for root / web page
  //example: server.on(endpoint, HTTP_###, callback);   //GET, PUT, etc
  //
  //This wasted a day of my life: rewrite to avoid anonymous function declaration [](){}
  // If a normal callback fucntion can solve this, this is not a reason to be clever!
  //server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  //  request->send(SPIFFS, "/Page.html", "text/html", false, template_processor);
  //});
  server.on("/", HTTP_GET, http_get_callback);

  server.serveStatic("/", SPIFFS, "/");

  server.onNotFound(http_not_found);

  // Start server after attaching handlers.
  server.begin();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  if (!SPIFFS.begin()) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  else{
   Serial.println("SPIFFS mounted successfully");
  }

  pinMode(Pin_GreenLed, OUTPUT);
  pinMode(Pin_RedLed, OUTPUT);
  pinMode(Pin_BlueLed, OUTPUT);

  digitalWrite(Pin_BlueLed, HIGH);      //start with BLUE, this will only be seen briefly if at all.

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi..");
    digitalWrite(Pin_RedLed, HIGH);     //Turn on the red LED also to show MAGENTA, indicating retries.
    delay(1024);
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  initWebSocketServer();
}

// Do something periodically in the loop section
unsigned long previousMillis = 0;        // will store last update
unsigned long updateInterval = 10000;    // how fast to update (ms)

void loop() {
  ws.cleanupClients();
  digitalWrite(Pin_GreenLed, GPIO12_State);
  digitalWrite(Pin_RedLed, GPIO13_State);
  digitalWrite(Pin_BlueLed, GPIO14_State);

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= updateInterval) {
    Serial.println(WiFi.localIP());
    previousMillis = currentMillis;
  }
  
}
