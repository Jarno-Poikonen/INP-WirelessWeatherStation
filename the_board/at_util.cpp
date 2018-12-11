#include <Arduino.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "at_util.h"
#include "inp_power.h"

//#define AT_UTIL_DEBUG_OUT
#ifdef AT_UTIL_DEBUG_OUT
#include <SoftwareSerial.h>
SoftwareSerial my_serial(7, 12); // RX, TX
#endif

const char* at_pin_code = "0000";

static char at_buffer[300];

inline unsigned long at_time()
{
	return millis();
}

inline void at_wait(unsigned long t)
{
	delay(t);
}

inline bool at_gprs_available()
{
	return Serial.available();
};

inline char at_gprs_read()
{
	char tmp = Serial.read();
#ifdef AT_UTIL_DEBUG_OUT
	if (tmp == 0x0D)
	  my_serial.print("\\x0D");
	else if (tmp == 0x0A)
	  my_serial.print("\\x0A\n");
	else
	  my_serial.write(tmp);
#endif
	return tmp;
};

inline void at_gprs_write(char data)
{
	Serial.write(data);
};

inline void at_gprs_print(const char* string)
{
	Serial.print(string);
};

static bool at_gprs_wait_for_available(unsigned long wait_for)
{
	for (unsigned long wait_limit = at_time() + wait_for; at_time() < wait_limit;)
		if (Serial.available())
			return true;
  at_wait(1024);
	return false;
};

size_t at_get_line_length(size_t buffer_length, const char* buffer)
{
	size_t length = 0;
	while (length != buffer_length && buffer[length] != '\n' && buffer[length] != '\r')
		++length;
	return length;
}

size_t at_get_next_line(size_t buffer_length, const char* buffer)
{
	for (size_t i = 0; i != buffer_length; ++i)
		if (buffer[i] == '\n' || buffer[i] == '\r')
		{
			if (i + 1 == buffer_length)
				return buffer_length;
			else
			{
				if ((buffer[i] == '\n' && buffer[i + 1] == '\r') || (buffer[i] == '\r' && buffer[i + 1] == '\n'))
					return i + 2;
				else
					return i + 1;
			}
		}
	return (size_t)~0;
}

size_t at_find_string(size_t buffer_length, const char* buffer, size_t string_length, const char* string)
{
	if (string_length > buffer_length)
		return (size_t)~0;
	for (size_t index = 0;; ++index)
	{
		size_t match = 0;
		while (match != string_length && buffer[index + match] == string[match])
			++match;
		if (match == string_length)
			return index;
		if (index + string_length == buffer_length)
			return (size_t)~0;
	}
}

bool at_validate_iv4_address(size_t length, const char* string)
{
	size_t offset = 0;
	for (size_t i = 4; i--;)
	{
		size_t decimal_count = 0;
		uint8_t tmp = 0;
		while (offset != length && string[offset] >= '0' && string[offset] <= '9')
		{
			uint8_t decimal = (uint8_t)(string[offset] - '0');
			if (((uint16_t)10 * (uint16_t)tmp + (uint16_t)decimal) & 0xFF00)
				return false;
			tmp = (uint8_t)10 * tmp + decimal;
			++decimal_count;
			++offset;
		}
		if (!decimal_count || (decimal_count > 1 && tmp < 10) || (decimal_count > 2 && tmp < 100) || decimal_count > 3)
			return false;
		if (i)
		{
			if (string[offset] == '.')
				++offset;
			else
				return false;
		}
	}
	return true;
}

size_t at_read_serial_input(size_t buffer_length, char* buffer, unsigned long wait_for)
{
	if (!wait_for)
	{
		if (!at_gprs_available())
			return (size_t)~0;
	}
	else if (wait_for == (unsigned long)~0)
	{
		while (!at_gprs_available())
			at_wait(1);
	}
	else
	{
		for (unsigned long wait_limit = at_time() + wait_for; !at_gprs_available() && at_time() < wait_limit;)
			at_wait(1);
		if (!at_gprs_available())
			return (size_t)~0;
	}
	size_t length = 0;
	for (bool available = true; length != buffer_length && available;)
	{
		buffer[length++] = at_gprs_read();

		if (at_gprs_available())
			available = true;
		else
		{
			at_wait(1);
			if (at_gprs_available())
				available = true;
			else
			{
				at_wait(16);
				available = (bool)at_gprs_available();
			}
		}
	}
	return length;
}

static void at_empty_serial()
{
	at_wait(16);
	at_read_serial_input(sizeof(at_buffer), at_buffer, 16);
}

bool at_wait_at_module_wakeup(unsigned long wait_for)
{
#ifdef AT_UTIL_DEBUG_OUT
  static bool my_serial_init = false;
  if (!my_serial_init)
  {
    my_serial.begin(9600);
    my_serial.print("AT_UTIL_DEBUG_OUT\n");
    my_serial_init = true;
  }
#endif
	at_empty_serial();
	for (unsigned long wait_limit = at_time() + wait_for; at_time() < wait_limit;)
	{
		at_gprs_print("AT\x0D\x0A");
		size_t length = at_read_serial_input(sizeof(at_buffer), at_buffer, 256);
		if (length != (size_t)~0 && length > 5 && at_find_string(length, at_buffer, 6, "\x0D\x0AOK\x0D\x0A"))
			return true;
		else if (at_time() + 256 < wait_limit)
			at_wait(1000);
	}
	return false;
}

bool at_generic_command(const char* command, size_t result_buffer_length, char* result_buffer, size_t* result_length, unsigned long wait_for_result)
{
	at_empty_serial();
	at_gprs_print(command);
	at_gprs_write('\x0D');
	at_gprs_write('\x0A');
	at_wait(1);
	size_t result_read = 0;
	unsigned long wait_limit = at_time() + wait_for_result;
	for (;;)
	{
		unsigned long now = at_time();
		if (wait_limit < now)
		{
			*result_length = 0;
			return false;
		}
		size_t input_length = at_read_serial_input(result_buffer_length - result_read, result_buffer + result_read, wait_limit - now);
		if (input_length == (size_t)~0)
		{
			*result_length = 0;
			return false;
		}

		//Serial.write(0);
		//for (size_t i = 0; i != input_length; ++i)
		//  Serial.write(result_buffer[result_read + i]);
		//Serial.write(0);
		result_read += input_length;

		size_t result_end = at_find_string(result_read, result_buffer, 6, "\x0D\x0AOK\x0D\x0A");
		if (result_end == (size_t)~0)
			result_end = at_find_string(result_read, result_buffer, 11, "\x0D\x0ACLOSE OK\x0D\x0A");
		if (result_end != (size_t)~0)
		{
			*result_length = result_read;
			return true;
		}
		result_end = at_find_string(result_read, result_buffer, 9, "\x0D\x0AERROR\x0D\x0A");
		if (result_end == (size_t)~0)
			result_end = at_find_string(result_read, result_buffer, 37, "\x0D\x0A+CME ERROR: operation not allowed\x0D\x0A");
		if (result_end != (size_t)~0)
		{
			*result_length = 0;
			return true;
		}
	}
}

bool at_get_cfun(int* value)
{
	at_empty_serial();
	size_t length;
	bool successful = at_generic_command("AT+CFUN?", sizeof(at_buffer), at_buffer, &length, 10000);
	if (!successful)
		return false;
	size_t offset = at_find_string(length, at_buffer, 9, "\x0D\x0A+CFUN: ");
	if (offset == (size_t)~0)
		return false;
	offset += 9;
	length = at_get_line_length(length - offset, at_buffer + offset);
	if (length != 1 || at_buffer[offset] < '0' || at_buffer[offset] > '9')
		return false;
	*value = (int)(at_buffer[offset] - '0');
	return true;
}

bool at_set_cfun(int value)
{
	at_empty_serial();
	char set_cfun[10] = { 'A', 'T', '+', 'C', 'F', 'U', 'N', '=', '0' + (char)value, 0 };
	size_t length;
	bool successful = at_generic_command(set_cfun, sizeof(at_buffer), at_buffer, &length, 10000);
	if (!successful)
		return false;
	return true;
}

bool at_get_cpin(bool* value)
{
	at_empty_serial();
	size_t length;
	bool successful = at_generic_command("AT+CPIN?", sizeof(at_buffer), at_buffer, &length, 10000);
	if (!successful)
		return false;
	size_t offset = at_find_string(length, at_buffer, 9, "\x0D\x0A+CPIN: ");
	if (offset == (size_t)~0)
		return false;
	offset += 9;
	length = at_get_line_length(length - offset, at_buffer + offset);
	if (length == 7 && !memcmp(at_buffer + offset, "SIM PIN", 7))
	{
		*value = false;
		return true;
	}
	if (length == 5 && !memcmp(at_buffer + offset, "READY", 5))
	{
		*value = true;
		return true;
	}
	return false;
}

bool at_set_cpin(const char* value)
{
	if (!value[0] || !value[1] || !value[2] || !value[3] || value[4])
		return false;
	at_empty_serial();
	char set_cpin[13] = { 'A', 'T', '+', 'C', 'P', 'I', 'N', '=', value[0], value[1], value[2], value[3], 0 };
	size_t length;
	at_generic_command(set_cpin, sizeof(at_buffer), at_buffer, &length, 10000);
	at_wait(8000);
	bool cpin_ready;
	if (!at_get_cpin(&cpin_ready) || !cpin_ready)
		return false;
	return true;
}

bool at_get_creg(int* value)
{
	at_empty_serial();
	size_t length;
	bool successful = at_generic_command("AT+CREG?", sizeof(at_buffer), at_buffer, &length, 10000);
	if (!successful)
		return false;
	size_t offset = at_find_string(length, at_buffer, 9, "\x0D\x0A+CREG: ");
	if (offset == (size_t)~0)
		return false;
	offset += 9;
	length = at_get_line_length(length - offset, at_buffer + offset);
	if (length < 3 || at_buffer[offset] < '0' || at_buffer[offset] > '9' || at_buffer[offset + 1] != ',' || at_buffer[offset + 2] < '0' || at_buffer[offset + 2] > '9')
		return false;
	*value = (int)(at_buffer[offset] - '0');
	return true;
}

bool at_set_creg(int value)
{
	at_empty_serial();
	char set_cfun[10] = { 'A', 'T', '+', 'C', 'R', 'E', 'G', '=', '0' + (char)value, 0 };
	size_t length;
	at_generic_command(set_cfun, sizeof(at_buffer), at_buffer, &length, 10000);
	at_wait(8000);
	int creg_value;
	if (!at_get_creg(&creg_value) || creg_value != value)
		return false;
	return true;
}

bool at_get_cgatt(int* value)
{
	at_empty_serial();
	size_t length;
	bool successful = at_generic_command("AT+CGATT?", sizeof(at_buffer), at_buffer, &length, 10000);
	if (!successful)
		return false;
	size_t offset = at_find_string(length, at_buffer, 10, "\x0D\x0A+CGATT: ");
	if (offset == (size_t)~0)
		return false;
	offset += 10;
	length = at_get_line_length(length - offset, at_buffer + offset);
	if (length != 1 || at_buffer[offset] < '0' || at_buffer[offset] > '9')
		return false;
	*value = (int)(at_buffer[offset] - '0');
	return true;
}

bool at_set_cgatt(int value)
{
	at_empty_serial();
	char set_cgatt[11] = { 'A', 'T', '+', 'C', 'G', 'A', 'T', 'T', '=', '0' + (char)value, 0 };
	size_t length;
	at_generic_command(set_cgatt, sizeof(at_buffer), at_buffer, &length, 10000);
	at_wait(8000);
	int cgatt_value;
	if (!at_get_cgatt(&cgatt_value) || cgatt_value != value)
		return false;
	return true;
}

bool at_enable_cmnet()
{
	at_empty_serial();
	size_t length;
	at_generic_command("AT+CSTT=\"CMNET\"", sizeof(at_buffer), at_buffer, &length, 10000);
	at_wait(16);
}

bool at_ciicr()
{
	// Bring up wireless connection (GPRS or CSD)
	at_empty_serial();
	size_t length;
	at_generic_command("AT+CIICR\x0D\x0A", sizeof(at_buffer), at_buffer, &length, 10000);
	at_wait(16);
}

bool at_cifsr(size_t* ip_address_length, char** ip_address)
{
	at_empty_serial();
	at_gprs_print("AT+CIFSR\x0D\x0A");
	at_wait(16);
	size_t length = at_read_serial_input(sizeof(at_buffer), at_buffer, 10000);
	if (length == (size_t)~0)
		return false;
	size_t offset = at_find_string(length, at_buffer, 11, "AT+CIFSR\x0D\x0D\x0A");
	if (offset == (size_t)~0)
		return false;
	offset += 11;
	length = at_get_line_length(length - offset, at_buffer + offset);
	if (!at_validate_iv4_address(length, at_buffer + offset))
		return false;
	*ip_address_length = length;
	*ip_address = at_buffer + offset;
	return true;
}

bool at_tcp_connect(const char* host, uint16_t port)
{
	at_empty_serial();
	char port_string[6] = { 0, 0, 0, 0, 0, 0 };
	do
	{
		memmove(port_string + 1, port_string, 5);
		port_string[0] = '0' + (char)(port % 10);
		port /= 10;
	} while (port);
	at_gprs_print("AT+CIPSHUT\x0D\x0A");
	at_wait(100);
	at_empty_serial();
	at_gprs_print("AT+CIPMUX=0\x0D\x0A");
	at_wait(100);
	at_empty_serial();
	at_gprs_print("AT+CIPRXGET=1\x0D\x0A");
	at_empty_serial();
	at_wait(100);
	at_gprs_print("AT+CIPSTART=\"TCP\",\"");
	at_gprs_print(host);
	at_gprs_print("\",\"");
	at_gprs_print(port_string);
	at_gprs_print("\"\x0D\x0A");
	size_t length;
	for (unsigned long retry = 0, wait_limit = at_time() + 8000;;)
	{
		unsigned long now = at_time();
		bool connect_timeout = wait_limit < now;
		if (!connect_timeout)
		{
			length = at_read_serial_input(sizeof(at_buffer), at_buffer, wait_limit - now);
			if (length == (size_t)~0)
				connect_timeout = true;
			length = at_find_string(length, at_buffer, 9, "CONNECT OK");
			if (length != (size_t)~0)
				break;
		}
		if (connect_timeout)
		{
			if (retry)
				return false;
			retry = 1;
			at_generic_command("AT+CIPCLOSE\x0D\x0A", sizeof(at_buffer), at_buffer, &length, 8000);
			at_empty_serial();
			at_gprs_print("AT+CIPSTART=\"TCP\",\"");
			at_gprs_print(host);
			at_gprs_print("\",\"");
			at_gprs_print(port_string);
			at_gprs_print("\"\x0D\x0A");
			wait_limit = at_time() + 60000;
		}
	}
	return true;
}

bool at_tcp_close()
{
  char tmp_buffer[12];
	at_wait(16);
  at_read_serial_input(sizeof(tmp_buffer), tmp_buffer, 16);
  at_gprs_print("AT+CIPCLOSE\x0D\x0A");
  for (bool wait_close_ok = true; wait_close_ok;)
  {
    memmove(tmp_buffer, tmp_buffer + 1, sizeof(tmp_buffer) - 1);
    if (!at_gprs_wait_for_available(8000))
      return true;
    tmp_buffer[sizeof(tmp_buffer) - 1] = at_gprs_read();
    wait_close_ok = memcmp(tmp_buffer, "\r\nCLOSE OK\r\n", sizeof(tmp_buffer)) != 0;
  }
	return true;
}

bool at_ip_send(size_t size, const void* data)
{
	at_empty_serial();
	char send_ok_cmp_buffer[11];
	memset(send_ok_cmp_buffer, 0, sizeof(send_ok_cmp_buffer));
	char size_string[6] = { 0, 0, 0, 0, 0, 0 };
	for (size_t l = 0, n = size; !l || n; n /= 10, ++l)
	{
		memmove(size_string + 1, size_string, l);
		size_string[0] = '0' + (char)(n % 10);
	}
	at_gprs_print("AT+CIPSEND=");
	at_gprs_print(size_string);
	at_gprs_print("\x0D\x0A");
	for (unsigned long wait_limit = at_time() + 60000;;)
	{
		if (at_gprs_available() && at_gprs_read() == '>')
			break;
		if (at_time() > wait_limit)
			return false;
	}
	for (const char* i = (const char*)data, *l = i + size; i != l; ++i)
		at_gprs_write(*i);
	for (bool wait_close_ok = true; wait_close_ok;)
	{
		memmove(send_ok_cmp_buffer, send_ok_cmp_buffer + 1, sizeof(send_ok_cmp_buffer) - 1);
		if (!at_gprs_wait_for_available(60000))
			return false;
		send_ok_cmp_buffer[sizeof(send_ok_cmp_buffer) - 1] = at_gprs_read();
		wait_close_ok = memcmp(send_ok_cmp_buffer, "\x0D\x0ASEND OK\x0D\x0A", sizeof(send_ok_cmp_buffer)) != 0;
	}
	//at_empty_serial();
	return true;
}

bool at_tcp_receive(void** buffer, size_t* data_size)
{
  at_wait(100);
	at_empty_serial();
	char at_buffer_size_string[6];
	char ciprxget_cmp_buffer[15];
	size_t result_size = 0;
	for (bool read_more = true; read_more;)
	{
		memset(at_buffer_size_string, 0, sizeof(at_buffer_size_string));
		memset(ciprxget_cmp_buffer, 0, sizeof(ciprxget_cmp_buffer));
		for (size_t l = 0, n = ((sizeof(at_buffer) - result_size) > 64) ? 64 : (sizeof(at_buffer) - result_size); !l || n; n /= 10, ++l)
		{
			memmove(at_buffer_size_string + 1, at_buffer_size_string, l);
			at_buffer_size_string[0] = '0' + (char)(n % 10);
		}
		size_t read_size;
		size_t read_more_size;
		for (unsigned long wait_limit = at_time() + 8000;;)
		{
			at_gprs_print("AT+CIPRXGET=2,");
			at_gprs_print(at_buffer_size_string);
			at_gprs_print("\x0D\x0A");
			bool error = false;
			for (bool ciprxget_cmp = true; ciprxget_cmp;)
			{
				memmove(ciprxget_cmp_buffer, ciprxget_cmp_buffer + 1, sizeof(ciprxget_cmp_buffer) - 1);
				if (!at_gprs_wait_for_available(8000))
				{
					error = true;
					break;
				}
				ciprxget_cmp_buffer[sizeof(ciprxget_cmp_buffer) - 1] = at_gprs_read();
				ciprxget_cmp = memcmp(ciprxget_cmp_buffer, "\x0D\x0A+CIPRXGET: 2,", 15) != 0;
			}
			read_size = 0;
			if (!error)
				for (bool read_result_size = true; read_result_size;)
				{
					if (!at_gprs_wait_for_available(8000))
					{
						error = true;
						break;
					}
					char input = at_gprs_read();
					if (input == ',')
						read_result_size = false;
					else
						read_size = (read_size * 10) + (size_t)(input - '0');
				}
			read_more_size = 0;
			if (!error)
				for (bool read_result_size = true; read_result_size;)
				{
					if (!at_gprs_wait_for_available(8000))
					{
						error = true;
						break;
					}
					char input = at_gprs_read();
					if (input == '\x0D')
						read_result_size = false;
					else
						read_more_size = (read_more_size * 10) + (size_t)(input - '0');
				}
			if (!error)
				for (bool read_new_line = true; read_new_line;)
					if (at_gprs_wait_for_available(8000))
					{
						if (at_gprs_read() == '\x0A')
							read_new_line = false;
					}
					else
					{
						error = true;
						break;
					}
			if (read_size > sizeof(at_buffer) - result_size)
				read_size = sizeof(at_buffer) - result_size;
			if (read_size || wait_limit < at_time())
				break;
		}
		for (char* i = (char*)at_buffer + result_size, *l = i + read_size; i != l; ++i)
		{
			if (!at_gprs_wait_for_available(8000))
				return false;
			*i = at_gprs_read();
		}
		result_size += read_size;
		if (!read_more_size || result_size == sizeof(at_buffer))
			read_more = false;
		else
			at_wait(128);
	}
/*
  my_serial.write('"');
  for (char* i = (char*)at_buffer, * l = i + result_size; i != l; ++i)
    my_serial.write(*i);
  my_serial.write('"');
 */
	*buffer = (void*)at_buffer;
	*data_size = result_size;
	return true;
}

const uint8_t at_http_post_str[5] PROGMEM = { 0x50, 0x4f, 0x53, 0x54, 0x20 };
const uint8_t at_http_host_str[17] PROGMEM = { 0x20, 0x48, 0x54, 0x54, 0x50, 0x2f, 0x31, 0x2e, 0x31, 0x0d, 0x0a, 0x48, 0x6f, 0x73, 0x74, 0x3a, 0x20 };
const uint8_t at_http_en_str[86] PROGMEM = { 0x0d, 0x0a, 0x43, 0x6f, 0x6e, 0x6e, 0x65, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x3a, 0x20, 0x63, 0x6c, 0x6f, 0x73, 0x65, 0x0d, 0x0a, 0x43, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 0x74, 0x2d, 0x54, 0x79, 0x70, 0x65, 0x3a, 0x20, 0x61, 0x70, 0x70, 0x6c, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x2f, 0x78, 0x2d, 0x77, 0x77, 0x77, 0x2d, 0x66, 0x6f, 0x72, 0x6d, 0x2d, 0x75, 0x72, 0x6c, 0x65, 0x6e, 0x63, 0x6f, 0x64, 0x65, 0x64, 0x0d, 0x0a, 0x43, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 0x74, 0x2d, 0x4c, 0x65, 0x6e, 0x67, 0x74, 0x68, 0x3a, 0x20 };

bool at_http_post(const char* host, uint16_t port, const char* path, const char* key_value_pairs, int* response_status, size_t* response_size, char** response)
{
	const size_t post_constant_data_size = 112;
	size_t post_host_length = strlen(host);
	size_t post_path_length = strlen(path);
	size_t post_key_value_pair_length = strlen(key_value_pairs);
	char post_key_value_pair_length_string[6] = { 0, 0, 0, 0, 0, 0 };
	size_t post_key_value_pair_length_string_length = 0;
	for (size_t n = post_key_value_pair_length; !post_key_value_pair_length_string_length || n; n /= 10, ++post_key_value_pair_length_string_length)
	{
		memmove(post_key_value_pair_length_string + 1, post_key_value_pair_length_string, post_key_value_pair_length_string_length);
		post_key_value_pair_length_string[0] = '0' + (char)(n % 10);
	}
	size_t post_length = post_constant_data_size + post_host_length + post_path_length + post_key_value_pair_length_string_length + post_key_value_pair_length;
	if (post_length > sizeof(at_buffer))
		return false;
	at_wait_at_module_wakeup(10000);
	int cfun;
	if (!at_get_cfun(&cfun))
		return false;
	if (cfun != 1)
	{
		cfun = 1;
		if (!at_set_cfun(cfun))
			return false;
	}
	bool pin_ready;
	if (!at_get_cpin(&pin_ready))
		return false;
	if (!pin_ready)
	{
		if (at_set_cpin(at_pin_code))
			pin_ready = true;
		else
			return false;
	}
	int creg;
	if (!at_get_creg(&creg))
		return false;
	if (creg != 1)
	{
		creg = 1;
		if (!at_set_creg(creg))
			return false;
	}
	int cgatt;
	if (!at_get_cgatt(&cgatt))
		return false;
	if (cgatt != 1)
	{
		cgatt = 1;
		if (!at_set_cgatt(cgatt))
			return false;
	}
	at_enable_cmnet();
	at_ciicr();
	size_t ip_address_length;
	char* ip_address;
	if (!at_cifsr(&ip_address_length, &ip_address))
		return false;
	if (!at_tcp_connect(host, port))
		return false;

  memcpy_P(at_buffer, at_http_post_str, 5);
	//memcpy(at_buffer, "POST ", 5);
	memcpy(at_buffer + 5, path, post_path_length);
  memcpy_P(at_buffer + 5 + post_path_length, at_http_host_str, 17);
	//memcpy(at_buffer + 5 + post_path_length, " HTTP/1.1\r\nHost: ", 17);
	memcpy(at_buffer + 5 + post_path_length + 17, host, post_host_length);
  memcpy_P(at_buffer + 5 + post_path_length + 17 + post_host_length, at_http_en_str, 86);
	//memcpy(at_buffer + 5 + post_path_length + 17 + post_host_length, "\r\nConnection: close\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: ", 86);
	memcpy(at_buffer + 5 + post_path_length + 17 + post_host_length + 86, post_key_value_pair_length_string, post_key_value_pair_length_string_length);
	memcpy(at_buffer + 5 + post_path_length + 17 + post_host_length + 86 + post_key_value_pair_length_string_length, "\r\n\r\n", 4);
	memcpy(at_buffer + 5 + post_path_length + 17 + post_host_length + 86 + post_key_value_pair_length_string_length + 4, key_value_pairs, post_key_value_pair_length);
	if (!at_ip_send(post_length, at_buffer))
	{
		at_tcp_close();
		return false;
	}
	at_wait(16);
	size_t packet_size;
	char* post_result;
	if (!at_tcp_receive((void**)&post_result, &packet_size))
		return false;
	at_tcp_close();

#ifdef AT_UTIL_DEBUG_OUT
  my_serial.print("\n\"");
  for (char* i = (char*)post_result, * l = i + packet_size; i != l; ++i)
    my_serial.write(*i);
  my_serial.print("\"\n");
#endif

	if (!packet_size)
		return false;

	size_t result_length = (size_t)packet_size;
	size_t result_status_line_length = (size_t)~0;
	for (size_t i = 0; result_status_line_length == (size_t)~0 && i + 2 < result_length; ++i)
		if (post_result[i] == '\r' && post_result[i + 1] == '\n')
			result_status_line_length = i;
	if (result_status_line_length == (size_t)~0 || result_status_line_length < 13 || memcmp(post_result, "HTTP/1.1 ", 9) ||
		post_result[9] < '0' || post_result[9] > '9' || post_result[10] < '0' || post_result[10] > '9' || post_result[11] < '0' || post_result[11] > '9' ||
		post_result[12] != ' ')
	{
		return false;
	}
	size_t result_header_size = (size_t)~0;
	for (size_t i = result_status_line_length; result_header_size == (size_t)~0 && i + 4 < result_length; ++i)
		if (post_result[i] == '\r' && post_result[i + 1] == '\n' && post_result[i + 2] == '\r' && post_result[i + 3] == '\n')
			result_header_size = i + 4;
	if (result_header_size == (size_t)~0)
	{
		return false;
	}

	*response_status = ((int)(post_result[9] - '0') * 100) + ((int)(post_result[10] - '0') * 10) + (int)(post_result[11] - '0');
	memmove(post_result, post_result + result_header_size, result_length - result_header_size);
	*response_size = result_length - result_header_size;
	*response = post_result;

  //my_serial.print("\nHTTP STATUS RESPONSE: ");
  //my_serial.print(*response_status);
  //my_serial.print("\n");

	return true;
}

bool at_udp_start(const char* host, uint16_t port)
{
	at_empty_serial();
	char port_string[6] = { 0, 0, 0, 0, 0, 0 };
	do
	{
		memmove(port_string + 1, port_string, 5);
		port_string[0] = '0' + (char)(port % 10);
		port /= 10;
	} while (port);
	at_gprs_print("AT+CIPSHUT\x0D\x0A");
	at_wait(100);
	at_empty_serial();
	at_gprs_print("AT+CIPMUX=0\x0D\x0A");
	at_wait(100);
	at_empty_serial();
	at_gprs_print("AT+CIPRXGET=1\x0D\x0A");
	at_empty_serial();
	at_wait(100);
	at_gprs_print("AT+CIPSTART=\"UDP\",\"");
	at_gprs_print(host);
	at_gprs_print("\",\"");
	at_gprs_print(port_string);
	at_gprs_print("\"\x0D\x0A");
	size_t length;
	for (unsigned long retry = 0, wait_limit = at_time() + 8000;;)
	{
		unsigned long now = at_time();
		bool connect_timeout = wait_limit < now;
		if (!connect_timeout)
		{
			length = at_read_serial_input(sizeof(at_buffer), at_buffer, wait_limit - now);
			if (length == (size_t)~0)
				connect_timeout = true;
			length = at_find_string(length, at_buffer, 9, "CONNECT OK");
			if (length != (size_t)~0)
				break;
		}
		if (connect_timeout)
		{
			if (retry)
				return false;
			retry = 1;
			at_generic_command("AT+CIPCLOSE\x0D\x0A", sizeof(at_buffer), at_buffer, &length, 8000);
			at_empty_serial();
			at_gprs_print("AT+CIPSTART=\"TCP\",\"");
			at_gprs_print(host);
			at_gprs_print("\",\"");
			at_gprs_print(port_string);
			at_gprs_print("\"\x0D\x0A");
			wait_limit = at_time() + 60000;
		}
	}
	return true;
}

bool at_udp_send(const char* host, uint16_t port, size_t size, void* data)
{
  digitalWrite(GPRS_RESET_PIN, HIGH);
  delay(1024);
  digitalWrite(GPRS_RESET_PIN, LOW);
  delay(8000);
  
	at_wait_at_module_wakeup(10000);

	int cfun;
	if (!at_get_cfun(&cfun))
		return false;
	if (cfun != 1)
	{
		cfun = 1;
		if (!at_set_cfun(cfun))
			return false;
	}
	bool pin_ready;
	if (!at_get_cpin(&pin_ready))
		return false;
	if (!pin_ready)
	{
		if (at_set_cpin(at_pin_code))
			pin_ready = true;
		else
			return false;
	}
	int creg;
	if (!at_get_creg(&creg))
		return false;
	if (creg != 1)
	{
		creg = 1;
		if (!at_set_creg(creg))
			return false;
	}
	int cgatt;
	if (!at_get_cgatt(&cgatt))
		return false;
	if (cgatt != 1)
	{
		cgatt = 1;
		if (!at_set_cgatt(cgatt))
			return false;
	}
	at_enable_cmnet();
	at_ciicr();
	if (!at_udp_start(host, port))
		return false;
	if (!at_ip_send(size, data))
	{
		at_tcp_close();
		return false;
	}
	at_tcp_close();
	return true;
}





