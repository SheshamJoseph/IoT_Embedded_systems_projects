// Code for a School Alarm System
#include <WiFi.h>
#include <time.h>

const char* ssid = "Nokia 2.1";
const char* password  = "cipherss";
const int buzzer = 4;

struct Alarm{
  int hour;
  int minute;
  int seconds;
  bool active;
};

// define  alarm_list
Alarm alarm_list[] = {
  {8, 00, 00, true},
  {8, 40, 00, true},
  {9, 20, 00, true},
  {9, 50, 00, true},
  {10, 30, 00, true},
  {11, 10, 00, true},
  {11, 50, 00, true},
  {12, 00, 00, true},
  {12, 40, 00, true},
  {13, 20, 00, true},

};

int numOfAlarms = sizeof(alarm_list) / sizeof(alarm_list[0]);

void checkAlarm(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to fetch time...");
    return;
  }

  // loop through the list of alarm_list and check if the current time matches any
  for (int i = 0; i < numOfAlarms; i++){
    if (alarm_list[i].active && timeinfo.tm_hour == alarm_list[i].hour && timeinfo.tm_min == alarm_list[i].minute && timeinfo.tm_sec == alarm_list[i].seconds){
      Serial.println("Alarm");
      digitalWrite(buzzer, HIGH);
      delay(1000);
      digitalWrite(buzzer, LOW);
    }
  }

}

void setup() {
  Serial.begin(115200);

  // initialize wifi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to wifi,");
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // initialize time
  configTime(3600, 0, "pool.ntp.org");

  // Wait for a valid time
  Serial.println("Wating for time...");

  while(!getLocalTime(nullptr)){
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
  Serial.println("Time configured....");
  
  pinMode(buzzer, OUTPUT);
  // make sure the buzzer is off after every initialization
  digitalWrite(buzzer, LOW);

  Serial.println("School Alarm initialized..");
  delay(1000); 

}

void loop() {
  // put your main code here, to run repeatedly:
  checkAlarm();
  delay(1000);

}
