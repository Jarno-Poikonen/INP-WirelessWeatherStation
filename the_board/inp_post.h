#include <Arduino.h>
#include <stddef.h>
#include <stdint.h>

bool inp_http_ask_measurement_interval(const char* host, const char* path, uint32_t station, uint32_t* interval);

bool inp_http_post_measurements(const char* host, const char* path, unsigned long station, float temperature, float humididy, float pressure, float illuminance);

bool inp_coap_post(const char* host, uint32_t station, float temperature, float humididy, float pressure, float illuminance);
