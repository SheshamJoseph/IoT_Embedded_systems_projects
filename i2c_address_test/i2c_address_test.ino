#define BLYNK_TEMPLATE_ID "TMPL2zhoUCZJl"
#define BLYNK_TEMPLATE_NAME "SEMS"
#include <LiquidCrystal.h>
#include "EmonLib.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
const int rs = 13, en = 12, d4 = 14, d5 = 27, d6 = 26, d7 = 25, d8 = 35,d9 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
#define BLYNK_PRINT Serial

EnergyMonitor emon;
#define vCalibration 83.3
#define currCalibration 0.5
BlynkTimer timer;
char auth[] = "2_vmBpCWryYpRsJ02RhhRSEKR0j8S3k5";
char ssid[] = "blackKareem";
char pass[] = "pokemon1998@#";
float kWh = 0;
unsigned long lastmillis = millis();
 
void myTimerEvent()
{
  emon.calcVI(20, 2000);
  kWh = kWh + emon.apparentPower * (millis() - lastmillis) / 3600000000.0;
  yield();
  Serial.print("Vrms: ");
  Serial.print(emon.Vrms, 2);
  Serial.print("V");
 
  Serial.print("\tIrms: ");
  Serial.print(emon.Irms, 4);
  Serial.print("A");
 
  Serial.print("\tPower: ");
  Serial.print(emon.apparentPower, 4);
  Serial.print("W");
 
  Serial.print("\tkWh: ");
  Serial.print(kWh, 5);
  Serial.println("kWh");
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Vrms:");
  lcd.print(emon.Vrms, 2);
  lcd.print("V");
  lcd.setCursor(0, 1);
  lcd.print("Irms:");
  lcd.print(emon.Irms, 4);
  lcd.print("A");
  delay(2500);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Power:");
  lcd.print(emon.apparentPower, 4);
  lcd.print("W");
  lcd.setCursor(0, 1);
  lcd.print("kWh:");
  lcd.print(kWh, 4);
  lcd.print("W");
  delay(2500);
 
  lastmillis = millis();
 
  Blynk.virtualWrite(V0, emon.Vrms);
  Blynk.virtualWrite(V1, emon.Irms);
  Blynk.virtualWrite(V2, emon.apparentPower);
  Blynk.virtualWrite(V3, kWh);
}
 
void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  lcd.begin(16, 2);
 
  emon.voltage(d9, vCalibration, 1.7); // Voltage: input pin, calibration, phase_shift
  emon.current(d8, currCalibration); // Current: input pin, calibration.
 
  timer.setInterval(5000L, myTimerEvent);
  lcd.setCursor(3, 0);
  lcd.print("PROJECT");
  lcd.setCursor(5, 1);
  lcd.print("SEMS");
  delay(3000);
  lcd.clear();
}

 
void loop()
{
  Blynk.run();
  timer.run();
}


