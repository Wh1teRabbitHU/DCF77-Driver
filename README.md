- [DCF77-Driver](#dcf77-driver)
	- [General usage](#general-usage)

# DCF77-Driver

DCF77 driver, written in C. Primarily made for STM32, but it can be also used with any other MCU-s.

## General usage

You should copy the content of the src folder into your project. If you are using an STM32 MCU, then you need to include the ```dc77-driver.h``` file in your main source file. To start receiving data first you need to call the ```DCF77_enable()``` function. This will let the code process the signal and after hitting the 1 minute mark, storing the time object. The other, main function that you need to place in your interrupt function is the ```void DCF77_handleInterrupt(uint32_t currentTick, uint8_t pinState);```. You have to provide two input parameters:

- currentTick is a timestamp stored in milliseconds. (STM32 HAL has the following function: ```HAL_GetTick();```)
- pinState is the incomming signal pin's current state (HIGH or LOW)

The interrupt should call this function when your pin is changing state (HIGH to LOW and LOW to HIGH). After 60-120 seconds you can request your first date time data. (Only if the signal is strong enough) You can monitor the state of the receiver by calling the ```uint8_t DCF77_timeReceived();``` method, if it returns 1, then you can use your new value. After retrieving the value, you should call the ```void DCF77_reset();``` function, which reset both the receiver logic and the timeReceived flag. If you don't need an other date value, then you can call the ```void DCF77_disable();``` function to stop the processing of the signal.
