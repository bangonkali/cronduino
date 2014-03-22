#include <LiquidCrystal.h>
#include <Wire.h>
#include "RTClib.h"
#include <EEPROM.h>

// Set ID to RTC.
RTC_DS1307 RTC;

// Pins for LCD
LiquidCrystal lcd(8, 7, 6, 5, 4, 3, 2);
DateTime now;

// Pins that are used for output
int ledPins[] = {0,1,9,10,11,12,13};
int i;
int delay_valu = 100;

bool read_flag = false;
int eeprom_value = 0;

void setup()
{
	for (i = 0; i < (sizeof(ledPins)/sizeof(int)); i++) {
		//Serial.println("Set pin[%d] as output.", ledPins[i]);
		pinMode(ledPins[i], OUTPUT);      // sets the digital pin as output
	}

	// set up the LCD's number of columns and rows: 
	lcd.begin(16, 2);

	// Serial.begin(57600);
	Wire.begin();
	RTC.begin();

	if (! RTC.isrunning()) {
		lcd.print("RTC is NOT running!");
		// following line sets the RTC to the date & time this sketch was compiled
		
	} else {
		lcd.print("RTC is running!");
		delay(2000);
		lcd.clear();
	}

	RTC.adjust(DateTime(__DATE__, __TIME__));
	
}

void loop()
{
	for (i = 0; i < (sizeof(ledPins)/sizeof(int)); i++) {
		//Serial.println("Set pin[%d] as output.", ledPins[i]);
		digitalWrite(ledPins[i], LOW);    // sets the LED off
	}

	delay(delay_valu);                  // waits for a second

	for (i = 0; i < (sizeof(ledPins)/sizeof(int)); i++) {
		//Serial.println("Set pin[%d] as output.", ledPins[i]);
		digitalWrite(ledPins[i], HIGH);    // sets the LED off
	}

	delay(delay_valu);                  // waits for a second
	now = RTC.now();

	lcd.clear();

	lcd.setCursor(0, 0);
    lcd.print(now.year(), DEC);
    lcd.print('/');
    lcd.print(now.month(), DEC);
    lcd.print('/');
    lcd.print(now.day(), DEC);

    lcd.setCursor(0, 1);
    lcd.print(now.hour(), DEC);
    lcd.print(':');
    lcd.print(now.minute(), DEC);
    lcd.print(':');
    lcd.print(now.second(), DEC);

    if (!read_flag) {
	    lcd.setCursor(10, 1);
	    EEPROM.write(1023, 103);
	    eeprom_value = EEPROM.read(1023);
	    lcd.print(eeprom_value, DEC);
	    read_flag = true;
	} else {
		lcd.setCursor(10, 1);
		lcd.print(eeprom_value, DEC);
	}
}