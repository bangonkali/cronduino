#include <LiquidCrystal.h>
#include <Wire.h>
#include "RTClib.h"
#include <EEPROM.h>

/*******************************************************************************************************
 * Scheduly Entry Definition
 *******************************************************************************************************/
typedef struct {
	unsigned char en; 
	unsigned char id;
	unsigned char ds;
	unsigned char mnl ;
	unsigned char mnc ;
	unsigned char mnu ;
	unsigned char hhl ;
	unsigned char hhc ;
	unsigned char hhu ;
	unsigned char mdl ;
	unsigned char mdc ;
	unsigned char mdu ;
	unsigned char wdl ;
	unsigned char wdc ;
	unsigned char wdu ;
	unsigned char mol ;
	unsigned char moc ;
	unsigned char mou ;
	unsigned char yyl ;
	unsigned char yyc ;
	unsigned char yyu ;
} EntryStruct ;

enum CLASSDATA {
	ASTERISK = 255,
	DASH = 254,
	SLASH = 253,
	ON = 252,
	OFF = 251,
	EMPTY = 250
};

void GetEntry(unsigned char *entry, EntryStruct *entry_s);
unsigned char GetDeviseFinalState(unsigned char deviceCommState);
unsigned char ActivateEntry(EntryStruct *entry, DateTime time);

/*******************************************************************************************************
 * RTC Definition
 *******************************************************************************************************/
RTC_DS1307 RTC;
DateTime now;

/*******************************************************************************************************
 * LCD Definition
 *******************************************************************************************************/
LiquidCrystal lcd(8, 7, 6, 5, 4, 3, 2);

/*******************************************************************************************************
 * Output Pins Array
 *******************************************************************************************************/
int ledPins[] = {0,1,9,10,11,12,13};

/*******************************************************************************************************
 * Global Configuration
 *******************************************************************************************************/
int i;
int delay_valu = 100;

bool read_flag = false;
int eeprom_value = 0;

/*******************************************************************************************************
 * Initialization
 *******************************************************************************************************/
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

/*******************************************************************************************************
 * Main Loop
 *******************************************************************************************************/
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

/*******************************************************************************************************
 * Schedult Entry Implementation
 *******************************************************************************************************/
void GetEntry(unsigned char *entry, EntryStruct *entry_s) {
	entry_s->en = entry[0]; // Enable
	entry_s->id = entry[1]; // DeviceID
	entry_s->ds = entry[2]; // DeviceState
	entry_s->mnl = entry[3]; // MinutesLower
	entry_s->mnc = entry[4]; // MinutesClassifier
	entry_s->mnu = entry[5]; // MinutesUpper
	entry_s->hhl = entry[6]; // HourLower
	entry_s->hhc = entry[7]; // HourClassifier
	entry_s->hhu = entry[8]; // HourUpper
	entry_s->mdl = entry[9]; // MonthDayLower
	entry_s->mdc = entry[10]; // MonthDayClassifier
	entry_s->mdu = entry[11]; // MonthDayUpper
	entry_s->wdl = entry[12]; // WeekdayLower
	entry_s->wdc = entry[13]; // WeekdayClassifier
	entry_s->wdu = entry[14]; // WeekdayUpper
	entry_s->mol = entry[15]; // MonthLower
	entry_s->moc = entry[16]; // MonthClassifier
	entry_s->mou = entry[17]; // MonthUpper
	entry_s->yyl = entry[18]; // YearLower
	entry_s->yyc = entry[19]; // YearClassifier
	entry_s->yyu = entry[20]; // YearUpper
}

unsigned char GetDeviseFinalState(unsigned char deviceCommState) {
	if (deviceCommState == ON) {
		return 1;
	} else {
		return 0;
	}
}

unsigned char ActivateEntry(EntryStruct *entry, DateTime time) {
	// only work with enabled schedules
	if (entry->en == 1) {
		if (entry->mnc == DASH) {
			if (time.minute() > entry->mnu && time.minute() < entry->mnl) {
				return 0;
			}
		}
		if (entry->mnl != time.minute() && entry->mnl != ASTERISK) {
			return 0;
		}


		if (entry->hhc == DASH) {
			if (time.hour() > entry->hhu && time.hour() < entry->hhl) {
				return 0;
			}
		}
		if (entry->hhl != time.hour() && entry->hhl != ASTERISK) {
			return 0;
		}


		if (entry->mdc == DASH) {
			if (time.day() > entry->mdu && time.day() < entry->mdl) {
				return 0;
			}
		}
		if (entry->mdl != time.day() && entry->mdl != ASTERISK) {
			return 0;
		}


		if (entry->wdc == DASH) {
			if (time.dayOfWeek() > entry->wdu && time.dayOfWeek() < entry->wdl) {
				return 0;
			}
		}
		if (entry->wdl != time.dayOfWeek() && entry->wdl != ASTERISK) {
			return 0;
		}


		if (entry->moc == DASH) {
			if (time.month() > entry->mou && time.month() < entry->mol) {
				return 0;
			}
		}
		if (entry->mol != time.month() && entry->mol != ASTERISK) {
			return 0;
		}


		if (entry->yyc == DASH) {
			if (time.year() > entry->yyu && time.year() < entry->yyl) {
				return 0;
			}
		}
		if (entry->yyl != time.year() && entry->yyl != ASTERISK) {
			return 0;
		}

		switch (entry->id) {
			case ASTERISK:
				if (GetDeviseFinalState(entry->ds)) {
					for (unsigned char i = 0; i<7; i++ ) {
						digitalWrite(ledPins[i], 1);
					}
				} else {
					for (unsigned char i = 0; i<7; i++ ) {
						digitalWrite(ledPins[i], 0);
					}
				}
				break;
			default:
				digitalWrite(ledPins[entry->id], GetDeviseFinalState(entry->ds));
				break;
		}

		return 1;
	} else {
		return 0;
	}
}