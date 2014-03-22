/*
  Entry.cpp - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#include "Entry.h"
#include <RTClib.h>
#include "Arduino.h"

/*******************************************************************************************************
 * Schedult Entry Implementation
 *******************************************************************************************************/
ScheduleEntry::ScheduleEntry(const uint8_t *entry) {
	this->SetEntry(entry);
};

uint8_t ScheduleEntry::SetEntry(const uint8_t *entry) {
	// entry is the input initial address in memory taken from the eeprom
	// entry_s is the return value which is the arranged value.

	en  = entry[0]; // Enable
	id  = entry[1]; // DeviceID
	ds  = entry[2]; // DeviceState
	ssl = entry[3]; // YearLower
	ssc = entry[4]; // YearClassifier
	ssu = entry[5]; // Ye
	mnl = entry[6]; // MinutesLower
	mnc = entry[7]; // MinutesClassifier
	mnu = entry[8]; // MinutesUpper
	hhl = entry[9]; // HourLower
	hhc = entry[10]; // HourClassifier
	hhu = entry[11]; // HourUpper
	mdl = entry[12]; // MonthDayLower
	mdc = entry[13]; // MonthDayClassifier
	mdu = entry[14]; // MonthDayUpper
	wdl = entry[15]; // WeekdayLower
	wdc = entry[16]; // WeekdayClassifier
	wdu = entry[17]; // WeekdayUpper
	mol = entry[18]; // MonthLower
	moc = entry[19]; // MonthClassifier
	mou = entry[20]; // MonthUpper
	yyl = entry[21]; // YearLower
	yyc = entry[22]; // YearClassifier
	yyu = entry[23]; // Ye

	return 0;
};

uint8_t ScheduleEntry::GetDeviseFinalState(uint8_t deviceCommState) {
	if (deviceCommState == ON) {
		return 1;
	} else {
		return 0;
	}
};

uint8_t ScheduleEntry::ActivateEntry(RTC_DS1307 RTC) {
	// only work with enabled schedules
	static int devices[7] = {1, 0, 13, 12, 11, 10, 9};
	static uint8_t pins = 7;

	time = RTC.now();
	// Serial.println("Contents of entry:");
	// for (int i = 0 ; i < 24; i++) {
	// 	Serial.println(analogValue, DEC);
	// }

	if (en == 1) {
		if (ssc == DASH) {
			if (time.second() > ssu && time.second() < ssl) {
				return 0;
			}
		}
		if (ssl != time.second() && ssl != ASTERISK) {
			return 0;
		}

		if (mnc == DASH) {
			if (time.minute() > mnu && time.minute() < mnl) {
				return 0;
			}
		}
		if (mnl != time.minute() && mnl != ASTERISK) {
			return 0;
		}


		if (hhc == DASH) {
			if (time.hour() > hhu && time.hour() < hhl) {
				return 0;
			}
		}
		if (hhl != time.hour() && hhl != ASTERISK) {
			return 0;
		}


		if (mdc == DASH) {
			if (time.day() > mdu && time.day() < mdl) {
				return 0;
			}
		}
		if (mdl != time.day() && mdl != ASTERISK) {
			return 0;
		}


		if (wdc == DASH) {
			if (time.dayOfWeek() > wdu && time.dayOfWeek() < wdl) {
				return 0;
			}
		}
		if (wdl != time.dayOfWeek() && wdl != ASTERISK) {
			return 0;
		}


		if (moc == DASH) {
			if (time.month() > mou && time.month() < mol) {
				return 0;
			}
		}
		if (mol != time.month() && mol != ASTERISK) {
			return 0;
		}


		if (yyc == DASH) {
			if (time.year() > yyu && time.year() < yyl) {
				return 0;
			}
		}
		if (yyl != time.year() && yyl != ASTERISK) {
			return 0;
		}

		switch (id) {
			case ASTERISK:
				if (ScheduleEntry::GetDeviseFinalState(ds)) {
					for (uint8_t i = 0; i<pins; i++ ) {
						digitalWrite(devices[i], 1);
					}
				} else {
					for (uint8_t i = 0; i<pins; i++ ) {
						digitalWrite(devices[i], 0);
					}
				}
				break;
			default:
				digitalWrite(devices[id], ScheduleEntry::GetDeviseFinalState(ds));
				break;
		}

		return 1;
	} else {
		return 0;
	}
};