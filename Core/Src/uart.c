/*
 * uart.c
 *
 *  Created on: May 12, 2023
 *      Author: clint
 */
#include "uart.h"

struct UARTMembers  uart; 

void print_float (float number, uint8_t action) {
    char temp_buffer[8];        //Define the array that will hold the ASCII values
    char c = '\0';

    /* USE SPRINT F TO BUILD THE ARRAY OF ASCII CHARACTERS */
    sprintf((char *)temp_buffer, "%.4f", number);   //f tells the function we want to print a float value


    HAL_UART_Transmit(&huart1, (uint8_t *)&temp_buffer, sizeof(temp_buffer), HAL_MAX_DELAY);


    /* CHECK TO SEE IF THE USER WISHES TO CREATE A NEW LINE */
    if(action == LF) {
        c = '\r';  HAL_UART_Transmit(&huart1,(uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
        c = '\n';  HAL_UART_Transmit(&huart1, (uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
    }
    else if(action == CR) {
        c = '\r';  HAL_UART_Transmit(&huart1,(uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
    }
}

void print_16b_binary_rep(uint16_t number, uint8_t action) {
    uint16_t i;
    uint16_t tx_char;
    char c = '\0';

    for(i = 16; i > 0; i--) {
        tx_char = (uint16_t)(number >> (i - 1));
        tx_char = (uint16_t)(tx_char & 0x01);
        tx_char = (uint16_t)(tx_char + 0x30);         //Turn this into an ASCII value
        c = (char)tx_char;

        HAL_UART_Transmit(&huart1, (uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
    }

    /* CHECK TO SEE IF THE USER WISHES TO CREATE A NEW LINE */
    if(action == LF) {
        c = '\r';  HAL_UART_Transmit(&huart1,(uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
        c = '\n';  HAL_UART_Transmit(&huart1, (uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
    }
    else if(action == CR) {
        c = '\r';  HAL_UART_Transmit(&huart1,(uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
    }
}

void print_unsigned_decimal (uint16_t number, uint8_t action) {
    char temphex[5];        //Define the array that will hold the ASCII values
    char c = '\r';
    uint8_t i;                
    uint8_t decimal_count;    //This is how many digits are written

    /* USE SPRINT F TO BUILD THE ARRAY OF ASCII CHARACTERS */
    decimal_count = (uint8_t)(sprintf(temphex, "%u", number)); //u tells the function we want an unsigned decimal number

    for(i = 0; i < decimal_count; i++) {    //Print out the array of ASCII characters.
        // TXREG1 = (temphex[i]);
        HAL_UART_Transmit(&huart1,(uint8_t *) &temphex[i], (uint16_t) 0x01, HAL_MAX_DELAY);
    }

    /* CHECK TO SEE IF THE USER WISHES TO CREATE A NEW LINE */
    if(action == LF) {
        HAL_UART_Transmit(&huart1,(uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
        c = '\n';  HAL_UART_Transmit(&huart1, (uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
    }
    else if(action == CR) {
        c = '\r';  HAL_UART_Transmit(&huart1,(uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
    }
}


void print_string(const char * s, uint8_t action) {
    char c = '\0';

    while(*s != '\0') {
        HAL_UART_Transmit(&huart1, (uint8_t *) s, (uint16_t) 0x01, HAL_MAX_DELAY);
        s++;
    }

    /* CHECK TO SEE IF THE USER WISHES TO CREATE A NEW LINE */
    if(action == LF) {
        c = '\r';  HAL_UART_Transmit(&huart1,(uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
        c = '\n';  HAL_UART_Transmit(&huart1, (uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
    }
    else if(action == CR) {
        c = '\r';  HAL_UART_Transmit(&huart1,(uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
    }

}

void ClearCursorUp( void ) {
    char c = 0x1B;
    HAL_UART_Transmit(&huart1,(uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
    print_string("[1J",0);        //Send the rest of the sequence to clear the screen

}

void clear_screen( void ) {
    char c = 0x1B;
    HAL_UART_Transmit(&huart1,(uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
    print_string("[2J",0);        //Send the rest of the sequence to clear the screen
}

void clear_line( void ) {
    char c = 0x1B;
    HAL_UART_Transmit(&huart1,(uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
    print_string("[2K",0);        //Send the rest of the sequence to clear the screen

}

void cursor_top_left( void ) {
    char c = 0x1B;
    HAL_UART_Transmit(&huart1,(uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
    print_string("[H",0);     //Send the rest of the sequence to clear the screen
}

void reset_terminal( void ) {
    char c = 0x1B;
    HAL_UART_Transmit(&huart1,(uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
    print_string("c",0);      //Send the rest of the sequence to clear the screen
}

void insert_line_feed( uint8_t line_feeds ) {
    uint8_t i = 0;         //Use this as a counter
    char c = '\r';
    
    for(i = 0; i < line_feeds; i++){
        HAL_UART_Transmit(&huart1,(uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
        c = '\n'; 
        HAL_UART_Transmit(&huart1,(uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
    }
}

void insert_line_separator( void ) {
    print_string("---------------------------------------", LF);
}

void reset_rx_buffer( void ) {

    uart.consumer_index = uart.producer_index = 0;                              //Reset the pointers
    uart.byte_counter = 0;                                            //Reset the data counter
    memset(uart.rxbuf,'\0',MAX_ELEMENTS);                           //Null out the buffer
} /* End of ResetRxBuffer */

