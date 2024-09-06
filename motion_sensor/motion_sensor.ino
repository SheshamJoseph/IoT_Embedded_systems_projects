int motionSensorPin = 26;
int ledPin = 2;

void setup() {
  // put your setup code here, to run once:
 pinMode(motionSensorPin, INPUT);
 pinMode(ledPin, OUTPUT);
Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
 
int sensorValue = digitalRead(motionSensorPin);
 Serial.println(sensorValue);
delay(2000);

if(sensorValue == HIGH){
  digitalWrite(ledPin,HIGH);
  Serial.println("Motion Detected");
  delay(3000);
  digitalWrite(ledPin,LOW);
}else{
    digitalWrite(ledPin,LOW);
      Serial.println("No Motion Detected");
}
delay(5000);
}
