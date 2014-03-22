#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <Wire.h>
#include "RTClib.h"
#include "Entry.h"

/*******************************************************************************************************
 * RTC Definition
 *******************************************************************************************************/
RTC_DS1307 RTC;
DateTime now;

/*******************************************************************************************************
 * LCD Definition
 *******************************************************************************************************/
LiquidCrystal lcd(8, 7, 6, 5, 4, 3, 2);
bool refreshed = false;

/*******************************************************************************************************
 * Output Pins Array
 *******************************************************************************************************/
int ledPins[] = {1, 0, 13, 12, 11, 10, 9};

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
	// Serial.begin(9600);

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
	// sec, state, pin
	set_schedule(0, ScheduleEntry::ON, 0);
	set_schedule(10, ScheduleEntry::OFF, 0);

	set_schedule(5, ScheduleEntry::ON, 1);
	set_schedule(15, ScheduleEntry::OFF, 1);

	set_schedule(10, ScheduleEntry::ON, 2);
	set_schedule(20, ScheduleEntry::OFF, 2);

	set_schedule(25, ScheduleEntry::ON, ScheduleEntry::ASTERISK);
	set_schedule(26, ScheduleEntry::OFF, ScheduleEntry::ASTERISK);

	set_schedule(30, ScheduleEntry::ON, 3);
	set_schedule(35, ScheduleEntry::ON, 4);

	set_schedule(40, ScheduleEntry::ON, 5);
	set_schedule(45, ScheduleEntry::ON, 6);

	set_schedule(59, ScheduleEntry::OFF, ScheduleEntry::ASTERISK);

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


 //    if (!read_flag) {
	//     lcd.setCursor(10, 1);
	//     EEPROM.write(1023, 103);
	//     eeprom_value = EEPROM.read(1023);
	//     lcd.print(eeprom_value, DEC);
	//     read_flag = true;
	// } else {
	// 	lcd.setCursor(10, 1);
	// 	lcd.print(eeprom_value, DEC);
	// }
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

	ScheduleEntry schedul_entry(entry);
	schedul_entry.ActivateEntry(RTC);
}
