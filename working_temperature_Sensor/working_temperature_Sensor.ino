#include <OneWire.h>
#include <DallasTemperature.h>

// Pin definition
const int oneWireBus = 4; // GPIO pin where the DS18B20 is connected

// Setup a oneWire instance to communicate with any OneWire device
OneWire oneWire(oneWireBus);

// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);

void setup() { 
  Serial.begin(9600);
  
  // Start up the temperature sensor library
  sensors.begin();
}

void loop() {
  // Request temperature readings
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
    // Print temperature value to the Serial console
  Serial.print("Temp: ");
  Serial.print(temperatureC);
  Serial.println(" C");
if(temperatureC >60){
  Serial.print("Temperature is getting hot "); 
}else if(temperatureC < 20){
  Serial.print("Temperature is getting cold "); 
}else{
   Serial.print("Temperature is warm"); 
}

  
  delay(10000); // Wait a second before the next loop
}
