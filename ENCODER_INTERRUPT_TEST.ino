#define INT_PIN 3
#define INT_DIR_PIN 4

volatile long enc = 0;

void setup() {
  pinMode(INT_PIN, INPUT);
  pinMode(INT_DIR_PIN, INPUT);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(INT_PIN), intEnc, RISING);
}

void intEnc(){
  if ( digitalRead(INT_DIR_PIN) ) {
    enc++;
  } else {
    enc--;
  }
}

void loop() {
  Serial.println(enc);
  delay(15);
}
