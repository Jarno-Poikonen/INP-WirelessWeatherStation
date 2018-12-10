#include <Arduino.h>
#include <string.h>
#include "inp_power.h"
#include "at_util.h"
#include "inp_post.h"
#include "ldr.h"

void setup()
{
  inp_configure_power();
  Serial.begin(9600);
  for (int i = 4; i--;)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(128);
    digitalWrite(LED_BUILTIN, LOW);
    delay(128);
  }
}

#include <SoftwareSerial.h>

void loop()
{
  SoftwareSerial my_serial(10, 11); // RX, TX
  my_serial.begin(9600);

  my_serial.print("ask measurement interval\n");
  
  digitalWrite(LED_BUILTIN, HIGH);
  
  for (bool success = false; !success;)
  {
    int post_response = -1;
    size_t post_response_size;
    char* post_response_data;
    uint32_t interval = 0;
    success = inp_http_ask_measurement_interval("www.students.oamk.fi", "/~t5imtu00/weatherstations/index.php/Measurement/get_measurement_interval", 4, &interval);
    //success = inp_http_post_measurements("www.students.oamk.fi", "/~t7nysa00/inp616.php", 1, 1.0f, 2.0f, 3.0f, 4.0f);
    //success = inp_http_post_measurements("www.students.oamk.fi", "/~t5imtu00/weatherstations/index.php/Measurement/add_new_measurement", 4, 0.0, 0.0, 0.0, LDR_4V_215_2R((uint16_t)((((float)analogRead(A5) / 1023.0) * 4.0f) * 1000.0f)));
    my_serial.print(interval);
  }
  
  digitalWrite(LED_BUILTIN, LOW);

  
  
  for (;;)
    inp_sleep(60000);
}
