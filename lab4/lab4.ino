#include <Servo.h>
#include "TimerOne.h"
#include "Wheels.h"

// constants
#define SPEED 255
#define LEFT 0
#define RIGHT 180
// piny dla sonaru (HC-SR04)
#define TRIG A4
#define ECHO A5

// pin kontroli serwo (musi być PWM)
#define SERVO 3



Servo serwo;
Wheels w;
volatile int counter = 0;
volatile int turns = 0;
short distanceLeft = 0;
short distanceRight = 0;

void setup() {
  pinMode(TRIG, OUTPUT);    // TRIG startuje sonar
  pinMode(ECHO, INPUT);     // ECHO odbiera powracający impuls
  w.attach(8,2,6,5,4,11);
  Serial.begin(9600);
  serwo.attach(SERVO);

  w.setSpeed(150);
  w.forward();
}

void loop() { 
  distanceLeft = lookAndTellDistance(LEFT);
  BTSerial.write("<L;" + distanceLeft + ';' + w.cnt + '>');
  if(lookAndTellDistance(80) <= 70 || lookAndTellDistance(100) <= 70){
    w.stop();
    exit();
  }
 
  distanceRight = lookAndTellDistance(RIGHT);
  BTSerial.write("<L;" + distanceLeft + ';' + w.cnt + '>');
  if(lookAndTellDistance(80) <= 70 || lookAndTellDistance(100) <= 70){
    w.stop();
    exit();
  }
}

int lookAndTellDistance(byte angle) {
  unsigned long tot;
  unsigned int distance;

  serwo.write(angle);
  delay(100);  

  digitalWrite(TRIG, HIGH);
  delay(10);
  digitalWrite(TRIG, LOW);
  tot = pulseIn(ECHO, HIGH);

  distance = tot/58;
  return distance;
}
