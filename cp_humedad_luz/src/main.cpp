/*
 * 1 - https://digistump.com/wiki/digispark/tutorials/basics
 */

#include <Arduino.h>
#include <usi.h>

#define SEN_HUM 1
#define SEN_ILU 2
#define TIEMPO_SENSADO 60000

void toogle_led (int, int);

int humedad;
int int_luminica;
String lectura_ADC;

void setup() {
  // put your setup code here, to run once:
    pinMode(0, OUTPUT);
}

void loop() {
    // put your main code here, to run repeatedly:
    /*
     * Emanuel Moroni
     */
        
    humedad      = analogRead(SEN_HUM);
    int_luminica = analogRead(SEN_ILU);    

    lectura_ADC = "H" + String(humedad) + "L" + String(int_luminica) + "F";

    int len_2 = lectura_ADC.length();
    for (uint8_t i = 0; i<len_2; i++)
    {
        while (!usiserial_send_available())
        {
            // Wait for last send to complete
        }
        usiserial_send_byte(lectura_ADC[i]);
    }
    digitalWrite(0, LOW);
    delay(TIEMPO_SENSADO);
    digitalWrite(0, HIGH);
}