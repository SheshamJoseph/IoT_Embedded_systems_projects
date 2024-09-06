#define BLYNK_TEMPLATE_ID "TMPL2AHX_JPCs"
#define BLYNK_TEMPLATE_NAME "LILYGO"
#define BLYNK_AUTH_TOKEN "-_aYJkur59bkTgxBRAt4sSasKeRvxBW9"

#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// WiFi credentials
const char ssid[] = "blackKareem";
const char pass[] = "pokemon1998@#";

// Blynk auth token
char auth[] = "-_aYJkur59bkTgxBRAt4sSasKeRvxBW9";

BlynkTimer timer;

// Turbidity sensor pin
int turbidityPin = 26; // ESP32 analog input pin

// pH sensor pin
int phSensorPin = 34; // ESP32 analog input pin

// TDS sensor pin
int tdsSensorPin = 35; // ESP32 analog input pin

// DS18B20 temperature sensor
#define ONE_WIRE_BUS 12 // ESP32 digital pin
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

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
  Serial.print("Turbidity: ");
  Serial.print(turbidity);
  Serial.print(" NTU, pH: ");
  Serial.print(ph);
  Serial.print(", TDS: ");
  Serial.print(tds);
  Serial.print(" ppm, Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  // Print what we are sending to Blynk
  Serial.print("Sending to Blynk - Turbidity: ");
  Serial.print(turbidity);
  Serial.print(", pH: ");
  Serial.print(ph);
  Serial.print(", TDS: ");
  Serial.print(tds);
  Serial.print(", Temperature: ");
  Serial.println(temperature);

  // Send the sensor data to the Blynk dashboard
  Blynk.virtualWrite(V1, turbidity);
  Blynk.virtualWrite(V2, ph);
  Blynk.virtualWrite(V3, tds);
  Blynk.virtualWrite(V4, temperature);
}

void checkBlynkConnection() {
  if (!Blynk.connected()) {
    Serial.println("Blynk disconnected, attempting to reconnect...");

    // Try to reconnect up to 3 times
    for (int i = 0; i < 3; i++) {
      if (Blynk.connect()) {
        Serial.println("Blynk reconnected");
        break; // Exit the loop if reconnected successfully
      }
      delay(2000); // Wait 2 seconds before retrying
    }

    if (!Blynk.connected()) {
      Serial.println("Failed to reconnect to Blynk");
      // Implement further actions or logging if reconnection fails
    }
  }
}

void setup() {
  // Set console baud rate
  Serial.begin(115200);
  Serial.println("Serial initialized");

  // Initialize WiFi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize Blynk
  Blynk.begin(auth, ssid, pass);

  // Initialize temperature sensor
  sensors.begin();
  Serial.println("Temperature sensor initialized");

  // Setup a function to be called every 30 seconds for sending sensor data
  timer.setInterval(30000L, sendSensorData); // 30000 milliseconds = 30 seconds

  // Setup a function to check Blynk connection every 30 seconds
  timer.setInterval(30000L, checkBlynkConnection); // 30000 milliseconds = 30 seconds
}

void loop() {
  Blynk.run();
  timer.run();
}
