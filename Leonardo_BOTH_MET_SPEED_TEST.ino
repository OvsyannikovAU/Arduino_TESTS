#include <TimerThree.h>

#define SPD_PIN 6
#define DIR_1_PIN 8
#define DIR_2_PIN 7
#define INT_PIN 3
#define INT_DIR_PIN 4
#define IPR 12 //impuls per rotation
#define REDUCTION 74.83
#define NOM_SPD 100 //RPM - Rotation per Minute
#define MAX_SPD NOM_SPD*6 //DPS - Degrees per Second
#define TIMER_PERIOD 50000

volatile long enc = 0;
volatile word dt = 65535;
volatile long dF = 0;
volatile float w_timer = 0.0f;
volatile float w_interrupt = 0.0f;
volatile unsigned long oldTime = 0;
volatile long oldEnc=0;
int motorPower=0; //power (speed) to motor
float motorW=0; //rotate speed to motor
int deltaPower=1; //delta speed per loop step

void setup() {
  pinMode(SPD_PIN, OUTPUT);
  pinMode(DIR_1_PIN, OUTPUT);
  pinMode(DIR_2_PIN, OUTPUT);
  pinMode(INT_PIN, INPUT);
  pinMode(INT_DIR_PIN, INPUT);
  Serial.begin(9600);
  Timer3.initialize(TIMER_PERIOD);
  Timer3.attachInterrupt(intTime);  
  oldTime = micros();
  attachInterrupt(digitalPinToInterrupt(INT_PIN), intEnc, RISING);
}

void intTime(void){
  long thisEnc=enc;
  dF=thisEnc-oldEnc;
  oldEnc=thisEnc;
  w_timer=(360.0f*1000000.0f*dF)/(REDUCTION*IPR*TIMER_PERIOD);
}

void intEnc(){
  unsigned long thisTime = micros();
  if( thisTime-oldTime>65535) {dt = 65353;}else{dt = thisTime - oldTime;}
  oldTime = thisTime;
  if ( digitalRead(INT_DIR_PIN) ) {
    enc++;
    w_interrupt=(360.0f*1000000.0f)/(REDUCTION*IPR*dt);
  } else {
    enc--;
    w_interrupt=-(360.0f*1000000.0f)/(REDUCTION*IPR*dt);
  }
}

void motorA(int spd){
  if(spd>255)spd=255;
  if(spd<-255)spd=-255;
  if(spd>=0)
  {
    digitalWrite(DIR_1_PIN, HIGH);
    digitalWrite(DIR_2_PIN, LOW);
    analogWrite(SPD_PIN, spd);
  }else{
    digitalWrite(DIR_1_PIN, LOW);
    digitalWrite(DIR_2_PIN, HIGH);
    analogWrite(SPD_PIN, abs(spd));    
  }
}

void loop() {
  if( motorPower>254 ) deltaPower=-1;
  if( motorPower<-254 ) deltaPower=1;
  motorPower=motorPower+deltaPower;
  motorA(motorPower);
  motorW=map(motorPower, -255, 255, -MAX_SPD, MAX_SPD);
  Serial.print(motorW);
  Serial.print("\t");
  Serial.print(w_timer);
  Serial.print("\t");
  Serial.print(w_interrupt);
  Serial.print("\t");
  Serial.print(0);
  Serial.print("\t");
  Serial.print(MAX_SPD);
  Serial.print("\t");
  Serial.println(-MAX_SPD);
  delay(15);
}
