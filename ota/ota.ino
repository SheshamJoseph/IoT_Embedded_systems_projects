/*
  -----------------------
  ElegantOTA - Demo Example
  -----------------------

  Skill Level: Beginner

  This example provides with a bare minimal app with ElegantOTA functionality.

  Github: https://github.com/ayushsharma82/ElegantOTA
  WiKi: https://docs.elegantota.pro

  Works with both ESP8266 & ESP32

  -------------------------------

  Upgrade to ElegantOTA Pro: https://elegantota.pro

*/


#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <WiFiClient.h>
  #include <ESP8266WebServer.h>
#elif defined(ESP32)
  #include <WiFi.h>
  #include <WiFiClient.h>
  #include <WebServer.h>
#endif

#include <ElegantOTA.h>

const char* ssid = "";
const char* password = "";

#if defined(ESP8266)
  ESP8266WebServer server(80);
#elif defined(ESP32)
  WebServer server(80);
#endif

const int LED2 = 2;
int count = 0;

void setup(void) {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("...");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", []() {
    server.send(200, "text/plain", "This is will no longer update via OTA Demo.");
  });

 // ElegantOTA.begin(&server);    // Start ElegantOTA
  server.begin();
  Serial.println("HTTP server started");
  pinMode(LED2, OUTPUT);
}

void loop(void) {
  server.handleClient();
  //ElegantOTA.loop();

  // Blink the LED 3 times
  while (count < 10) {
    digitalWrite(LED2, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(2000);                // wait for a second
    digitalWrite(LED2, LOW);    // turn the LED off by making the voltage LOW
    delay(2000);                // wait for a second
    count++;
  }

  // Reset the count after a delay to blink again
  delay(3000);
  count = 0;
}
