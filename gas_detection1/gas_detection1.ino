const int buzzer = 2;
const int mq5 = 35;
const int threshold = 1900;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(buzzer, OUTPUT);
  pinMode(mq5, INPUT);
  delay(1000);
  Serial.println("MQ-5 Gas Detector running...");

}

void loop() {
  // put your main code here, to run repeatedly:
  int value = analogRead(mq5);
  Serial.println(value);
  if (value > threshold){
    digitalWrite(buzzer, HIGH);
    delay(1000);
    digitalWrite(buzzer, LOW);
    delay(1000);
  }
}
