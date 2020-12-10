/*
 * Librería creada para el control de sensor BME280 con el microcontrolador ESP32S
 * Emanuel Moroni - 2020
 */

#include <Arduino.h>
#include <Wire.h>
#include <SparkFunBME280.h>

struct valores_medidos{
    float tempC;
    float humedad;
    float presion;
};

uint8_t inic_bme280_I2C_mode (int, int);
String mostrar_lecturas ();
valores_medidos tomar_valores ();