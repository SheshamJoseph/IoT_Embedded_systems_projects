// Blynk configuration
#define BLYNK_TEMPLATE_ID "TMPL2AHX_JPCs"       // Blynk template ID
#define BLYNK_TEMPLATE_NAME "LILYGO"            // Blynk template name
#define BLYNK_AUTH_TOKEN "-_aYJkur59bkTgxBRAt4sSasKeRvxBW9"  // Blynk authentication token

// Select your modem
#define TINY_GSM_MODEM_SIM800  // Define the modem type as SIM800

// TTGO T-Call pins configuration for the SIM800 modem
#define MODEM_RST            5  // Reset pin
#define MODEM_PWKEY          4  // Power key pin
#define MODEM_POWER_ON       23 // Power on pin
#define MODEM_TX             27 // Modem TX pin
#define MODEM_RX             26 // Modem RX pin

// Set serial for debug console (to Serial Monitor, default speed 115200)
#define SerialMon Serial

// Set serial for AT commands (to SIM800 module)
#define SerialAT Serial1

#include <TinyGsmClient.h>
#include <BlynkSimpleTinyGSM.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Enable prints for debugging
#define BLYNK_PRINT Serial

// GSM PIN, if any
#define GSM_PIN ""

// GPRS credentials
const char apn[] = "web.gprs.mtnnigeria.net"; // APN for GPRS
const char gprsUser[] = "";                  // GPRS username, if any
const char gprsPass[] = "";                  // GPRS password, if any
char auth[] = "-_aYJkur59bkTgxBRAt4sSasKeRvxBW9"; // Blynk authentication token

// Modem and client objects
TinyGsm modem(SerialAT);
TinyGsmClient client(modem);
BlynkTimer timer;

// Sensor pins
int turbidityPin = 32; // ESP32 analog input pin for turbidity sensor
int phSensorPin = 34;  // ESP32 analog input pin for pH sensor
int tdsSensorPin = 35; // ESP32 analog input pin for TDS sensor

// DS18B20 temperature sensor setup
#define ONE_WIRE_BUS 14 // ESP32 digital pin for OneWire communication
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void initializeModem() {
  // Initialize modem control pins
  pinMode(MODEM_PWKEY, OUTPUT);
  pinMode(MODEM_RST, OUTPUT);
  pinMode(MODEM_POWER_ON, OUTPUT);
  digitalWrite(MODEM_PWKEY, LOW);
  digitalWrite(MODEM_RST, HIGH);
  digitalWrite(MODEM_POWER_ON, HIGH);
  SerialMon.println("Modem pins initialized");

  delay(10000); // Wait for modem to initialize

  // Initialize serial communication with the modem
  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
  SerialMon.println("SerialAT initialized");

  // Restart the modem
  SerialMon.println("Initializing modem...");
  modem.restart();
  delay(20000); // Wait for modem to restart
  SerialMon.println("Modem initialized");

  // Unlock SIM card with PIN if necessary
  if (strlen(GSM_PIN) && modem.getSimStatus() != 3) {
    modem.simUnlock(GSM_PIN);
    SerialMon.println("SIM unlocked");
  }

  // Connect to GPRS
  SerialMon.print("Connecting to APN: ");
  SerialMon.print(apn);
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    SerialMon.println(" GPRS connection failed");
    while (true); // Loop indefinitely if connection fails
  }
  SerialMon.println("GPRS connected");

  // Start Blynk with the provided credentials
  Blynk.begin(auth, modem, apn, gprsUser, gprsPass);
}

void sendSensorData() {
  // Read the turbidity sensor value
  int turbidityValue = analogRead(turbidityPin);
  int turbidity = map(turbidityValue, 2900, 3600, 100, 0); // Map the value to NTU

  // Read the pH sensor value
  int phValue = analogRead(phSensorPin);
  float voltage = phValue * (3.3 / 4095.0); // Convert ADC value to voltage
  float ph = 3.5 * voltage + 1.0; // Example calibration, adjust as needed

  // Read the TDS sensor value
  int tdsValue = analogRead(tdsSensorPin);
  float tds = (133.42 * tdsValue * tdsValue * tdsValue - 255.86 * tdsValue * tdsValue + 857.39 * tdsValue) * 0.5; // Example calibration, adjust as needed

  // Request temperature reading from DS18B20 sensor
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);

  // Display sensor values on Serial Monitor
  SerialMon.print("Turbidity: ");
  SerialMon.print(turbidity);
  SerialMon.print(" NTU, pH: ");
  SerialMon.print(ph);
  SerialMon.print(", TDS: ");
  SerialMon.print(tds);
  SerialMon.print(" ppm, Temperature: ");
  SerialMon.print(temperature);
  SerialMon.println(" °C");

  // Send the sensor data to Blynk
  SerialMon.print("Sending to Blynk - Turbidity: ");
  SerialMon.print(turbidity);
  SerialMon.print(", pH: ");
  SerialMon.print(ph);
  SerialMon.print(", TDS: ");
  SerialMon.print(tds);
  SerialMon.print(", Temperature: ");
  SerialMon.println(temperature);

  Blynk.virtualWrite(V1, turbidity);    // Send turbidity value to Blynk virtual pin V1
  Blynk.virtualWrite(V2, ph);           // Send pH value to Blynk virtual pin V2
  Blynk.virtualWrite(V3, tds);          // Send TDS value to Blynk virtual pin V3
  Blynk.virtualWrite(V4, temperature);  // Send temperature value to Blynk virtual pin V4
}

void checkModemStatus() {
  // Print modem signal quality (RSSI)
  int signalQuality = modem.getSignalQuality();
  SerialMon.print("Signal quality: ");
  SerialMon.println(signalQuality);
}

void checkBlynkConnection() {
  // Reconnect to Blynk if disconnected
  if (!Blynk.connected()) {
    SerialMon.println("Blynk disconnected, attempting to reconnect...");

    // Try to reconnect up to 3 times
    for (int i = 0; i < 3; i++) {
      if (Blynk.connect()) {
        SerialMon.println("Blynk reconnected");
        break; // Exit the loop if reconnected successfully
      }
      delay(2000); // Wait 2 seconds before retrying
    }

    if (!Blynk.connected()) {
      SerialMon.println("Failed to reconnect to Blynk");
      // Implement further actions or logging if reconnection fails
    }
  }
}

void setup() {
  // Initialize Serial Monitor for debugging
  SerialMon.begin(115200);
  SerialMon.println("Serial initialized");

  // Initialize modem
  initializeModem();

  // Initialize temperature sensor
  sensors.begin();
  SerialMon.println("Temperature sensor initialized");

  // Setup a function to be called every 30 seconds for sending sensor data
  timer.setInterval(10000L, sendSensorData); // 30000 milliseconds = 30 seconds
  
  // Setup a function to be called every 60 seconds for checking modem status
  timer.setInterval(60000L, checkModemStatus); // 60000 milliseconds = 60 seconds

  // Setup a function to check Blynk connection every 30 seconds
  timer.setInterval(30000L, checkBlynkConnection); // 30000 milliseconds = 30 seconds
}

void loop() {
  // Main loop to run Blynk and the timer
  Blynk.run();
  timer.run();
}
