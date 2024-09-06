
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize with address 0x27, 16 columns, 2 rows

void setup()
{
	// Initialize the LCD
	lcd.init();
	// Turn on the backlight
	lcd.backlight();

	// Initialize serial communication at 9600 baud rate
	Serial.begin(9600);
}

void loop()
{
	// Clear the display buffer
	lcd.clear();
	// Set cursor to (0, 0)
	lcd.setCursor(0, 0);
	// Print "Hello" at (0, 0)
	lcd.print("Hello");
	// Set cursor to (0, 1)
	lcd.setCursor(0, 1);
	// Print "Geek" at (0, 1)
	lcd.print("VICTO/r");

	// Print "Hello Geek" to the serial monitor
	Serial.println("Hello Geek");

	delay(1000); // Delay for 1 second before repeating
}
