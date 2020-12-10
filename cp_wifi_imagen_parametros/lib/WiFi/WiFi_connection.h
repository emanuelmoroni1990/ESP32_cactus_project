/*
 * Librería creada para el control de la conexión a una red WiFi con el microcontrolador ESP32S
 * Emanuel Moroni - 2020
 */

#include <Arduino.h>
#include <WiFi.h>

struct estado_conexion{

    String estado;
    String direccion_IP;

};

estado_conexion inic_conexion (const char *, const char *, int);