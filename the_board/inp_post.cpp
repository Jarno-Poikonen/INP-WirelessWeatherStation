#include <Arduino.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "at_util.h"
#include "inp_power.h"

static size_t decimal_encoding_length(size_t n)
{
	size_t length = 0;
	do
	{
		++length;
		n /= (size_t)10;
	} while (n);
	return length;
}

static size_t print_float(float value, size_t fraction_part_maximum_length, char* buffer)
{
	char* write = buffer;
	if (value < 0.0f)
	{
		if (value < -999999999.0f)
		{
			for (char* copy_source = (char*)"<-999999999.0", *copy_source_end = copy_source + 13, *copy_destination = buffer; copy_source != copy_source_end; ++copy_source, ++copy_destination)
				*copy_destination = *copy_source;
			return 13;
		}
		value = -value;
		*write++ = L'-';
	}
	else
	{
		if (value > 999999999.0f)
		{
			for (char* copy_source = (char*)">999999999.0", *copy_source_end = copy_source + 12, *copy_destination = buffer; copy_source != copy_source_end; ++copy_source, ++copy_destination)
				*copy_destination = *copy_source;
			return 12;
		}
	}
	uint32_t wholePart = 1;
	for (uint32_t powerOf10 = 10; wholePart != 9 && (uint32_t)(value / (float)powerOf10); powerOf10 *= 10)
		++wholePart;
	uint32_t fractionPart = 9 - wholePart;
	uint32_t decimalDigitMovingMultiplier = 1;
	for (uint32_t i = 0; i != fractionPart; ++i)
		decimalDigitMovingMultiplier *= 10;
	uint32_t integer = (uint32_t)(value * (float)decimalDigitMovingMultiplier);
	for (uint32_t i = wholePart - 1; i--;)
		decimalDigitMovingMultiplier *= 10;
	for (uint32_t i = wholePart; i--; decimalDigitMovingMultiplier /= 10)
		*write++ = '0' + (char)((integer / decimalDigitMovingMultiplier) % 10);
	*write++ = '.';
	if (fractionPart)
	{
		uint32_t irrelevantZeroes = 0;
		for (uint32_t n = fraction_part_maximum_length && fraction_part_maximum_length < (size_t)fractionPart ? (uint32_t)fraction_part_maximum_length : fractionPart, i = n; i--; decimalDigitMovingMultiplier /= 10)
		{
			char newDigit = '0' + (char)((integer / decimalDigitMovingMultiplier) % 10);
			if (newDigit != '0' || i + 1 == n)
				irrelevantZeroes = 0;
			else
				++irrelevantZeroes;
			*write++ = newDigit;
		}
		return ((size_t)((uintptr_t)write - (uintptr_t)buffer) / sizeof(char)) - (size_t)irrelevantZeroes;
	}
	else
	{
		*write++ = '0';
		return (size_t)((uintptr_t)write - (uintptr_t)buffer) / sizeof(char);
	}
}

//#include <SoftwareSerial.h>
//SoftwareSerial my_serial(10, 11); // RX, TX

bool inp_http_ask_measurement_interval(const char* host, const char* path, uint32_t station, uint32_t* interval)
{
  digitalWrite(GPRS_RESET_PIN, HIGH);
  
  char station_id[21];
  memcpy(station_id, "idStation=", 10);
  size_t station_length = 0;
  for (uint32_t n = station; !station_length || n; n /= 10)
    ++station_length;
  for (size_t n = station, i = station_length; i--; n /= 10)
    *(station_id + 10 + i) = '0' + (char)(n % 10);
  station_id[10 + station_length] = 0;

  
  delay(1024);
  digitalWrite(GPRS_RESET_PIN, LOW);
  delay(8000);
  
  int response = -1;
  size_t post_response_size;
  char* post_response;

  bool success = at_http_post(host, 80, path, station_id, &response, &post_response_size, &post_response);
  if (!success || !post_response_size)
    return false;

  if (response < 200 || response > 299)
    return false;

  uint32_t t = 0;

  for (size_t i = 0; i != post_response_size; ++i)
    if (post_response[i] < '0' || post_response[i] > '9')
      return false;
    else
      t = (t * 10) + (uint32_t)(post_response[i] - '0');
    
  *interval = t;
  
  return true;
}

bool inp_http_post_measurements(const char* host, const char* path, unsigned long station, float temperature, float humididy, float pressure, float illuminance)
{
  digitalWrite(GPRS_RESET_PIN, HIGH);
   
  static char post_key_value_pairs[119];
  size_t station_length = 0;
  for (unsigned long n = station; !station_length || n; n /= (unsigned long)10)
    ++station_length;
  if (station_length > 10)
  {
    digitalWrite(GPRS_RESET_PIN, LOW);
    return false;
  }
  memcpy(post_key_value_pairs, "idStation=", 10);
  for (size_t o = 10, n = station, i = station_length; i--; n /= 10)
    *(post_key_value_pairs + o + i) = '0' + (char)(n % 10);
  memcpy(post_key_value_pairs + 10 + station_length, "&temperature=", 13);
  size_t temperature_length = print_float(temperature, 4, post_key_value_pairs + 10 + station_length + 13);
  memcpy(post_key_value_pairs + 10 + station_length + 13 + temperature_length, "&humidity=", 10);
  size_t humididy_length = print_float(humididy, 4, post_key_value_pairs + 10 + station_length + 13 + temperature_length + 10);
  memcpy(post_key_value_pairs + 10 + station_length + 13 + temperature_length + 10 + humididy_length, "&pressure=", 10);
  size_t pressure_length = print_float(pressure, 4, post_key_value_pairs + 10 + station_length + 13 + temperature_length + 10 + humididy_length + 10);
  memcpy(post_key_value_pairs + 10 + station_length + 13 + temperature_length + 10 + humididy_length + 10 + pressure_length, "&illuminance=", 13);
  size_t illuminance_length = print_float(illuminance, 4, post_key_value_pairs + 10 + station_length + 13 + temperature_length + 10 + humididy_length + 10 + pressure_length + 13);
  post_key_value_pairs[10 + station_length + 13 +  temperature_length + 10 + humididy_length + 10 + pressure_length + 13 + illuminance_length] = 0;

  delay(1024);
  digitalWrite(GPRS_RESET_PIN, LOW);
  delay(8000);
  
  int response = -1;
  size_t post_response_size;
  char* post_response;

  //my_serial.print("post \"");
  //my_serial.print(post_key_value_pairs);
  //my_serial.print("\"\n");
 

  bool success = at_http_post(host, 80, path, post_key_value_pairs, &response, &post_response_size, &post_response);
  if (!success)
  {
     //my_serial.print("post error\n");
    return false;
  }

  //my_serial.print("post ok\n");

  if (response < 200 || response > 299)
    return false;

  return true;
}


bool inp_coap_post(const char* host, uint32_t station, float temperature, float humididy, float pressure, float illuminance)
{
  uint8_t coap_version = 1;
  uint8_t transaction_type = 1;// non-confirmable
  uint8_t token_length = 0;
  uint8_t code = 2;//post
  uint16_t message_id = 0xFFFF;
  uint32_t message_temperature = (uint32_t)(temperature * 65536.0f);
  uint32_t message_humididy = (uint32_t)(humididy * 65536.0f);
  uint32_t message_pressure = (uint32_t)(pressure * 65536.0f);
  uint32_t message_illuminance = (uint32_t)(illuminance * 65536.0f);
  uint8_t message[25] = {
    (token_length & 0xF) | ((transaction_type & 0x3) << 4) | ((coap_version & 0x3) << 6),
    code,
    (uint8_t)(message_id & 0xFF),
    (uint8_t)((message_id >> 8) & 0xFF),
    0xFF,
    (uint8_t)(station & 0xFF),
    (uint8_t)((station >> 8) & 0xFF),
    (uint8_t)((station >> 16) & 0xFF),
    (uint8_t)((station >> 24) & 0xFF),
    (uint8_t)(message_temperature & 0xFF),
    (uint8_t)((message_temperature >> 8) & 0xFF),
    (uint8_t)((message_temperature >> 16) & 0xFF),
    (uint8_t)((message_temperature >> 24) & 0xFF),
    (uint8_t)(message_humididy & 0xFF),
    (uint8_t)((message_humididy >> 8) & 0xFF),
    (uint8_t)((message_humididy >> 16) & 0xFF),
    (uint8_t)((message_humididy >> 24) & 0xFF),
    (uint8_t)(message_pressure & 0xFF),
    (uint8_t)((message_pressure >> 8) & 0xFF),
    (uint8_t)((message_pressure >> 16) & 0xFF),
    (uint8_t)((message_pressure >> 24) & 0xFF),
    (uint8_t)(message_illuminance & 0xFF),
    (uint8_t)((message_illuminance >> 8) & 0xFF),
    (uint8_t)((message_illuminance >> 16) & 0xFF),
    (uint8_t)((message_illuminance >> 24) & 0xFF) };
  
  if (!at_udp_send(host, 5683, sizeof(message), message))
    return false;

  return true;
}

