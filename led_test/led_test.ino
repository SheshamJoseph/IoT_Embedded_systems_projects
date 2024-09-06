int LED2 = 2;
int count =0;
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED2, OUTPUT);
    
}
 
// the loop function runs over and over again forever
void loop() {

do {
  digitalWrite(LED2, LOW); 
  digitalWrite(LED2, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED2, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);
  count ++;
}
while (count <3);

digitalWrite(LED2, LOW); 
  delay(3000);                     // wait for a second
}