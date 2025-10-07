/*
 * timer.c
 *
 *  Created on: Nov 12, 2021
 *      Author: C. Guenther
 */
#include "timer.h"

struct timing       time;

//TODO need to clean up the following

void init_timer ( void ) {
//   time.led_fast_blink = false; //TODO remove?  
  time.flag_10ms_tick = false;
  time.flag_100ms_tick = false;
  time.flag_500ms_tick = false;
  time.ticks10ms = 0;
  time.ticks100ms = 0;
  time.ticks500ms = 0;

}

void blocking_us_delay (uint16_t us) {
    __HAL_TIM_SET_COUNTER(&htim2,0);    // set the counter value a 0
    while (__HAL_TIM_GET_COUNTER(&htim2) < us);  // Ticks are in us
}

void blocking_ms_delay(uint32_t ms)
{
    uint32_t start = __HAL_TIM_GET_COUNTER(&htim2);     // µs counter
    uint32_t wait  = ms * 1000U;                         // total µs

    // works across wrap-around because of unsigned subtraction
    while ((uint32_t)(__HAL_TIM_GET_COUNTER(&htim2) - start) < wait) {
        __NOP();
    }
}

// void start_us_counter ( void ) {
// 	__HAL_TIM_SET_COUNTER(&htim3,0);    // set the counter value a 0
// 	HAL_TIM_Base_Start(&htim3);
// }

// void stop_us_counter ( void ) {
// 	HAL_TIM_Base_Stop(&htim3);
// }

// uint16_t get_us_counter ( void ) {
//     return (__HAL_TIM_GET_COUNTER(&htim3));
// }

