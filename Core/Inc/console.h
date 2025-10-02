/**
  ******************************************************************************
  * @file           : console.h
  * @brief          : Header file for command line interface 
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 Clinton Guenther
  * All rights reserved.
  *
  ******************************************************************************
  */
#ifndef __CONSOLE_H
#define __CONSOLE_H

#include "stm32f1xx_hal.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>      // Mostly needed for sscanf()
#include "uart.h"
#include "main.h"
#include "sensor.h"

extern UART_HandleTypeDef huart1;

// TODO Update all comment BLOCKS!!
// TODO Reference console.c

/**
* @brief Main implementation of command-line interface 
* @param: Void passed in
* @retval None
*/
void console_menu( void );


/**
* @brief Get unsigned (u8) number from user
* @param: Void passed in
* @retval uint8_t
*/
uint8_t get_uint8 ( void );

/**
* @brief Get float number from user
* @param: Void passed in
* @retval User-defined float value
*/
float get_float ( void );

#endif
/* END OF FILE */
