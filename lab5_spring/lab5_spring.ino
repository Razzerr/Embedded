#include <Servo.h>
#include "TimerOne.h"
#include "Wheels.h"

#define SPEED 255
// piny dla sonaru (HC-SR04)
#define TRIG A4
#define ECHO A5
// pin kontroli serwo (musi być PWM)
#define SERVO 3

Servo serwo;
Wheels w;

void setup() {
  pinMode(TRIG, OUTPUT);    // TRIG startuje sonar
  pinMode(ECHO, INPUT);     // ECHO odbiera powracający impuls
  w.attach(8,2,6,5,4,11);
  Serial.begin(9600);

  serwo.attach(SERVO);
}

void loop() { 
  int distance = lookAndTellDistance(90);
  if (distance <= 95 || lookAndTellDistance(85) <= 95 || lookAndTellDistance(95) <= 95){
    w.setSpeed(100+pow((12-(0.12)*distance), 2));
    w.back();
  } else if (distance >= 105 || lookAndTellDistance(85) >= 105 || lookAndTellDistance(95) >= 105){
    w.setSpeed(100+pow(((0.12)*distance-12), 2));
    w.forward();
  } else {
    w.stop();
  }
}

int lookAndTellDistance(byte angle) {
  unsigned long tot;      // czas powrotu (time-of-travel)
  unsigned int distance;

  serwo.write(angle);
  delay(100);  
/* uruchamia sonar (puls 10 ms na `TRIGGER')
 * oczekuje na powrotny sygnał i aktualizuje
 */
  digitalWrite(TRIG, HIGH);
  delay(10);
  digitalWrite(TRIG, LOW);
  tot = pulseIn(ECHO, HIGH);
  

/* prędkość dźwięku = 340m/s => 1 cm w 29 mikrosekund
 * droga tam i z powrotem, zatem:
 */
  distance = tot/58;
  return distance;
}
