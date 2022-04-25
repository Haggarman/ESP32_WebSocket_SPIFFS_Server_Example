# ESP32_WebSocket_SPIFFS_Server_Example
 From mutiple web browsers control and see status of an RGB LED.

# Original Source of Example Code (Retrieved April 2022):
 https://microcontrollerslab.com/esp32-web-server-spi-flash-file-system-spiffs/
 https://github.com/me-no-dev/ESPAsyncWebServer

# Changes:
 Example code extended to show 3 buttons, to control 3 outputs. Intended to connect to an RGB LED.
 LED colors help with WiFi connection diagnosis, instead of the LED defaulting to being off.
 Remove anonymous function from server.on(), as I feel it is an inappropriate use that is confusing.
 Print the local IP address to Serial every 10 seconds for later on Serial Monitor connections.
 Add Arduino_secrets.h file.

# Instructions:
 Use this Board: ESP32 Dev Module
  Partition Scheme: Default 4MB with spiffs
  Other defaults are OK
 Actual board used for testing: Clone of DOIT ESP32 DEVKIT V1

# Tutorial on how to load SPI Flash File System (SPIFFS):
 https://microcontrollerslab.com/install-esp32-filesystem-uploader-in-arduino-ide-spiffs/
 https://github.com/etherfi/arduino-esp32fs-plugin-esp32s2
 
#Some errors corrected from the example code:
 The HTML header was fixed. I think it was a copy-paste error.
 The function template_processor() needs to return a default value.
 The SPIFFS filenames need to match lowercase/uppercase exactly.
