#include <Arduino.h>
#include <string.h>
#include "inp_power.h"
#include "at_util.h"
#include "inp_post.h"
#include "SAP.h"
#define MS_TIME_MIMUTE 60000
#define MS_TIME_3_HOURS 10800000

#define LDR_VOLTAGE_OUTPUT_PIN A0      // !!! NOT TO BE USED IN THE FINAL INTEGRATION !!!
#define PWM_4_006V 967                // !!! NOT TO BE USED IN THE FINAL INTEGRATION !!!
#define LDR_ADC_RESOLUTION 1024       // !!! NOT TO BE USED IN THE FINAL INTEGRATION !!!
#define PWM_4_006V_AS_MILLIVOLTS 4006.0f // !!! NOT TO BE USED IN THE FINAL INTEGRATION !!!
static SAP sap;

#define station_id 4
static uint32_t sleep_time;

void setup()
{
  inp_configure_power();
  pinMode(LDR_VOLTAGE_OUTPUT_PIN, INPUT);
  //sap.begin(0, 102350);
  Serial.begin(9600);
  sleep_time = MS_TIME_3_HOURS;
  for (int i = 4; i--;)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(128);
    digitalWrite(LED_BUILTIN, LOW);
    delay(128);
  }
}

void loop()
{
  // calculate and return temperature as Celsius
  //float temperature = sap.getTemperature();
  float temperature = 1.0f;

  // calculate and return humiduty as percentage
  //float humidity = sap.getHumidity();
  float humidity = 2.0f;

  // calculate and return pressure as hectoPascals
  //float pressure = sap.getPressure();
  float pressure = 3.0f;

  // calculate and return the illuminance as lux
  //float illuminance = sap.getIlluminance((float)analogRead(LDR_VOLTAGE_OUTPUT_PIN) / LDR_ADC_RESOLUTION * PWM_4_006V_AS_MILLIVOLTS);
  float illuminance = 4.0f;

  //digitalWrite(LED_BUILTIN, HIGH);
  
  for (int try_count = 8;;)
  {
    if (inp_http_post_measurements("www.students.oamk.fi", "/~t5imtu00/weatherstations/index.php/Measurement/add_new_measurement", station_id, temperature, humidity, pressure, illuminance))
      break;
    else
      --try_count;
  }
  
  //digitalWrite(LED_BUILTIN, LOW);

  for (int try_count = 8;;)
  {
     uint32_t interval;
     if (inp_http_ask_measurement_interval("www.students.oamk.fi", "/~t5imtu00/weatherstations/index.php/Measurement/get_measurement_interval", station_id, &interval))
     {
      sleep_time = interval * MS_TIME_MIMUTE;
      break;
     }
     else
       --try_count;
  }

  inp_sleep(sleep_time);
}
