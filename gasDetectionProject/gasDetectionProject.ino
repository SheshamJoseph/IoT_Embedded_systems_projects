#define BLYNK_TEMPLATE_ID "TMPL23RdupsVG"
#define BLYNK_TEMPLATE_NAME "GasDecProject"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Arduino.h>

// const int mq2Pin = 34; // Analog pin connected to MQ-2 sensor
const int mq6Pin = 35; // Analog pin connected to MQ-6 sensor
const int buzzerPin = 26; // PWM-capable pin connected to buzzer
const int redPin = 14; // Pin connected to red LED
const int greenPin = 12; // Pin connected to green LED
const int bluePin = 27; // Pin connected to blue LED

const int ledFrequency = 1000; // Base frequency (1kHz)
const int ledResolution = 8; // 8-bit resolution

char auth[] = "5UYt4bopaJwsdjVrUpa27Tw7ScO1Bx8V";  // Your Blynk authentication token
char ssid[] = "blackKareem";   // Your WiFi network name
char pass[] = "Pass123#";  // Your WiFi network password

void setup() {
  // Debug console
  Serial.begin(115200);
  
  // Start Blynk connection
  Serial.println("Connecting to Blynk...");
  Blynk.begin(auth, ssid, pass);
  
  // Wait for connection
  while (Blynk.connected() == false) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to Blynk!");

  pinMode(mq6Pin, INPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  // Configure the PWM pin using the new API
  ledcAttach(buzzerPin, ledFrequency, ledResolution);

  // Allow the sensor to initialize
  digitalWrite(bluePin, HIGH); // Turn on blue LED during initialization
  delay(10000); // 60 seconds
  digitalWrite(bluePin, LOW); // Turn off blue LED after initialization
}

void loop() {
  Blynk.run();

  int lpgValue = analogRead(mq6Pin);
  Serial.print("LPG: ");
  Serial.println(lpgValue);

  Blynk.virtualWrite(V0, lpgValue);

  if (lpgValue > 300) { // High LPG concentration
    ledcWriteTone(buzzerPin, 1000); // 500Hz frequency for LPG
    digitalWrite(redPin, HIGH); // Turn on red LED
    digitalWrite(greenPin, LOW);
    delay(1000); // 0.5 second duration for LPG
    ledcWriteTone(buzzerPin, LOW); // Stop tone
  } else { // Low concentration of LPG
    ledcWriteTone(buzzerPin, LOW); // Stop tone
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, HIGH); // Turn on green LED
  }

  delay(3000); // Wait for 10 seconds before the next reading
}
