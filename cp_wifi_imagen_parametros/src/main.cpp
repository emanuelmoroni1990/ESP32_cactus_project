#include <Arduino.h>
//#include <WiFi.h>
//#include <Wire.h>
#include <HTTPClient.h>
//#include <SparkFunBME280.h>
/*
#include <soc/soc.h>
#include <soc/rtc_cntl_reg.h>*/

#include <WiFi_connection.h>
#include <ov2640.h>
#include <bme280.h>

#define WIFI_SSID "losmoros437"
#define WIFI_PASSWORD "hechos1631"

#define LED 33
#define BLINK_TIME 1000

#define I2C_SDA 14
#define I2C_SCL 15

#define CORRECCION_TEMP 0
#define TIMEOUT_WIFI 20

#define ESP32_NUMBER_MACETA_ID "ESP32S_1" // Este valor más adelante lo puedo modificar desde una app o
                                          // por hard desde un dipswitch. emoroni.

String serverName = "http://192.168.100.16"; 
String uploadDataPath = "/post-esp-dat.php";

String serverNamePhoto = "192.168.100.16";
String uploadPhotoPath = "/upload.php";     // The default uploadDataPath should be upload.php

String apiKeyValue = "emoroni_utn_frba_2020";
const int serverPort = 80;

estado_conexion info_WiFi;
OV2640 info_camera;
uint8_t info_bme;
valores_medidos valores_sensados;

WiFiClient client;
HTTPClient httpclient;

void setup() {

  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  
  /*
   * Conexión a la red WiFi y chequeo de si efectivamente la conexión se realizó con éxito. emoroni.
   */
  info_WiFi = inic_conexion (WIFI_SSID, WIFI_PASSWORD, TIMEOUT_WIFI);

  /*
   * Inicializaciones en la configuraciones de la cámara y el sensor BME280. emoroni.
   */
  info_camera = inic_camera();
  info_bme = inic_bme280_I2C_mode(/*bme,*/ I2C_SDA, I2C_SCL);

  /*
   * Impresiones por terminal serial para conocer el estado de la configuración. emoroni.
   */
  Serial.println("Información de configuración: ");
  Serial.println("WiFi: ");
  Serial.println("- Estado: " + info_WiFi.estado);
  Serial.println("- Dirección IP: " + info_WiFi.direccion_IP);
  Serial.println("Camera: ");
  Serial.println("- psramFound: " + String(info_camera.psramFound)); 
  Serial.println("- ¿Error?: " + info_camera.error);
  Serial.println("BME280: ");
  Serial.println("- Chip_ID: " + String(info_bme));
}

void loop() {

  digitalWrite(LED, LOW);
  delay(BLINK_TIME);
  digitalWrite(LED, HIGH);
  delay(BLINK_TIME);

  Serial.println(mostrar_lecturas(/*bme*/));

  httpclient.begin(serverName + uploadDataPath);
  httpclient.addHeader("Content-Type", "application/x-www-form-urlencoded");
  // Preparo la información que voy a pasar en mi solicitud HTTP.
  valores_sensados = tomar_valores();
  String httpRequestData = "api_key=" + apiKeyValue + "&temp=" + String(valores_sensados.tempC)
  + "&humd=" + String(valores_sensados.humedad) + "&pres=" + String(valores_sensados.presion/100.0F) + 
  "&espId=" + ESP32_NUMBER_MACETA_ID + "";
  Serial.print("httpRequestData: ");
  Serial.println(httpRequestData);
  // Envío al servidor la solicitud HTTP POST. emoroni.
  int httpResponseCode = httpclient.POST(httpRequestData);

  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Libero los recursos. emoroni.
  httpclient.end();

  send_photo(client, serverNamePhoto, uploadPhotoPath, serverPort);

  delay(1000*60-(BLINK_TIME*2));
}
