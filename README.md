# ESP32_WebSocket_SPIFFS_Server_Example
- From mutiple web browsers control and see status of an RGB LED.
![Screenshot_20220612-124102_Firefox](https://user-images.githubusercontent.com/96515734/178652673-4ae9d7ca-ac53-4315-9b2b-b15f8aff5c1f.jpg)
![20220612_123334](https://user-images.githubusercontent.com/96515734/178651975-1db44275-ab05-49a7-82d5-4d3af195ac1c.jpg)

# Original Source of Example Code (Retrieved April 2022):
1. https://microcontrollerslab.com/esp32-web-server-spi-flash-file-system-spiffs/
2. https://github.com/me-no-dev/ESPAsyncWebServer

# Changes:
1. Example code extended to show 3 buttons, to control 3 outputs. Intended to connect to an RGB LED.
2. LED colors help with WiFi connection diagnosis, instead of the LED defaulting to being off.
3. Remove anonymous function from server.on(), as I feel it is an inappropriate use that is confusing.
4. Print the local IP address to Serial every 10 seconds for later on Serial Monitor connections.
5. Add Arduino_secrets.h file.

# Instructions:
1. Use this Board: ESP32 Dev Module
 - Partition Scheme: Default 4MB with spiffs
 - Other defaults are OK
2. Actual board used for testing: Clone of DOIT ESP32 DEVKIT V1

# Tutorial on how to load SPI Flash File System (SPIFFS):
1. https://microcontrollerslab.com/install-esp32-filesystem-uploader-in-arduino-ide-spiffs/
2. https://github.com/etherfi/arduino-esp32fs-plugin-esp32s2
 
# Some errors corrected from the example code:
1. The HTML header was fixed. I think it was a copy-paste error.
2. The function template_processor() needs to return a default value.
3. The SPIFFS filenames need to match lowercase/uppercase exactly.
