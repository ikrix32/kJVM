#ifndef _BLE_CONSOLE_
#define _BLE_CONSOLE_

#include <stdint.h>
#include <stdbool.h>

#include "config.h"

#ifdef BLE_CONSOLE
void init_ble_console();
void console_print(uint8_t * p_data, int length);
bool is_ble_console_connected();
void power_manage(void);
#endif
#endif
