#include "Arduino.h"

// Libraries required
#include <SPI.h>
#include <bitBangedSPI.h>

// Nick Gammon's library AMENDED BY ME if you want the 'switch off' extra function and to work in Eclipse
#include <MAX7219_Dot_Matrix.h>

// UPDATE THIS TO HOW MANY MAX7219 CHIPS ARE PRESENT
// THERE ARE FOUR CHIPS PER 4-IN-1 LED UNIT.
const byte chips = 4; // one units

// How long to keep the display running when no movement?
#define displayMinOnTime 10000

// 12 chips (display modules), hardware SPI with load on D10
MAX7219_Dot_Matrix display(chips, 10);  // Chips / LOAD

unsigned long lastMoved = 0;
unsigned long MOVE_INTERVAL = 30;  // mS
int messageOffset;

// In this example message you can use the tilde character ~ to show the degree character like this:
//unsigned char message[] = "New York 88~F London 75~F Benny is the best cat ever!";

// Or follow the example below to create your string with special characters
unsigned char message[100] = {};

// -----------------------------------------------------------------------------------------
// ROUTINES TO GET THE BRIGHTNESS AND SPEED FROM ANALOG PORTS A0 & A1 (10K POT)
// Wire up the potentiometers thus: +ve to one end, -ve to other end, analog pin to centre
// Note that the brightness control requires the updated library from Ralph S Bacon to work
// -----------------------------------------------------------------------------------------
void getDelay(unsigned long int *delayVar) {
	int delay = analogRead(14);
	*delayVar = map(delay, 0, 1023, 5, 75);
}

unsigned char getBrightness() {
	int bright = analogRead(15);
	return map(bright, 0, 1023, 0, 15);
}

// -----------------------------------------------------------------------------------------
// UPDATE DISPLAY WITH NEW MESSAGE
// -----------------------------------------------------------------------------------------
void updateDisplay() {

	display.sendSmooth(message, messageOffset);

	// next time show one pixel onwards
	if (messageOffset++ >= (int) (strlen((char *)message) * 8)) messageOffset = -chips * 8;
}  // end of updateDisplay

// -----------------------------------------------------------------------------------------
// SETUP	SETUP     SETUP     SETUP     SETUP     SETUP     SETUP     SETUP     SETUP
// -----------------------------------------------------------------------------------------
void setup() {

	display.begin();
	display.setIntensity(6); // initial brightness is moderate
	Serial.begin(9600);

  // message to be displayed
	String msg = "Hello PJ";

	// Convert the entire string to a character array for sending to LED
	msg.toCharArray((char*)message, 100);

	// Debug message (might not display 100% correctly!)
	Serial.println((char *)message);

}  // end of setup

// -----------------------------------------------------------------------------------------
// LOOP       LOOP     LOOP     LOOP     LOOP     LOOP     LOOP     LOOP     LOOP     LOOP
// -----------------------------------------------------------------------------------------
void loop() {

	static unsigned long int displayOnTime = millis();

	// update display if time is up
	if (millis() - lastMoved >= MOVE_INTERVAL) {
		updateDisplay();
		lastMoved = millis();
	}

	// New function: Set the delay between pixel moves
	getDelay(&MOVE_INTERVAL);

	// New function: Set the brightness (0=min, 15=max)
	display.setIntensity(getBrightness());
}  // end of loop
