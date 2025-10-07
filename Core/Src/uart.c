/*
 * uart.c
 *
 *  Created on: May 12, 2023
 *      Author: clint
 */
#include "uart.h"

struct UARTMembers  uart; 

// void uart_print_float(float value, int decimals)
// {
//     char buf[64];
//     if (decimals < 0) decimals = 0;
//     if (decimals > 6) decimals = 6;  // keep it reasonable



//     char hello[] = "Float test\r\n";
//     HAL_UART_Transmit(&huart1, (uint8_t*)hello, sizeof hello - 1, HAL_MAX_DELAY);


//     // IMPORTANT: pass a double to %f (variadic promotion)
//     int n = snprintf(buf, sizeof buf, "%.*f", decimals, (double)value);
//     if (n < 0) return;                          // formatting error
//     if (n >= (int)sizeof buf) n = sizeof buf - 1; // truncated (still null-terminated)

//     HAL_UART_Transmit(&huart1, (uint8_t*)buf, (uint16_t)n, HAL_MAX_DELAY);
//     const char crlf[] = "\r\n";
//     HAL_UART_Transmit(&huart1, (uint8_t*)crlf, 2, HAL_MAX_DELAY);
// }

void print_float (float number, uint8_t action) {
    char temp_buffer[16];        //Define the array that will hold the ASCII values
    uint8_t c = '\0';

    /* USE SPRINT F TO BUILD THE ARRAY OF ASCII CHARACTERS */
    // int chars = snprintf((char *)temp_buffer, "%.4f", (double)number);   //f tells the function we want to print a float value

    int chars = snprintf(temp_buffer, sizeof temp_buffer, "%.4f", (double)number);

    // if (chars < 0)   //TODO we can remove this?
    // {
    //     return;            // encoding error  
    // } 
    // if (chars >= (int)sizeof(temp_buffer)) 
    // {  
    //     chars = sizeof(temp_buffer) - 1;
    // }
    
    HAL_UART_Transmit(&huart1, (uint8_t *) temp_buffer, (uint16_t)chars, HAL_MAX_DELAY);

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
    uint8_t c = '\0';

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
    char temphex[16];        //Define the array that will hold the ASCII values
    uint8_t c = '\r';
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
    uint8_t c = '\0';

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

