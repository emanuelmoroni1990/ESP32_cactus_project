/*
 * GET - https://www.php.net/manual/es/reserved.variables.get.php. emoroni.
 */

#include <ov2640.h>

OV2640 inic_camera (void)
{
    camera_config_t config;
    OV2640 info_ov2640;

    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;

    info_ov2640.psramFound = psramFound();

    // Estoy trabajando en modos de sacar la foto en una calidad inferior a la máxima que podría
    // sacar con esta cámara. emoroni.
    // init with high specs to pre-allocate larger buffers
    if(psramFound()){
        config.frame_size = FRAMESIZE_UXGA; //;FRAMESIZE_SVGA FRAMESIZE_UXGA
        config.jpeg_quality = 10;  //0-63 lower number means higher quality
        config.fb_count = 2;
    } else {
        config.frame_size = FRAMESIZE_CIF;
        config.jpeg_quality = 12;  //0-63 lower number means higher quality
        config.fb_count = 1;
    }

    // camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        info_ov2640.error = "Camera init failed with error 0x" + err;
        ESP.restart();
    }

    return info_ov2640;
}

String send_photo (WiFiClient client, String serverNamePhoto, String uploadPhotoPath, int serverPort) 
{
  String getAll;
  String getBody;
  String fileName;

  int bytes_send;
  float porcentaje_carga = 0;
  size_t contador_carga = 0;

  camera_fb_t * fb = NULL;
  /* Empleando la función esp_camera_fb_get obtengo un puntero al comienzo de la trama almacenada
     en el buffer de la cámara que contiene la imagen sacada en ese momento e información sobre ella. 
     Es un puntero del tipo camera_fb_t. emoroni */
  fb = esp_camera_fb_get(); 
  if(!fb) {
    Serial.println("Camera capture failed");
    delay(1000);
    ESP.restart();
  }
  
  Serial.println("Connecting to server: " + serverNamePhoto);

  /*
   * Cuando empleo una calidad en las imagenes igual a FRAMESIZE_SVGA, imagenLen y fbLen coinciden,
   * pero no ocurre lo mismo cuando utilizo la calidad FRAMESIZE_UXGA. En este último caso imagenLen
   * es mucho menor. ¿Es por qué no tiene tiempo suficiente para actulizar el buffer? emoroni.
   * No, era un problema con el tipo de variable que usaba en un caso y en otro. emoroni.
   */

  if (client.connect(serverNamePhoto.c_str(), serverPort)) {
    Serial.println("Connection successful!");    
    String head = "--RandomNerdTutorials\r\nContent-Disposition: form-data; name=\"imageFile\"; filename=\"esp32-cam.jpg\"\r\nContent-Type: image/jpeg\r\n\r\n";
    String tail = "\r\n--RandomNerdTutorials--\r\n";

    size_t imageLen = fb->len; // Almaceno el largo del buffer en bytes. emoroni.
    Serial.println("imageLen: " + String(imageLen));
    size_t extraLen = head.length() + tail.length();
    size_t totalLen = imageLen + extraLen;
  
    client.println("POST " + uploadPhotoPath + " HTTP/1.1");
    client.println("Host: " + serverNamePhoto);
    client.println("Content-Length: " + String(totalLen));
    Serial.println("TotalLen: " + String(totalLen));
    client.println("Content-Type: multipart/form-data; boundary=RandomNerdTutorials");
    client.println();
    client.print(head);
  
    uint8_t *fbBuf = fb->buf; // Este buffer es el que contiene la imagen. emoroni.
    size_t fbLen = fb->len; // Almaceno el largo del buffer en bytes nuevamente, ¿por qué?. emoroni.
    Serial.println(fbLen);

    /* En este for voy a escribir en mi servidor de a 1024 bytes de imagen. Cuando el resto de la 
       imagen que quede por subir sea menor a 1024 bytes, subo solo lo que falta. emoroni */
    for (size_t n=0; n<fbLen; n=n+1024) {
      if (n+1024 < fbLen) {
        bytes_send = client.write(fbBuf, 1024);
        //Descomentar en caso de ser necesario ver la cantidad de bytes cargados. emoroni.
        //Serial.println(bytes_send); 
        fbBuf += 1024;
        contador_carga += 1024;
        porcentaje_carga = (contador_carga * 100) / fbLen;
        Serial.println("Porcentaje de carga: " + String(porcentaje_carga) + "%");
      }
      else if (fbLen%1024>0) {
        size_t remainder = fbLen%1024;
        bytes_send = client.write(fbBuf, remainder);
        //Descomentar en caso de ser necesario ver la cantidad de bytes cargados. emoroni.
        //Serial.println(bytes_send);
        Serial.println("Carga Finalizada");
      }
    }   
    client.print(tail);
    
    esp_camera_fb_return(fb);

  }
  String prueba = "prueba";
  return prueba;
}