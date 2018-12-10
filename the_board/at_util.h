#include <Arduino.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

bool at_wait_at_module_wakeup(unsigned long wait_for);

bool at_generic_command(const char* command, size_t result_buffer_length, char* result_buffer, size_t* result_length, unsigned long wait_for_result);

bool at_get_cfun(int* value);

bool at_set_cfun(int value);

bool at_get_cpin(bool* value);

bool at_set_cpin(const char* value);

bool at_get_creg(int* value);

bool at_set_creg(int value);

bool at_get_cgatt(int* value);

bool at_set_cgatt(int value);

bool at_enable_cmnet();

bool at_ciicr();

bool at_cifsr(size_t* ip_address_length, char** ip_address);

bool at_http_post(const char* host, uint16_t port, const char* path, const char* key_value_pairs, int* response_status, size_t* response_size, char** response);

bool at_udp_send(const char* host, uint16_t port, size_t size, void* data);