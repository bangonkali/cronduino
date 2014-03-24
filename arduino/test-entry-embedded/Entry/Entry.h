/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
#ifndef _ENTRY_H_
#define _ENTRY_H_

#include <Wire.h>
#include <RTClib.h>
#include "Arduino.h"

class ScheduleEntry {
public:
    // Constructor definition
    ScheduleEntry(const uint8_t *entry);
    ScheduleEntry();

	enum CLASSDATA {
		ASTERISK = 255,
		DASH = 254,
		SLASH = 253,
		ON = 252,
		OFF = 251,
		EMPTY = 250
	};

    uint8_t enable() const { return en; }
    uint8_t device_id() const { return id; }
    uint8_t device_state() const { return ds; }

    uint8_t year_upper() const { return yyu; }
    uint8_t year_lower() const { return yyl; }
    uint8_t year_classifier() const { return yyc; }

    uint8_t month_upper() const { return mou; }
    uint8_t month_lower() const { return mol; }
    uint8_t month_classifier() const { return moc; }

    uint8_t day_upper() const { return mdu; }
    uint8_t day_lower() const { return mdl; }
    uint8_t day_classifier() const { return mdc; }

    uint8_t hour_upper() const { return hhu; }
    uint8_t hour_lower() const { return hhl; }
    uint8_t hour_classifier() const { return hhc; }

    uint8_t minute_upper() const { return mnu; }
    uint8_t minute_lower() const { return mnl; }
    uint8_t minute_classifier() const { return mnc; }

    uint8_t second_upper() const { return ssu; }
    uint8_t second_lower() const { return ssl; }
    uint8_t second_classifier() const { return ssc; }

    uint8_t dayOfWeek_upper() const { return wdu; }
    uint8_t dayOfWeek_lower() const { return wdl; }
    uint8_t dayOfWeek_classifier() const { return wdc; }

    uint8_t SetEntry(const uint8_t *entry);
	uint8_t ActivateEntry(RTC_DS1307 RTC);
protected:
	uint8_t en; 
	uint8_t id;
	uint8_t ds;
	
	uint8_t ssl ;
	uint8_t ssc ;
	uint8_t ssu ;

	uint8_t mnl ;
	uint8_t mnc ;
	uint8_t mnu ;

	uint8_t hhl ;
	uint8_t hhc ;
	uint8_t hhu ;

	uint8_t mdl ;
	uint8_t mdc ;
	uint8_t mdu ;

	uint8_t wdl ;
	uint8_t wdc ;
	uint8_t wdu ;

	uint8_t mol ;
	uint8_t moc ;
	uint8_t mou ;

	uint8_t yyl ;
	uint8_t yyc ;
	uint8_t yyu ;

	DateTime time;

	static uint8_t GetDeviseFinalState(uint8_t deviceCommState);
};

#endif 