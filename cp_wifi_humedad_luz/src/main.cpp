/*
 * 1 - https://www.arduino.cc/reference/en/language/functions/communication/serial/parseint/
 * 2 - https://www.luisllamas.es/como-programar-el-esp8266-por-wifi-con-arduino-ota/
 */

#include <Arduino.h>
#include <WiFi_connection.h>
#include <ESP8266HTTPClient.h>

#define HTTPPOST true
#define WIFI_SSID "losmoros437"
#define WIFI_PASSWORD "hechos1631"

#define TIMEOUT_WIFI 200

#define PIN_LED 1

void toogle_led (int, int);

String serverName = "http://192.168.100.16"; 
String uploadDataPath = "/post-esp8266-dat.php";

String apiKeyValue = "emoroni_utn_frba_2020";
const int serverPort = 80;

HTTPClient httpclient;

estado_conexion info_WiFi;

bool flag_h = false;
bool flag_l = false;
bool datos_completos = false;

int data_humedad, data_intluminica, data_seccion;
int i = 0, h = 0, l = 0;

unsigned char aux;
char hum [5];
char ilu [5];

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(9600);

  /*
   * Conexión a la red WiFi y chequeo de si efectivamente la conexión se realizó con éxito. emoroni.
   */
  info_WiFi = inic_conexion (WIFI_SSID, WIFI_PASSWORD, TIMEOUT_WIFI);

  /*
   * Impresiones por terminal serial para conocer el estado de la configuración. emoroni.
   */
  Serial.println("Información de configuración: ");
  Serial.println("WiFi: ");
  Serial.println("- Estado: " + info_WiFi.estado);
  Serial.println("- Dirección IP: " + info_WiFi.direccion_IP);
}

void loop() {
  // put your main code here, to run repeatedly:

  /*
   * Realizo la lectura del buffer de la UART y almaceno esos datos en dos variables nuevas.
   * Estas dos variables van a tener información sobre la humedad del suelo y la intensidad
   * lumínica que exista en un punto particular relevante a una planta en particular. emoroni.
   */

  if(Serial.available()) {

    aux = Serial.read();

    if(aux == 'H')
    {
      //Serial.println(aux);
      flag_h = true;
      flag_l = false;
      h = 0;
    }
    else if(aux == 'L')
    {
      //Serial.println(aux);
      flag_l = true;
      flag_h = false;
      l = 0;
    }
    else if(aux == 'F')
    {
      datos_completos = true;
    }
    else if(flag_h == true)
    {
      hum[h] = aux;
      //Serial.println(hum[h]);
      h++;
    }
    else if(flag_l == true)
    {
      ilu[l] = aux;
      //Serial.println(ilu[l]);
      l++;
    }
  }
  
  if (datos_completos == true)
  {
    data_humedad = atoi(hum);
    data_intluminica = atoi(ilu);
    data_seccion ++;

    Serial.print("Humedad: ");
    Serial.println(data_humedad);
    Serial.print("Intensidad lumínica: ");
    Serial.println(data_intluminica);

    #ifdef HTTPPOST
      httpclient.begin(serverName + uploadDataPath);
      httpclient.addHeader("Content-Type", "application/x-www-form-urlencoded");
      // Preparo la información que voy a pasar en mi solicitud HTTP.
      String httpRequestData = "api_key=" + apiKeyValue + "&humdsuelo=" + String(data_humedad)
      + "&intlum=" + String(data_intluminica) + "&idsec=" + String(data_seccion) + "";
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
    #endif

   datos_completos = false;

   if(data_seccion == 3)
   {
      data_seccion = 0;   
   }
  }
}

void toogle_led (int pin, int ms)
{
  digitalWrite (pin, HIGH);
  delay(ms);
  digitalWrite (pin, LOW);
  delay(ms);
}