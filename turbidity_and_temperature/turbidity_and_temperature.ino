#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Initialize the LCD with the I2C address of 0x27 and set the display size to 16x4
LiquidCrystal_I2C lcd(0x27, 16, 4);

// Turbidity sensor pin
int turbidityPin = 26; // ESP32 analog input pin

// DS18B20 temperature sensor
#define ONE_WIRE_BUS 4 // ESP32 digital pin
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(9600);
  Serial.setDebugOutput(true);
  Serial.println();
  Serial.println("Program begun!");

  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // Initialize temperature sensor
  sensors.begin();
  Serial.println("Temperature sensor initialized");
}

void loop() {
  // Read the turbidity sensor value
  int sensorValue = analogRead(turbidityPin);
  Serial.print("Raw sensor value: ");
  Serial.println(sensorValue);

  int turbidity = map(sensorValue, 2900, 3200, 100, 0);

  Serial.print("Mapped turbidity: ");
  Serial.println(turbidity);

  // Display values on Serial Monitor
  Serial.print("Turbidity: ");
  Serial.println(turbidity);

  // Clear the LCD before writing new data
  lcd.clear();

  // Display turbidity value on the LCD
  lcd.setCursor(0, 0);
  lcd.print("Turbidity:");
  lcd.setCursor(10, 0);
  lcd.print(turbidity);

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

  // Request temperature reading
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);

  // Print raw data from sensor
  uint8_t addr[8];
  uint8_t data[12];
  if (!oneWire.search(addr)) {
    oneWire.reset_search();
    delay(250);
    return;
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
    Serial.println("CRC is not valid!");
    return;
  }

  if (addr[0] != 0x28) {
    Serial.println("Device is not a DS18B20 family device.");
    return;
  }

  oneWire.reset();
  oneWire.select(addr);
  oneWire.write(0x44, 1); // start conversion, with parasite power on at the end
  delay(1000); // maybe 750ms is enough, maybe not
  // we might do a oneWire.depower() here, but the reset will take care of it.

  oneWire.reset();
  oneWire.select(addr);
  oneWire.write(0xBE); // Read Scratchpad

  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = oneWire.read();
  }

  Serial.print("Raw data: ");
  for (int i = 0; i < 9; i++) {
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  if (temperature == DEVICE_DISCONNECTED_C) {
    Serial.println("Error: Could not read temperature data");
    temperature = -127.00; // Indicates error
  } else {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    // Display temperature on the LCD
    lcd.setCursor(0, 2);
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print(" C");
  }

  delay(5000); // Adjust delay as needed
}
