#define BLYNK_TEMPLATE_ID "TMPL2zhoUCZJl"
#define BLYNK_TEMPLATE_NAME "SEMS"
#define BLYNK_PRINT Serial
#define vCalibration 83.3
#define currCalibration 0.5

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "EmonLib.h"

char auth[] = "2_vmBpCWryYpRsJ02RhhRSEKR0j8S3k5";  // Your Blynk authentication token
char ssid[] = "Elon Net";   // Your WiFi network name
char pass[] = "Amejimadan@890234";  // Your WiFi network password

LiquidCrystal_I2C lcd(0x27, 16, 2); //

const int d8 = 35, d9 = 34;
EnergyMonitor emon;

BlynkTimer timer;

float kWh = 0;
unsigned long lastmillis = millis();
bool firstRun = true;

void updateLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Vrms:");
  lcd.print(emon.Vrms, 2);
  lcd.print("V");

  lcd.setCursor(0, 1);
  lcd.print("Irms:");
  lcd.print(emon.Irms, 2);
  lcd.print("A");
}

void myTimerEvent() {
  // Update emon data
  emon.calcVI(20, 2000);
  kWh = kWh + emon.apparentPower * (millis() - lastmillis) / 3600000000.0;
  lastmillis = millis();

  // Print data on serial monitor
  Serial.print("Vrms: ");
  Serial.print(emon.Vrms, 2);
  Serial.print("V");

  Serial.print("\tIrms: ");
  Serial.print(emon.Irms, 2);
  Serial.print("A");

  Serial.print("\tPower: ");
  Serial.print(emon.apparentPower, 2);
  Serial.print("W");

  Serial.print("\tkWh: ");
  Serial.print(kWh, 2);
  Serial.println("kWh");

  // Update LCD with the latest values
  if (firstRun) {
    updateLCD(); // Display Vrms and Irms
    delay(500);  // Delay for 500 milliseconds
    firstRun = false; // Set firstRun to false after displaying first two values
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Power:");
    lcd.print(emon.apparentPower, 2);
    lcd.print("W");

    lcd.setCursor(0, 1);
    lcd.print("kWh:");
    lcd.print(kWh, 2);
    lcd.print("kWh");

    // Reset firstRun to true after displaying second screen
    firstRun = true;
  }

  // Send data to Blynk
  Blynk.virtualWrite(V0, emon.Vrms);
  Blynk.virtualWrite(V1, emon.Irms);
  Blynk.virtualWrite(V2, emon.apparentPower);
  Blynk.virtualWrite(V3, kWh);
}

void setup() {
  // Initialize the LCD
  lcd.init();
  // Turn on the backlight
  lcd.backlight();

  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  lcd.begin(16, 2);

  emon.voltage(d8, vCalibration, 1.7); // Voltage: input pin, calibration, phase_shift
  emon.current(d9, currCalibration);    // Current: input pin, calibration.

  timer.setInterval(5000L, myTimerEvent);
}

void loop() {
  Blynk.run();
  timer.run();
}
