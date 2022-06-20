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

const int Pin_GreenLED = 12;
const int Pin_RedLED = 13;
const int Pin_BlueLED = 14;

const char* PinText_GreenLED = "12";
const char* PinText_RedLED = "13";
const char* PinText_BlueLED = "14";

bool State_GreenLED = 1;  //Green LED will turn on when setup has completed okay.
bool State_RedLED = 0;
bool State_BlueLED = 0;

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
    if (strcmp((char*)data, "toggle_green") == 0) {
      State_GreenLED = !State_GreenLED;
      //notifyClients();
      ws.textAll(String("led_green=") + String(State_GreenLED));
    }
    else if (strcmp((char*)data, "toggle_red") == 0) {
      State_RedLED = !State_RedLED;
      ws.textAll(String("led_red=") + String(State_RedLED));
    }
    else if (strcmp((char*)data, "toggle_blue") == 0) {
      State_BlueLED = !State_BlueLED;
      ws.textAll(String("led_blue=") + String(State_BlueLED));
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
  
  if(var == "STATE_GREEN_LED"){
    //encountered %STATE_GREEN_LED% in the html file.
    if (State_GreenLED) {return "ON";} else {return "OFF";}
  }
  
  else if(var == "STATE_RED_LED"){
    if (State_RedLED) {return "ON";} else {return "OFF";}
  }
  
  else if(var == "STATE_BLUE_LED"){
    if (State_BlueLED) {return "ON";} else {return "OFF";}
  }
  
  else if(var == "PIN_GREEN_LED"){
    return PinText_GreenLED;
  }
  
  else if(var == "PIN_RED_LED"){
    return PinText_RedLED;
  }
  
  else if(var == "PIN_BLUE_LED"){
    return PinText_BlueLED;
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
  //Rewrite to avoid anonymous function declaration [](){}
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

  pinMode(Pin_GreenLED, OUTPUT);
  pinMode(Pin_RedLED, OUTPUT);
  pinMode(Pin_BlueLED, OUTPUT);

  digitalWrite(Pin_BlueLED, HIGH);      //start with BLUE, this will only be seen briefly if at all.

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi..");
    digitalWrite(Pin_RedLED, HIGH);     //Turn on the red LED also to show MAGENTA, indicating retries.
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
  digitalWrite(Pin_GreenLED, State_GreenLED);
  digitalWrite(Pin_RedLED, State_RedLED);
  digitalWrite(Pin_BlueLED, State_BlueLED);

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= updateInterval) {
    Serial.println(WiFi.localIP());
    previousMillis = currentMillis;
  }
  
}
