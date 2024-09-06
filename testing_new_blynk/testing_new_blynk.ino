#define BLYNK_TEMPLATE_ID "TMPL2zhoUCZJl"
#define BLYNK_TEMPLATE_NAME "SEMS"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#define BLYNK_PRINT Serial
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

char auth[] = "2_vmBpCWryYpRsJ02RhhRSEKR0j8S3k5";  // Your Blynk authentication token
char ssid[] = "blackKareem";   // Your WiFi network name
char pass[] = "pokemon1998@#";  // Your WiFi network password

LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize with address 0x27, 16 columns, 2 rows

void setup()
{
  Serial.begin(9600);
  Serial.println("Connecting to WiFi...");
  Blynk.begin(auth, ssid, pass);  // Connect to Blynk using WiFi

  while (Blynk.connect() == false) {
    // Wait until connected
  }
  
  Serial.println("Connected to Blynk!");
  Serial.print("Blynk Auth Token: ");
  Serial.println(auth);
  Serial.print("WiFi SSID: ");
  Serial.println(ssid);

  // Initialize the I2C interface
  Wire.begin();
  // Initialize the LCD
  lcd.begin(16, 2);  // Adjust according to your LCD size
  // Turn on the backlight
  lcd.backlight();
}

void loop()
{
  Blynk.run();  // Continuously check for Blynk commands

  // Display connection status on LCD
  if (Blynk.connected()) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Connected to");
    lcd.setCursor(0, 1);
    lcd.print("Blynk");
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Disconnected");
    lcd.setCursor(0, 1);
    lcd.print("from Blynk");
  }

  delay(1000); // Delay for 1 second before repeating
}
