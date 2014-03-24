using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace cronduino
{
    class Entry
    {
        public string Name { get; set; }

        public bool Enable
        {
            get
            {
                return _enable;
            }
            set
            {
                _enable = value;
                toArray();
            }
        }

        public char DeviceID
        {
            get
            {
                return _deviceID;
            }
            set
            {
                _deviceID = value;
                toArray();
            }
        }

        public bool DeviceState
        {
            get
            {
                return _deviceState;
            }
            set
            {
                _deviceState = value;
                toArray();
            }
        }

        public string Second
        {
            get
            {
                return _second;
            }
            set
            {
                if (value.Length > 5) throw new Exception("Second must only have less than 5 characters.");
                _second = value;
                toArray();
            }
        }
        public string Minute
        {
            get
            {
                return _minute;
            }
            set
            {
                if (value.Length > 5) throw new Exception("Minute must only have 5 characters.");
                _minute = value;
                toArray();
            }
        }
        public string Hour
        {
            get
            {
                return _hour;
            }
            set
            {
                if (value.Length > 5) throw new Exception("Hour must only have 5 characters.");
                _hour = value;
                toArray();
            }
        }
        public string MonthDay
        {
            get
            {
                return _monthDay;
            }
            set
            {
                if (value.Length > 5) throw new Exception("MonthDay must only have 5 characters.");
                _monthDay = value;
                toArray();
            }
        }
        public string WeekDay
        {
            get
            {
                return _weekDay;
            }
            set
            {
                if (value.Length > 5) throw new Exception("WeekDay must only have 5 characters.");
                _weekDay = value;
                toArray();
            }
        }
        public string Month
        {
            get
            {
                return _month;
            }
            set
            {
                if (value.Length > 5) throw new Exception("Month must only have 5 characters.");
                _month = value;
                toArray();
            }
        }
        public string Year
        {
            get
            {
                return _year;
            }
            set
            {
                if (value.Length > 5) throw new Exception("Year must only have 5 characters.");
                _year = value;
                toArray();
            }
        }

        private bool _enable = true;
        private char _deviceID = '*';
        private bool _deviceState = true;

        private string _second = "*";
        private string _minute = "*";
        private string _hour = "*";
        private string _monthDay = "*";
        private string _weekDay = "*";
        private string _month = "*";
        private string _year = "*";

        enum CLASSDATA
        {
            ASTERISK = 255,
            DASH = 254,
            SLASH = 253,
            ON = 252,
            OFF = 251,
            EMPTY = 250
        };

        public byte[] entry = new byte[24];

        public Entry()
        {
            this.entry = new byte[24];
            this.toArray();
        }

        public void toArray()
        {

            if (_enable)
            {
                this.entry[0] = (byte)1;
            }
            else
            {
                this.entry[0] = (byte)0;
            }

            if (_deviceID == 42)
            {
                this.entry[1] = (byte)Entry.CLASSDATA.ASTERISK;
            }
            else if (isNumeric(_deviceID) && int.Parse(_deviceID.ToString()) < 8)
            {
                this.entry[1] = (byte)(int.Parse(_deviceID.ToString()));
            }
            

            if (_deviceState)
            {
                this.entry[2] = (byte)Entry.CLASSDATA.ON;
            }
            else
            {
                this.entry[2] = (byte)Entry.CLASSDATA.OFF;
            }

            this.saveEntrySet(_second, 3);
            this.saveEntrySet(_minute, 6);
            this.saveEntrySet(_hour, 9);
            this.saveEntrySet(_monthDay, 12);
            this.saveEntrySet(_weekDay, 15);
            this.saveEntrySet(_month, 18);
            this.saveEntrySet(_year, 21);

            // this.printDebugEntry();
        }

        public void printDebugEntry()
        {
            string buffer = "";
            for (int i = 0; i < 24; i++)
            {
               
                buffer += i.ToString() + ":" + this.entry[i].ToString() + "\t";
            }
            System.Diagnostics.Debug.WriteLine(buffer);
        }

        // from string to actual array
        public void saveEntrySet(string data, int offset)
        {
            int state = 0, previousState = 0; // initial state
            string bufferA = "";
            string bufferB = "";
            byte classifier = (byte)Entry.CLASSDATA.EMPTY;
            bool asteriskFirst = false;

            for (int i = 0; i < data.Length; i++)
            {
                if (isValid(data[i]))
                {
                    // either immediately * or immediately a number
                    if (state == 0)
                    {
                        if (data[i] == '*')
                        {
                            this.entry[offset] = (byte)Entry.CLASSDATA.ASTERISK;
                            asteriskFirst = true;
                            state = 3;
                        }
                        else if (Entry.isNumeric(data[i]))
                        {
                            // this.entry[offset] = (byte)((byte)data[i] - (byte)48); // '0' is 48
                            bufferA += data[i].ToString();
                            state = 1;
                        }
                        else
                        {
                            throw new Exception("Syntax Error: Invalid character on first byte.");
                        }
                    }

                    else if (state == 1)
                    {
                        if (Entry.isNumeric(data[i]))
                        {
                            // this.entry[offset] = (byte)((byte)data[i] - (byte)48); // '0' is 48
                            bufferA += data[i].ToString();
                        }
                        else if (data[i] == '/')
                        {
                            classifier = (byte)Entry.CLASSDATA.SLASH;
                            state = 2;
                        }
                        else if (data[i] == '-')
                        {
                            classifier = (byte)Entry.CLASSDATA.DASH;
                            state = 2;
                        }
                        else if (data[i] == '*')
                        {
                            classifier = (byte)Entry.CLASSDATA.ASTERISK;
                            state = 2;
                        }
                        else
                        {
                            throw new Exception("Syntax Error: Classifier should follow after first numeric data.");
                        }
                    }

                    else if (state == 2)
                    {
                        if (Entry.isNumeric(data[i]))
                        {
                            // this.entry[offset] = (byte)((byte)data[i] - (byte)48); // '0' is 48
                            bufferB += data[i].ToString();
                            state = 4;
                        }
                        else
                        {
                            throw new Exception("Syntax Error: Invelid character following classifier.");
                        }
                    }

                    else if (state == 3)
                    {
                        if (data[i] == '/')
                        {
                            classifier = (byte)Entry.CLASSDATA.SLASH;
                            state = 2;
                        }
                        else
                        {
                            throw new Exception("Syntax Error: There should be nothing else that follows after asterisk except '/'.");
                        }
                    }

                    else if (state == 4)
                    {
                        if (Entry.isNumeric(data[i]))
                        {
                            // this.entry[offset] = (byte)((byte)data[i] - (byte)48); // '0' is 48
                            bufferB += data[i].ToString();
                            state = 4;
                        }
                        else
                        {
                            throw new Exception("Syntax Error: Incorrect character after upper limit.");
                        }
                    }

                }
                else
                {
                    throw new Exception("Syntax Error: Ivalid character.");
                }

                previousState = state;
            }

            if (asteriskFirst && classifier == (byte)Entry.CLASSDATA.SLASH)
            {
                this.entry[offset] = (byte)Entry.CLASSDATA.ASTERISK;
                this.entry[offset + 1] = (byte)Entry.CLASSDATA.SLASH;
                saveEntryField(bufferB, offset + 2);
            } 
            else if (asteriskFirst)
            {
                this.entry[offset] = (byte)Entry.CLASSDATA.ASTERISK;
                this.entry[offset+1] = (byte)Entry.CLASSDATA.EMPTY;
                this.entry[offset+2] = (byte)Entry.CLASSDATA.EMPTY;
            }
            else
            {
                saveEntryField(bufferA, offset);
                saveEntryField(bufferB, offset + 2);
                this.entry[offset + 1] = classifier;
            }
            
        }

        /* Moves from char data to this.entry[offset] */
        public void saveEntryField(string data, int offset)
        {
            int valA = 0;

            if (data.Length > 0)
            {
                if (isNumeric(data))
                {
                    valA = int.Parse(data);
                    if (valA < 256)
                    {
                        this.entry[offset] = (byte)valA;
                    }
                    else
                    {
                        throw new Exception("Value Error: Numbers must be less than 256.");
                    }
                }
            }
        }

        public static bool isNumeric(char data)
        {
            return isNumeric(data.ToString());
        }

        public static bool isNumeric(string data)
        {
            return Regex.IsMatch(data, @"^\d+$", RegexOptions.Compiled);
        }

        public static bool isValid(char data)
        {
            return (isNumeric(data) || data == '/' || data == '*' || data == '-' || data == ' ');
        }
    }
}
