#include <WiFi_connection.h>

estado_conexion inic_conexion (const char* ssid, const char* password, int timeout)
{
    estado_conexion WiFi_STATE;
    int contador_segundos = 0;
    bool flag_while = 0;

    WiFi.begin(ssid, password);

    /*
     * Permanezco en un while hasta alcanzar la condición de estar conectado a la red o que se cumpla el timeout
     */
    while((WiFi.status() != WL_CONNECTED) && (flag_while == 0)){
        delay(100);
        Serial.println(".");
        contador_segundos ++;
        if (contador_segundos == timeout){flag_while = 1;} // Coloco esta condición como timeout para que en caso de que no se pueda conectar pueda salir del lazo del while. emoroni.
    }

    /*
     * A este if ingresaré solo si ya está conectado a la red wifi
     */
    if(WiFi.status() == WL_CONNECTED){
        WiFi_STATE.estado = WiFi.status();
        WiFi_STATE.direccion_IP = WiFi.localIP().toString();
    }

    return (WiFi_STATE);
}

/*
void chequeo_conexion (void)
{
  while((WiFi.status() != WL_CONNECTED) && (flag_while == 0)){
    delay(100);
    Serial.println(".");
    contador_segundos ++;
    if (contador_segundos == 60){flag_while = 1;} // Coloco esta condición como timeout = 2s, para que en caso de que no se pueda conectar pueda salir del lazo del while.
  }
  if(WiFi.status() == WL_CONNECTED){
    Serial.print("ESTADO: ");
    Serial.println(WiFi.status());
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
}*/