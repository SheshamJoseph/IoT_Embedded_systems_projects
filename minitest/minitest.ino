#define BLYNK_TEMPLATE_ID "TMPL2JB9S0-pi"
#define BLYNK_TEMPLATE_NAME "SEMS"
#define BLYNK_PRINT Serial  // Comment this out to disable prints and save space

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <TinyGsmClient.h>
#include <BlynkSimpleTinyGSM.h>

// Initialize the LCD with the I2C address of 0x27 and set the display size to 16x4
LiquidCrystal_I2C lcd(0x27, 16, 4);

int sensorPin = 26; // ESP32 analog input pin

// Select your modem:
#define TINY_GSM_MODEM_SIM800

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial

// Set serial for AT commands (to the module)
#define SerialAT Serial1

// See all AT commands, if wanted
// #define DUMP_AT_COMMANDS

// Define the serial console for debug prints, if needed
#define TINY_GSM_DEBUG SerialMon

// set GSM PIN, if any
#define GSM_PIN ""

// Your GPRS credentials, if any
const char apn[] = "web.gprs.mtnnigeria.net";       // APN
const char gprsUser[] = ""; // GPRS User
const char gprsPass[] = ""; // GPRS Password

// Your Blynk Auth Token
char auth[] = "mpnKj0bv-3aM4BDcfvY0sikdos12KLeN";

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif
TinyGsmClient client(modem);

void setup() {
  // Set console baud rate
  SerialMon.begin(115200);

  // Initialize LCD
  lcd.init();
  
  // Turn on LCD backlight
  lcd.backlight();

  delay(10);

  // Set GSM module baud rate and UART pins
  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);

  delay(6000);

  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  SerialMon.println("Initializing modem...");
  modem.restart();
  // modem.init();

  String modemInfo = modem.getModemInfo();
  SerialMon.print("Modem Info: ");
  SerialMon.println(modemInfo);

  // Unlock your SIM card with a PIN
  // modem.simUnlock("1234");

  Blynk.begin(auth, modem, apn, gprsUser, gprsPass);
}

void loop() {
  int sensorValue = analogRead(sensorPin);
  Serial.print("Raw sensor value: ");
  Serial.println(sensorValue);

  int turbidity = map(sensorValue, 2900, 3600, 100, 0);

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
    lcd.print(" it's CLEAR ");
    Serial.println(" it's CLEAR ");
  } else if (turbidity >= 20 && turbidity < 50) {
    lcd.print(" it's CLOUDY ");
    Serial.println(" it's CLOUDY ");
  } else {
    lcd.print(" it's DIRTY ");
    Serial.println(" it's DIRTY ");
  }

  // Use additional rows if needed
  // Example: Display additional info on rows 2 and 3
  lcd.setCursor(0, 2);
  lcd.print("More Info:");
  lcd.setCursor(0, 3);
  lcd.print("Row 3 Data");

  // Send the turbidity data to the Blynk dashboard
  Blynk.virtualWrite(V1, turbidity);

  delay(5000); // Adjust delay as needed

  Blynk.run();
}
