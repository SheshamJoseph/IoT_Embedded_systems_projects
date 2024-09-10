/*
Smart waster pump system including a voltage regulator 
to prevent pump from turning on with low voltage
*/

#define hcsr_trig 25
#define hcsr_echo 34
#define relay 14
#define zmpt 35
#define led1 2
#define led2 4
#define led3 5
#define led4 18
#define lcd_scl 21
#define lcd_sda 22

#define tankHeight 400 // in cm

double check_level() {
  long duration, distance;
  digitalWrite(hcsr_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(hcsr_trig, HIGH);
  delayMicroseconds(10),
  digitalWrite(hcsr_trig, LOW);

  duration = pulseIn(hcsr_echo, HIGH);
  distance = (duration * 0.034) / 2;
  // return distance;
  double level = (double) ((tankHeight-distance) / tankHeight) * 100;
  return level;
}

void setup() {
  delay(1000);
  Serial.begin(115200);

  // define pin modes
  pinMode(hcsr_trig, OUTPUT);
  pinMode(hcsr_echo, INPUT);
  pinMode(relay, OUTPUT);
  pinMode(zmpt, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(lcd_scl, OUTPUT);
  pinMode(lcd_sda, OUTPUT);

  Serial.println("Water Pump system started...");
}

void loop() {
}
