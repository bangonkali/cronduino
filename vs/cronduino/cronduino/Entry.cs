using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace cronduino
{
    class Entry
    {
        public string Name { get; set; }
        public char Enable { get; set; }
        public char DeviceID { get; set; }
        public char DeviceState { get; set; }
        public char SecondLower { get; set; }
        public char SecondClassifier { get; set; }
        public char SecondUpper { get; set; }
        public char MinutesLower { get; set; }
        public char MinutesClassifier { get; set; }
        public char MinutesUpper { get; set; }
        public char HourLower { get; set; }
        public char HourClassifier { get; set; }
        public char HourUpper { get; set; }
        public char MonthDayLower { get; set; }
        public char MonthDayClassifier { get; set; }
        public char MonthDayUpper { get; set; }
        public char WeekdayLower { get; set; }
        public char WeekdayClassifier { get; set; }
        public char WeekdayUpper { get; set; }
        public char MonthLower { get; set; }
        public char MonthClassifier { get; set; }
        public char MonthUpper { get; set; }
        public char YearLower { get; set; }
        public char YearClassifier { get; set; }
        public char YearUpper { get; set; }

        public char[] entry = new char[24];

        public Entry()
        {
            this.Enable = (char)0;
            this.DeviceID = (char)0;
            this.DeviceState = (char)0;
            this.SecondLower = (char)0;
            this.SecondClassifier = (char)0;
            this.SecondUpper = (char)0;
            this.MinutesLower = (char)0;
            this.MinutesClassifier = (char)0;
            this.MinutesUpper = (char)0;
            this.HourLower = (char)0;
            this.HourClassifier = (char)0;
            this.HourUpper = (char)0;
            this.MonthDayLower = (char)0;
            this.MonthDayClassifier = (char)0;
            this.MonthDayUpper = (char)0;
            this.WeekdayLower = (char)0;
            this.WeekdayClassifier = (char)0;
            this.WeekdayUpper = (char)0;
            this.MonthLower = (char)0;
            this.MonthClassifier = (char)0;
            this.MonthUpper = (char)0;
            this.YearLower = (char)0;
            this.YearClassifier = (char)0;
            this.YearUpper = (char)0;

            this.entry = new char[24];
            this.toArray();
        }

        public void toArray() {

            if (this.Enable == '1')
            {
                entry[0] = (char)1;
            }
            entry[0] = this.Enable;
            
            entry[0] = this.DeviceID;
            entry[0] = this.DeviceState;
            entry[0] = this.SecondLower;
            entry[0] = this.SecondClassifier;
            entry[0] = this.SecondUpper;
            entry[0] = this.MinutesLower;
            entry[0] = this.MinutesClassifier;
            entry[0] = this.MinutesUpper;
            entry[0] = this.HourLower;
            entry[0] = this.HourClassifier;
            entry[0] = this.HourUpper;
            entry[0] = this.MonthDayLower;
            entry[0] = this.MonthDayClassifier;
            entry[0] = this.MonthDayUpper;
            entry[0] = this.WeekdayLower;
            entry[0] = this.WeekdayClassifier;
            entry[0] = this.WeekdayUpper;
            entry[0] = this.MonthLower;
            entry[0] = this.MonthClassifier;
            entry[0] = this.MonthUpper;
            entry[0] = this.YearLower;
            entry[0] = this.YearClassifier;
            entry[0] = this.YearUpper;
        }



    }
}
