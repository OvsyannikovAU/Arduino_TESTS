#define INT_A_PIN 6
#define INT_B_PIN 7
long encTick = 0;
volatile bool sigA=false;
volatile bool sigB=false;
void setup() {
  pinMode(INT_A_PIN, INPUT);
  pinMode(INT_B_PIN, INPUT);
  sigA=digitalRead(INT_A_PIN);
  sigB=digitalRead(INT_B_PIN);
  attachInterrupt(INT_A_PIN, intAsub, CHANGE);
  attachInterrupt(INT_B_PIN, intBsub, CHANGE);
  Serial.begin(9600);
}
void intAsub()
{
  sigA=digitalRead(INT_A_PIN);
  if ( sigB != sigA ){ encTick++; }else{ encTick--; }
}

void intBsub()
{
  sigB=digitalRead(INT_B_PIN);
  if ( sigA==sigB ){ encTick++; }else{ encTick--; }
}

void loop() {
    Serial.println(encTick);
    sigA=false;
    Serial.println(sigA);
}
