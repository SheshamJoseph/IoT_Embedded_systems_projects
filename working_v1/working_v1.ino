#define BLYNK_TEMPLATE_ID "TMPL2AHX_JPCs"
#define BLYNK_TEMPLATE_NAME "LILYGO"
#define BLYNK_AUTH_TOKEN "-_aYJkur59bkTgxBRAt4sSasKeRvxBW9"

// Select your modem:
#define TINY_GSM_MODEM_SIM800

// TTGO T-Call pins
#define MODEM_RST            5
#define MODEM_PWKEY          4
#define MODEM_POWER_ON       23
#define MODEM_TX             27
#define MODEM_RX             26

// Set serial for debug console (to Serial Monitor, default speed 115200)
#define SerialMon Serial
// Set serial for AT commands (to SIM800 module)
#define SerialAT Serial1

#include <TinyGsmClient.h>
#include <BlynkSimpleTinyGSM.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space

// set GSM PIN, if any
#define GSM_PIN ""

// Your GPRS credentials, if any
const char apn[] = "web.gprs.mtnnigeria.net";       // APN
const char gprsUser[] = ""; // GPRS User
const char gprsPass[] = ""; // GPRS Password
char auth[] = "-_aYJkur59bkTgxBRAt4sSasKeRvxBW9";

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);
BlynkTimer timer;

// Turbidity sensor pin
int turbidityPin = 32; // ESP32 analog input pin

// pH sensor pin
int phSensorPin = 34; // ESP32 analog input pin

// TDS sensor pin
int tdsSensorPin = 35; // ESP32 analog input pin

// DS18B20 temperature sensor
#define ONE_WIRE_BUS 14 // ESP32 digital pin
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void initializeModem() {
  // Set modem reset, enable, power pins
  pinMode(MODEM_PWKEY, OUTPUT);
  pinMode(MODEM_RST, OUTPUT);
  pinMode(MODEM_POWER_ON, OUTPUT);
  digitalWrite(MODEM_PWKEY, LOW);
  digitalWrite(MODEM_RST, HIGH);
  digitalWrite(MODEM_POWER_ON, HIGH);
  SerialMon.println("Modem pins initialized");

  delay(10000); // Wait for modem to initialize properly

  // Set GSM module baud rate and UART pins
  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
  SerialMon.println("SerialAT initialized");

  // Restart SIM800 module
  SerialMon.println("Initializing modem...");
  modem.restart();
  delay(20000); // Wait for modem to restart
  SerialMon.println("Modem initialized");

  // Unlock your SIM card with a PIN
  if (strlen(GSM_PIN) && modem.getSimStatus() != 3 ) {
    modem.simUnlock(GSM_PIN);
    SerialMon.println("SIM unlocked");
  }

  // Connect to GPRS
  SerialMon.print("Connecting to APN: ");
  SerialMon.print(apn);
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    SerialMon.println(" GPRS connection failed");
    while (true); // Loop indefinitely or implement retry logic
  }
  SerialMon.println("GPRS connected");

  // Start Blynk
  Blynk.begin(auth, modem, apn, gprsUser, gprsPass);
}

void sendSensorData() {
  int turbidityValue = analogRead(turbidityPin);
  int turbidity = map(turbidityValue, 2900, 3600, 100, 0);

  // Read the pH sensor value
  int phValue = analogRead(phSensorPin);
  float voltage = phValue * (3.3 / 4095.0);
  float ph = 3.5 * voltage + 1.0; // Example calibration, adjust as needed

  // Read the TDS sensor value
  int tdsValue = analogRead(tdsSensorPin);
  float tds = (133.42 * tdsValue * tdsValue * tdsValue - 255.86 * tdsValue * tdsValue + 857.39 * tdsValue) * 0.5; // Example calibration, adjust as needed

  // Request temperature reading
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);

  // Display values on Serial Monitor
  SerialMon.print("Turbidity: ");
  SerialMon.print(turbidity);
  SerialMon.print(" NTU, pH: ");
  SerialMon.print(ph);
  SerialMon.print(", TDS: ");
  SerialMon.print(tds);
  SerialMon.print(" ppm, Temperature: ");
  SerialMon.print(temperature);
  SerialMon.println(" °C");

  // Print what we are sending to Blynk
  SerialMon.print("Sending to Blynk - Turbidity: ");
  SerialMon.print(turbidity);
  SerialMon.print(", pH: ");
  SerialMon.print(ph);
  SerialMon.print(", TDS: ");
  SerialMon.print(tds);
  SerialMon.print(", Temperature: ");
  SerialMon.println(temperature);

  // Send the sensor data to the Blynk dashboard
  Blynk.virtualWrite(V1, turbidity);
  Blynk.virtualWrite(V2, ph);
  Blynk.virtualWrite(V3, tds);
  Blynk.virtualWrite(V4, temperature);
}

void checkModemStatus() {
  // Print signal quality (RSSI)
  int signalQuality = modem.getSignalQuality();
  SerialMon.print("Signal quality: ");
  SerialMon.println(signalQuality);
}

void checkBlynkConnection() {
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
  // Set console baud rate
  SerialMon.begin(115200);
  SerialMon.println("Serial initialized");

  // Initialize modem
  initializeModem();

  // Initialize temperature sensor
  sensors.begin();
  SerialMon.println("Temperature sensor initialized");

  // Setup a function to be called every 30 seconds for sending sensor data
  timer.setInterval(30000L, sendSensorData); // 30000 milliseconds = 30 seconds
  
  // Setup a function to be called every 60 seconds for checking modem status
  timer.setInterval(60000L, checkModemStatus); // 60000 milliseconds = 60 seconds

  // Setup a function to check Blynk connection every 30 seconds
  timer.setInterval(30000L, checkBlynkConnection); // 30000 milliseconds = 30 seconds
}

void loop() {
  Blynk.run();
  timer.run();
}
