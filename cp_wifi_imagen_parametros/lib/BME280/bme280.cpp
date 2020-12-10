#include <bme280.h>

BME280 bme;

uint8_t inic_bme280_I2C_mode (/*BME280 bme, */int I2C_SDA, int I2C_SCL)
{
  Wire.begin(I2C_SDA, I2C_SCL);

  bme.settings.commInterface = I2C_MODE;
  bme.settings.I2CAddress = 0x76;
  bme.settings.runMode = 3;
  bme.settings.tStandby = 0;
  bme.settings.filter = 0;
  bme.settings.tempOverSample = 1;
  bme.settings.pressOverSample = 1;
  bme.settings.humidOverSample = 1;

  return(bme.begin());
}

String mostrar_lecturas (/*BME280 bme*/){
  //bme.setTemperatureCorrection(CORRECCION_TEMP);

  float temperature, humidity, presion;
  temperature = bme.readTempC();
  humidity = bme.readFloatHumidity();
  presion = bme.readFloatPressure();
  String message = "Temperature: " + String(temperature) + " ºC \n";
  message += "Humidity: " + String (humidity) + " % \n";
  message += "Presión: " + String (presion/100) + " hPa \n";

  return message;
}

valores_medidos tomar_valores ()
{
    valores_medidos valores;

    valores.tempC = bme.readTempC();
    valores.humedad = bme.readFloatHumidity();
    valores.presion = bme.readFloatPressure();

    return(valores);
}