int ledPins[] = {0,1,9,10,11,12,13};                 // LED connected to digital pin 13
int i;
int delay_valu = 100;

void setup()
{
	for (i = 0; i < (sizeof(ledPins)/sizeof(int)); i++) {
		//Serial.println("Set pin[%d] as output.", ledPins[i]);
		pinMode(ledPins[i], OUTPUT);      // sets the digital pin as output
	}

	
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
}