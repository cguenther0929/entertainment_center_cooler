# Entertainment Center Cooler Controller (STM32) 

## Architecture
Bare metal -- application written in C.       

## Dependencies
* STM32Cube IDE: v1.6.1
   

## Tagged Versions 
* v0.0.1 -- This version compiles, but there is a lot of code to add.  
* v0.0.2 -- This version compiles.  Some debugging occurred.  The blocking delay functions seem to work, and the LED blinks.
* v0.0.3 -- This version compiles.  We are able to measure temperature.  The console functions well.  Timers and sys tick appear to be working well.  
* v0.1.0 -- This probably a release candidate.  We might want to reduce the blink count.  
* v0.1.1 -- The blink count was reduced.  We only blink when transitioning, otherwise the LED is dark.  
* v0.2.0 -- The temperature threshold was brought down to 85, and the hysteresis was taken down to one degree.  