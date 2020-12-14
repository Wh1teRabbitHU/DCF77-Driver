#include "dcf77-driver.h"

/**
 * Variables
 */

static uint8_t receiverEnabled = 0;
static uint8_t timeReceived = 0;
static uint32_t previousStart = 0;
static uint32_t timeElapsed = 0;
static uint8_t signalIndex = 0;
static uint16_t signalLength = 0;
static uint8_t signals[60] = {0};
static DCF77_dateTime_t lastTimeValue = {0};

/**
 * Private methods
 */

uint8_t getMinutes() {
	uint8_t minutes = 0;

	minutes += signals[27] ? 40 : 0;
	minutes += signals[26] ? 20 : 0;
	minutes += signals[25] ? 10 : 0;
	minutes += signals[24] ? 8 : 0;
	minutes += signals[23] ? 4 : 0;
	minutes += signals[22] ? 2 : 0;
	minutes += signals[21] ? 1 : 0;

	return minutes;
}

uint8_t getHours() {
	uint8_t hours = 0;

	hours += signals[34] ? 20 : 0;
	hours += signals[33] ? 10 : 0;
	hours += signals[32] ? 8 : 0;
	hours += signals[31] ? 4 : 0;
	hours += signals[30] ? 2 : 0;
	hours += signals[29] ? 1 : 0;

	return hours;
}

uint8_t getDayOfMonth() {
	uint8_t dayOfMonth = 0;

	dayOfMonth += signals[41] ? 20 : 0;
	dayOfMonth += signals[40] ? 10 : 0;
	dayOfMonth += signals[39] ? 8 : 0;
	dayOfMonth += signals[38] ? 4 : 0;
	dayOfMonth += signals[37] ? 2 : 0;
	dayOfMonth += signals[36] ? 1 : 0;

	return dayOfMonth;
}

uint8_t getDayOfWeek() {
	uint8_t dayOfWeek = 0;

	dayOfWeek += signals[44] ? 4 : 0;
	dayOfWeek += signals[43] ? 2 : 0;
	dayOfWeek += signals[42] ? 1 : 0;

	return dayOfWeek;
}

uint8_t getMonths() {
	uint8_t month = 0;

	month += signals[49] ? 10 : 0;
	month += signals[48] ? 8 : 0;
	month += signals[47] ? 4 : 0;
	month += signals[46] ? 2 : 0;
	month += signals[45] ? 1 : 0;

	return month;
}

uint8_t getYears() {
	uint8_t years = 0;

	years += signals[57] ? 80 : 0;
	years += signals[56] ? 40 : 0;
	years += signals[55] ? 20 : 0;
	years += signals[54] ? 10 : 0;
	years += signals[53] ? 8 : 0;
	years += signals[52] ? 4 : 0;
	years += signals[51] ? 2 : 0;
	years += signals[50] ? 1 : 0;

	return years;
}

void DCF77_succeded() {
	signalIndex = 0;
	timeReceived = 1;
}

void DCF77_signalReceived(uint16_t length, uint16_t elapsed) {
	if (signalIndex < 59) {
		signals[signalIndex] = length < 150 ? 0 : 1;
		signalIndex++;
	} else {
		lastTimeValue.minutes = getMinutes();
		lastTimeValue.hours = getHours();
		lastTimeValue.dayOfMonth = getDayOfMonth();
		lastTimeValue.dayOfWeek = getDayOfWeek();
		lastTimeValue.months = getMonths();
		lastTimeValue.years = getYears();

		DCF77_succeded();
	}
}

uint8_t DCF77_isMinuteStarted(uint16_t elapsed) {
	return signalIndex > 0 || elapsed > 1600 ? 1 : 0;
}

/**
 * Public methods
 */

void DCF77_enable() {
	receiverEnabled = 1;
}

void DCF77_disable() {
	receiverEnabled = 0;
}

void DCF77_reset() {
	signalIndex = 0;
	timeReceived = 0;
}

void DCF77_handleInterrupt(uint32_t currentTick, uint8_t pinState) {
	if (receiverEnabled == 0) {
		return;
	}

	if (previousStart > 0) {
		if (pinState == PIN_STATE_HIGH) {
			timeElapsed = currentTick - previousStart;
			previousStart = currentTick;
		} else {
			signalLength = currentTick - previousStart;

			if (DCF77_isMinuteStarted(timeElapsed)) {
				DCF77_signalReceived(signalLength, timeElapsed);
			}
		}
	} else {
		previousStart = currentTick;
	}
}

uint8_t DCF77_timeReceived() {
	return timeReceived;
}

DCF77_dateTime_t * DCF77_lastTimeValue() {
	return &lastTimeValue;
}
