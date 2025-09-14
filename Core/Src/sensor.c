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
    uint16_t signed_temp_data   = 0;
    uint32_t raw                = 0;

    float temp_sum              = 0.0;
    float temp_value            = -0.0;

    uint8_t tx[4] = {0x00, 0x00, 0x00, 0x00};   // dummy bytes to clock data out
    uint8_t rx[4] = {0};
  
  
//   delay(1);                   //Delay in ms
//   SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
//   digitalWrite(SPI_CS,0);     //Drop CS to enable the transaction
//   delay(10);                   //Delay in ms

    /**
     * Drop the CS line low
     */
    HAL_GPIO_WritePin(TEMP_CS_GPIO_Port, TEMP_CS_Pin, GPIO_PIN_RESET);
    blocking_ms_delay(2);
    
    if(HAL_SPI_TransmitReceive(&hspi1, tx, rx, sizeof(rx), 10) != HAL_OK)  // 10 ms timeout
    {
        return false;
    } 
    
    /**
     * Bring the CS pin back high
     */
    HAL_GPIO_WritePin(TEMP_CS_GPIO_Port, TEMP_CS_Pin, GPIO_PIN_SET);
    
    
    raw = ((uint32_t)rx[0] << 24) | ((uint32_t)rx[1] << 16)
    | ((uint32_t)rx[2] << 8)  |  (uint32_t)rx[3];
    
    signed_temp_data = (uint16_t)((raw >> 18) & 0x3FFF);     // Going to pack the result as an integer
    
    if(signed_temp_data & (0x2000)) 
    {
        signed_temp_data = twos_convert(signed_temp_data);
        temp_value = (float)(signed_temp_data / -4.0);
    }
    else 
    {
        temp_value = (float)(signed_temp_data / 4.0);
    }
    
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


    
    
    // raw_therm_reading = SPI.transfer(0x00);                               //Reads bits 31:24
    // raw_therm_reading = (raw_therm_reading << 8) | SPI.transfer(0x00);    //Read bits 23:16
    // raw_therm_reading = (raw_therm_reading << 8) | SPI.transfer(0x00);    //Read bits 15:8
    // raw_therm_reading = (raw_therm_reading << 8) | SPI.transfer(0x00);    //Read bits 7:0
    
    // SPI.endTransaction();
    // delay(1);                   //Delay in ms
    // digitalWrite(SPI_CS,1);     //CS back high now that SPI transaction is over


    // signed_temp_data = (int)((raw_therm_reading >> 18) & 0x3FFF);     //Going to pack the result as an integer 

    // if(signed_temp_data & (0x2000)) 
    // {
    // signed_temp_data = twos_convert(signed_temp_data);
    // temp_value = float(signed_temp_data/-4.0);
    // }
    // else 
    // {
    // temp_value = float(signed_temp_data / 4.0);
    // }

    // /* Convert Celsius to Fahrenheit */
    // temp_value = float((temp_value * 1.8) + 32);

    // if(just_rebooted)
    // {
    // if(ENABLE_LOGGING)
    // {
    //     Serial.println("^Just rebooted flag is set to true.");
    // }
    // this -> just_rebooted = false;
    // for(i=0;i++;i<8)
    // {
    //     this -> temperature_buffer[this -> temp_buf_index] = temp_value;   
    //     (this -> temp_buf_index >= 7) ? (this -> temp_buf_index = 0):(this -> temp_buf_index++);
    // }
    // }

    // else
    // {
    // this -> temperature_buffer[this -> temp_buf_index] = temp_value;   
    // (this -> temp_buf_index >= 7) ? (this -> temp_buf_index = 0):(this -> temp_buf_index++);
    // }


    // temp_sum = 0.0;   // Reset this value in preparation to calculate the average
    // for(i=0; i<8; i++) 
    // {
    // temp_sum += this -> temperature_buffer[i];
    // }

    // temp_value = float(temp_sum / 8);  


    // return(temp_value);

}
