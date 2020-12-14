#ifndef INC_DCF77_RECEIVER_H_
#define INC_DCF77_RECEIVER_H_

#include <stdint.h>

typedef struct DCF77_dateTime_t {
	uint8_t minutes;
	uint8_t hours;
	uint8_t dayOfMonth;
	uint8_t dayOfWeek;
	uint8_t months;
	uint8_t years;
} DCF77_dateTime_t;

#define PIN_STATE_HIGH 1
#define PIN_STATE_LOW 0

void DCF77_enable();
void DCF77_disable();
void DCF77_reset();
void DCF77_handleInterrupt(uint32_t currentTick, uint8_t pinState);
uint8_t DCF77_timeReceived();
DCF77_dateTime_t* DCF77_lastTimeValue();

#endif
