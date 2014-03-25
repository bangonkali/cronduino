#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <Wire.h>
#include "RTClib.h"
#include "Entry.h"

/*******************************************************************************************************
 * RTC Definition
 *******************************************************************************************************/
RTC_DS1307 RTC; // the object instantiation of the RTC_DS1307 Hardware based realtime clock
DateTime now;  // used for printing in to the LCD the current time and date.

/*******************************************************************************************************
 * LCD Definition
 *******************************************************************************************************/
LiquidCrystal lcd(8, 7, 6, 5, 4, 3, 2); // has been designed in hardware in this particular order.

/*******************************************************************************************************
 * Output Pins Array
 *******************************************************************************************************/
int ledPins[] = {1, 0, 13, 12, 11, 10, 9}; // has been designed in hardware in this particular order.

/*******************************************************************************************************
 * Global Configuration
 *******************************************************************************************************/
int i; // re-usable global iterator :D
bool read_flag = false; 
int eeprom_value = 0;
bool SerialMode = true;

volatile unsigned char state = 0;
unsigned char terminator_buffers[3];
unsigned char entries[1024];
unsigned char current_entry[24];
unsigned char current_entries = 0;

char display_buffer[16];
int entry_bytes_counter = 0;

ScheduleEntry _ScheduleEntry;

int current_second = 0;
int previous_second = 0;

/*******************************************************************************************************
 * Initialization
 *******************************************************************************************************/
void setup()
{
	Wire.begin();
	RTC.begin();
	RTC.adjust(DateTime(__DATE__, __TIME__)); // set the date and time to compilation time
	

	pinMode(A3, INPUT); // sets whether we are at serial mode (0) or scheduler mode (1)
	lcd.begin(16, 2); // set up the LCD's number of columns and rows
	
	// Check if in serial mode or not
	if (digitalRead(A3) == 1) {
		// Serial.begin(56700);

		SerialMode = false;
		for (i = 0; i < (sizeof(ledPins)/sizeof(int)); i++) {
			//Serial.println("Set pin[%d] as output.", ledPins[i]);
			pinMode(ledPins[i], OUTPUT);      // sets the digital pin as output
		}

		if (! RTC.isrunning()) {
			lcd.print("RTC is NOT running!");
			// following line sets the RTC to the date & time this sketch was compiled
			
		} else {
			lcd.print("RTC OK!");
		}

		delay(2000);

		// load eeprom data
		for(int i=0; i<1024; i++){
		    entries[i] = EEPROM.read(i);
		}


		lcd.setCursor(0, 1);
		lcd.print("EEPROM LOADED!");

		delay(2000);

	} else {
		SerialMode = true;
		Serial.begin(9600);

		lcd.clear();
		lcd.print("Serial Mode");
	}
}

void serialEvent() {
	while (Serial.available()) {
		// get the new byte:
		unsigned char inChar = (char)Serial.read(); 

		if (state == 0) {
			if (inChar == 'B') {
				state = 1;
			}
		} else if (state == 1) {
			if (inChar == 'E') {
				state = 2;
			}
		} else if (state == 2) {
			if (inChar == 'G') {
				entry_bytes_counter = 0;
				state = 3;				
			}
		} else if (state == 3) {
			entries[entry_bytes_counter] = inChar;

			terminator_buffers[0] = entries[entry_bytes_counter - 2];
			terminator_buffers[1] = entries[entry_bytes_counter - 1];
			terminator_buffers[2] = inChar;
			terminator_buffers[3] = '\0';

			entry_bytes_counter++;

			if (terminator_buffers[0] == 'E' && terminator_buffers[1] == 'N' && terminator_buffers[2] == 'D') {
				state = 5;
			}			
		}
	}
}

/*******************************************************************************************************
 * Main Loop
 *******************************************************************************************************/
void loop()
{
	if (!SerialMode) { // scheduler mode
		// load eeprom data

		// save current sec
		now = RTC.now();
		current_second = now.second();

		if (current_second != previous_second) {
			// total entries = (1024 eeprom total bytes)/(24 bytes per entry)
			int k = 0; // current eeprom address
			for(int i=0; i<42; i++){  // current eeprom entry

				// get current entry
				for(int j=0; j<24; j++){ // current entry address
					current_entry[j] = entries[k];
				    k++;

				    // lcd.clear();
				    // lcd.print(j);
				    // lcd.setCursor(0, 1);
				    // lcd.print(current_entry[j]);
				    // delay(1000);
				}

				// check the current entry
				if (isEmpty(current_entry)) {
					break;
				} else {
					current_entries++;
					
					// activate current entry
					_ScheduleEntry.SetEntry(current_entry);
					_ScheduleEntry.ActivateEntry(RTC);
				}
			}

			// slower refresh rate
			// if (millis() % 1000 < 150) {
			// 	printTime();
			// 	lcd.setCursor(10, 1);
			// 	lcd.print("SCHD");

			// 	lcd.setCursor(15, 1);
			// 	lcd.print(current_entries);
			// }
				
			printTime();
			lcd.setCursor(9, 1);
			lcd.print("SCHD");

			lcd.setCursor(14, 1);
			lcd.print(current_entries);

			current_entries = 0;
		}

		previous_second = current_second;

	} else { // serialMode
		if (state == 3) {
			lcd.clear();
			lcd.print("LOADING");
		} else if (state == 5) {

			for(int i=entry_bytes_counter-4; i<1024; i++){
			    entries[i] = ScheduleEntry::EMPTY;
			}

			for(int i=0; i<1024; i++){
			    EEPROM.write(i, entries[i]);
			}

			lcd.clear();
			lcd.print("FINISHED LOADING");

			sprintf (display_buffer, "BYTES: %4d", entry_bytes_counter-3);
			lcd.setCursor(0, 1);
			lcd.print(display_buffer);

			state = 0;
			entry_bytes_counter = 0;
		}
	}
}


void set_schedule(unsigned char sec, unsigned char state, unsigned char pin) {
	unsigned char entry[24];

	/*******************************************************************************************************
	 * Schedule for turning on
	 *******************************************************************************************************/
	entry[0]  = 1; // Enable
	entry[1]  = pin; // DeviceID
	entry[2]  = state; // DeviceState
	entry[3]  = sec; // SecondLower
	entry[4]  = ScheduleEntry::EMPTY; // SecondClassifier
	entry[5]  = 30; // SecondUpper
	entry[6]  = ScheduleEntry::ASTERISK; // MinutesLower
	entry[7]  = 0; // MinutesClassifier
	entry[8]  = 0; // MinutesUpper
	entry[9]  = ScheduleEntry::ASTERISK; // HourLower
	entry[10] = 0; // HourClassifier
	entry[11] = 0; // HourUpper
	entry[12] = ScheduleEntry::ASTERISK; // MonthDayLower
	entry[13] = 0; // MonthDayClassifier
	entry[14] = 0; // MonthDayUpper
	entry[15] = ScheduleEntry::ASTERISK; // WeekdayLower
	entry[16] = 0; // WeekdayClassifier
	entry[17] = 0; // WeekdayUpper
	entry[18] = ScheduleEntry::ASTERISK; // MonthLower
	entry[19] = 0; // MonthClassifier
	entry[20] = 0; // MonthUpper
	entry[21] = ScheduleEntry::ASTERISK; // YearLower
	entry[22] = 0; // YearClassifier
	entry[23] = 0; // YearUpper

	ScheduleEntry _ScheduleEntry(entry);
	_ScheduleEntry.ActivateEntry(RTC);
}

bool isEmpty(unsigned char *data) {
	for(int i=0; i<24; i++){
	    if (data[i] != ScheduleEntry::EMPTY) {
	    	return false;
	    }
	}

	return true;
}

void printTime() {
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
}