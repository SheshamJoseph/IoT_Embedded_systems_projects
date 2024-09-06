#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize the LCD with the I2C address of 0x27 and set the display size to 16x4
LiquidCrystal_I2C lcd(0x27, 16, 4);

int sensorPin = 26; // ESP32 analog input pin

void setup() {
  Serial.begin(9600);
   Serial.setDebugOutput(true);
    Serial.println();
    Serial.println("Program begun!");
  // Initialize LCD
  lcd.init();
  
  // Turn on LCD backlight
  lcd.backlight();
}

void loop() {
  int sensorValue = analogRead(sensorPin);
  Serial.print("Raw sensor value: ");
  Serial.println(sensorValue);

  int turbidity = map(sensorValue, 2900, 3200, 100, 0);
 
  Serial.print("Sensor value: ");
  Serial.println(sensorValue);
  
 
  
  // Clear the LCD before writing new data
  lcd.clear();
  
  // Display turbidity value on the LCD
  lcd.setCursor(0, 0);
  lcd.print("Turbidity:");
  lcd.setCursor(10, 0);
  lcd.print(turbidity);
 Serial.print("Mapped turbidity: ");
  Serial.println(turbidity);
  // Determine the turbidity level and display it on the LCD and serial console
  Serial.print("Turbidity: ");
  Serial.println(turbidity);
  
  lcd.setCursor(0, 1);
  if (turbidity < 20) {
    lcd.print(" its CLEAR ");
    Serial.println(" its CLEAR ");
  } else if (turbidity >= 20 && turbidity < 50) {
    lcd.print(" its CLOUDY ");
    Serial.println(" its CLOUDY ");
  } else {
    lcd.print(" its DIRTY ");
    Serial.println(" its DIRTY ");
  }

  // Use additional rows if needed
  // Example: Display additional info on rows 2 and 3
  lcd.setCursor(0, 2);
  lcd.print("More Info:");
  lcd.setCursor(0, 3);
  lcd.print("Row 3 Data");
  
  delay(5000); // Adjust delay as needed
}
