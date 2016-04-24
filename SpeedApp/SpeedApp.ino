/***************************************************
1,1059a    tid  10:59 a end
2,60a      Puls, 60, a end
3,225a     fart  22.5 a end

Obs:  Serial.parseInt() parsar byte till end (a)

****************************************************/

#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

Adafruit_7segment matrix = Adafruit_7segment();
volatile int rpmcount = 0;
int rpmPin = 2;
int rpm = 0;
unsigned long lastmillis = 0;
double distance = 0.0;
double ms = 0.0;
double constSpeed = 16.666666667;
 
void setup() {
#ifndef __AVR_ATtiny85__
	Serial.begin(9600);
	Serial.println("MinTravApp 2");
#endif
	matrix.begin(0x70);
	matrix.print(0, DEC);
	matrix.blinkRate(1);
	matrix.writeDisplay();

	pinMode(rpmPin, INPUT_PULLUP);

	attachInterrupt(0, rpm_fan, FALLING);

}

void loop() {

	if (millis() - lastmillis >= 1000) {

		detachInterrupt(0);
		rpm = rpmcount * 60;

		distance = rpmcount * 1.9;
		ms = distance;

		double res = ms *  constSpeed;

		res = (res - int(res)) * 60 * 10;

		int valueToPrint = int(res);

		matrix.blinkRate(0);
		matrix.writeDisplay();

		matrix.writeDigitRaw(2, 0x10);
		matrix.writeDigitNum(0, -1, false);
		matrix.writeDigitNum(1, (valueToPrint / 100) % 10, false);
		matrix.writeDigitNum(3, (valueToPrint / 10) % 10, false);
		matrix.writeDigitNum(4, valueToPrint % 10, false);
		matrix.writeDisplay();


		Serial.println(ms, DEC);

		rpmcount = 0; // Restart the RPM counter
		lastmillis = millis(); // Uptade lasmillis
		attachInterrupt(0, rpm_fan, FALLING); //enable interrupt
	}




	if (Serial.available() > 0) {

		matrix.blinkRate(0);
		matrix.writeDisplay();

		int commando = Serial.parseInt();
		int valueToPrint = Serial.parseInt();

		if (Serial.read() == 'a') {



			//Tid
			if (commando == 1) {
				matrix.writeDigitNum(0, (valueToPrint / 1000), true);
				matrix.writeDigitNum(1, (valueToPrint / 100) % 10, true);
				matrix.drawColon(true);
				matrix.writeDigitNum(3, (valueToPrint / 10) % 10, true);
				matrix.writeDigitNum(4, valueToPrint % 10, true);
				matrix.writeDisplay();
			}
			//Puls
			else if (commando == 2) {
				matrix.print(valueToPrint, DEC);
				matrix.writeDisplay();

			}
			//min/km
			else if (commando == 3) {

				matrix.writeDigitRaw(2, 0x10);
				matrix.writeDigitNum(0, -1, false);
				matrix.writeDigitNum(1, (valueToPrint / 100) % 10, false);
				matrix.writeDigitNum(3, (valueToPrint / 10) % 10, false);
				matrix.writeDigitNum(4, valueToPrint % 10, false);
				matrix.writeDisplay();

			}

		}

	}

	delay(10);

}

void rpm_fan() {
	rpmcount++;


}
