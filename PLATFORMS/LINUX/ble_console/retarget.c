/*----------------------------------------------------------------------------
 * Name:    Retarget.c
 * Purpose: 'Retarget' layer for target-dependent low level functions
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the µVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2009 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "retarget.h"
#include "console.h"

#ifdef BLE_CONSOLE
void ble_print( const char * format, ... )
{
  char buffer[256];
  va_list args;
  va_start (args, format);
  vsprintf (buffer,format, args);
  console_print((uint8_t*)&buffer,strlen(buffer));
  va_end (args);
}
#else
void print( const char * format, ... ){
	char buffer[256];
  va_list args;
  va_start (args, format);
  vsprintf (buffer,format, args);
  //console_print((uint8_t*)&buffer,strlen(buffer));
  va_end (args);
}
#endif


#include "system_nrf51.h"
//extern "C" {
void HardFault_Handler(void)
{
  __asm volatile("BKPT #01"); 
  //while(1); 
}
//}

