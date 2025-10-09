/**
  ******************************************************************************
  * @file           : console.c
  * @brief          : Implementation of command line interface 
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 Clinton Guenther
  * All rights reserved.
  *
  ******************************************************************************
  */

#include "console.h"            //Include file to support console
#include "uart.h"

struct UARTMembers uart;
struct TEMPMembers temp;


uint8_t get_uint8 ( void ) {
    int number      = 0;
    uint8_t timeout     = 0;

    reset_rx_buffer();
    
    uart.rxchar = '\0';
    while(uart.rxchar == '\0'){}                   // Wait for keyboard input 
    
    while (timeout < 20) {
        if(uart.rxchar == ENTER_KEY) break;

        blocking_ms_delay(1000);
        timeout++;
    }

    sscanf(uart.rxbuf,"%d",&number);

    print_string("Number Received: ",0);
    print_unsigned_decimal(number, LF);

    reset_rx_buffer();
    return(number);
}


float get_float( void ) {
    float number        = 0;
    uint8_t timeout     = 0;
    
    reset_rx_buffer();
    
    uart.rxchar = '\0';
    while(uart.rxchar == '\0'){}                   // Wait for keyboard input 
    
    while (timeout < 80) {
        
        if(uart.rxchar == ENTER_KEY) break;

        blocking_ms_delay(100);
        timeout++;
    }

    sscanf(uart.rxbuf,"%f.3",&number);
    
    print_string("Number Received: ",0);
    print_float(number, LF);
    
    reset_rx_buffer();

    return(number);

}

void console_menu( void ) 
{
    /*
     * This routine will be using the same buffer as what's 
     * used for the main application.  reset_rx_buffer() is called
     * in main.c just prior to entering this routine. 
     * 
     */

	uint8_t     usr_number_u8       = 0;        // Number user has entered will be stored here
    // float       temp_float          = 0.0;
    
    uart.rxchar = '\0';                  
    reset_terminal();                            // Clear all the contents on the terminal

    
    blocking_ms_delay(10);
    cursor_top_left();
    blocking_ms_delay(10);

    while(usr_number_u8 != 99) 
    {
        print_string("1 --- View HW version.",LF);
        print_string("2 --- View SW version.",LF);
        print_string("3 --- Report temperature values.",LF);
        print_string("       press \'x\' to quit.",LF);
        print_string("4 --- View temperature trip point.",LF);
        print_string("5 --- Enable fan.",LF);
        print_string("6 --- Disable the fan.",LF);
        
        print_string("99 -- Exit menu.",LF);  
        insert_line_feed(2);
        
        print_string("Enter Selection:  ",0);
        usr_number_u8 = get_uint8();

       
        switch(usr_number_u8) 
        {
            
            /**
             * View HW version
             */
            case 1:
            {
                uint8_t version = 0;    
                clear_screen();
                cursor_top_left();
                insert_line_separator();
                
                version = (HAL_GPIO_ReadPin(ID_B2_GPIO_Port, ID_B2_Pin) << 2) |
                (HAL_GPIO_ReadPin(ID_B0_GPIO_Port, ID_B0_Pin) << 1) |
                HAL_GPIO_ReadPin(ID_B0_GPIO_Port, ID_B0_Pin);
                
                
                print_string("HW vrsion: ", 0);
                print_unsigned_decimal(version, LF);
                break;
            }
            
            /**
             * View SW version
             */
            case 2:
                clear_screen();
                cursor_top_left();
                insert_line_separator();
                
                print_string("SW version",0);
                print_string(SW_VERSION,LF);
            break;
            
            
            /**
             * Report temperature values
             */
            case 3:
                clear_screen();
                cursor_top_left();
                insert_line_separator();
                
                
                uart.rxchar = '\0';
                while (uart.rxchar != 'x')
                {
                    uint8_t i=0;

                    if (get_temperature_reading () == false) {
                        print_string("Error retrieving temp value", LF);
                    }

                    print_string("Temp value: ", 0);
                    print_float(temp.flt_temp_value, LF);
                    
                    for(i=0;i<10;i++)
                    {
                        blocking_ms_delay(50);          //Max is 65535us
                    }

                }
                clear_screen();
                cursor_top_left();
            break;
            
            /**
             * View temperature trip point
             */
            case 4:
                clear_screen();
                cursor_top_left();
                insert_line_separator();
                
                print_string("Temperature rising trip point: ",0);
                print_unsigned_decimal(TEMP_TRIP_F_RISING,LF);
                insert_line_separator();
                print_string("Temperature falling trip point: ",0);
                print_unsigned_decimal(TEMP_TRIP_F_FALLING,LF);
            break;
            
            /**
             * Enable the fan
             */
            case 5:
                clear_screen();
                cursor_top_left();
                insert_line_separator();
                print_string("Enabling the fan",LF);

                HAL_GPIO_WritePin(FAN_EN_GPIO_Port, FAN_EN_Pin, GPIO_PIN_SET);
            break;
                
            /**
             * Disable the fan
             */
            case 6:
                clear_screen();
                cursor_top_left();
                insert_line_separator();
                print_string("Disabling the fan",LF);
                
                HAL_GPIO_WritePin(FAN_EN_GPIO_Port, FAN_EN_Pin, GPIO_PIN_RESET);
            break;

            case 99:
                reset_terminal();              //Leaving menu, so clear the screen so not to confuse user
                blocking_ms_delay(1);
                cursor_top_left();        //Make sure the cursor is in the Top Left position
                blocking_ms_delay(1);
                usr_number_u8 = 99;
            break;

            default:
                usr_number_u8 = 99;
        }   //END Switch(usr_number_u8)
    }  //END while(glbinfo.rxchar != 99)

    print_string("Leaving console.",LF);

}
