/*
 * sensor.h
 *
 *  Created on: Sep 14, 2025
 *      Author: C. Guenther
 */

#ifndef INC_SPI_H_
#define INC_SPI_H_

#include "main.h"
#include "stm32f1xx_hal.h"

extern SPI_HandleTypeDef   hspi1;

/*!
 * Structure for the sensor 
 */
struct TEMPMembers {
    float       flt_temp_value;             // Current temperature value
    float       temperature_buffer[8];      // Buffer for temperature values 
    uint8_t     temp_buf_index;             // Current index of the temperature buffer
    
    bool        fan_transition;             // Flag to capture fan transitions (like off to on)
    bool        fan_on;                     // Flag to indicate when the fan is on
    uint16_t    fan_transition_blink_count; // Flag to capture fan transitions (like off to on)
};


/**
* @brief TBD 
* @param TBD 
* @retval TBD
*/
uint16_t twos_convert(uint16_t x);

/**
* @brief TBD 
* @param TBD 
* @retval TBD
*/
bool get_temperature_reading ( void );



#endif /* INC_SPI_H_ */
