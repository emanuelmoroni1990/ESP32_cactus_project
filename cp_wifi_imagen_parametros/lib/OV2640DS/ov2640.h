/*
 * Librería creada para el control de la cámara OV2640 con el microcontrolador ESP32S
 * Emanuel Moroni - 2020
 */
#include <Arduino.h>
#include <WiFi.h>
#include <esp_camera.h>

#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

struct OV2640
{
    int psramFound;
    String error;
};

OV2640 inic_camera (void);
String send_photo (WiFiClient, String, String, int);