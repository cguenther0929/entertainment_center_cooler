/*
 * spi.c
 *
 *  Created on: Sep 14, 2025
 *      Author: C. Guenther
 */
#include "sensor.h"
#include "timer.h"


struct TEMPMembers temp;

uint16_t twos_convert(uint16_t x) 
{
    x ^= 0xFFFF;
    x += 0x0001;
    x &= 0x3FFF;    //Only care about the 14 temperature bits
    return(x);
}


bool get_temperature_reading( void )
{
    uint8_t i                   = 0;  
    int32_t i32_signed_temp_data   = 0;
    uint32_t raw                = 0;

    float temp_sum              = 0.0;
    float temp_value            = -0.0;

    uint8_t tx[4] = {0x00, 0x00, 0x00, 0x00};   // dummy bytes to clock data out
    uint8_t rx[4] = {0};
  
    /**
     * Drop the CS line low
     */
    HAL_GPIO_WritePin(TEMP_CS_GPIO_Port, TEMP_CS_Pin, GPIO_PIN_RESET);
    blocking_ms_delay(2);
    
    if(HAL_SPI_TransmitReceive(&hspi1, tx, rx, sizeof(rx), 10) != HAL_OK)  // 10 ms timeout. The first byte will land in rx[0]
    {
        return false;
    } 
    
    /**
     * Bring the CS pin back high
     */
    HAL_GPIO_WritePin(TEMP_CS_GPIO_Port, TEMP_CS_Pin, GPIO_PIN_SET);
    
    raw = ((uint32_t)rx[0] << 24) | ((uint32_t)rx[1] << 16)
    | ((uint32_t)rx[2] << 8)  |  ((uint32_t)rx[3]);

    /**
     * For debugging only
     */

    // char buf[64];
    // uint16_t chars = snprintf(buf, sizeof buf,
    //      "raw=0x%08" PRIX32 "  F=%u SCV=%u SCG=%u OC=%u\r\n",
    //      (uint32_t)raw,
    //      (unsigned)((raw >> 16) & 1),
    //      (unsigned)((raw >> 2)  & 1),
    //      (unsigned)((raw >> 1)  & 1),
    //      (unsigned)(raw & 1));
    
    // HAL_UART_Transmit(&huart1, (uint8_t *) buf, (uint16_t)chars, HAL_MAX_DELAY);


    /**
     * Check the temp fault bit
     */
    if (raw & 0x00010000UL) 
    {                   // FAULT bit set (bit16)
        uint8_t f = raw & 0x07;                 // bits 2..0
        if (f & 0x04) print_string("Fault: SCV (short to VCC)", LF);
        if (f & 0x02) print_string("Fault: SCG (short to GND)", LF);
        if (f & 0x01) print_string("Fault: OC  (open thermocouple)", LF);
        
        // // You can still read cold-junction temp if you want:
        return false;
    }

    /**
     * Only process this if we don't have a fault
     */
    i32_signed_temp_data = (int32_t)((raw >> 18) & 0x3FFF);     // sign-extend 
    temp_value = (float)(i32_signed_temp_data / 4.0);
    

    /**
     * Convert Celsius to Fahrenheit
     */
    temp_value = (float)((temp_value * 1.8) + 32);

    temp.temperature_buffer[temp.temp_buf_index] = temp_value;   
    (temp.temp_buf_index >= 7) ? (temp.temp_buf_index = 0):(temp.temp_buf_index++);

    temp_sum = 0.0;   // Reset this value in preparation to calculate the average
    
    for(i = 0; i < 8; i++) 
    {
        temp_sum += temp.temperature_buffer[i];
    }

    temp.flt_temp_value = (float)(temp_sum / 8);  

    return(true);

}
