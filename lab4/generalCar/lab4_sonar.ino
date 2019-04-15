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
  w.attach(3,2,6,5,4,11);
  Serial.begin(9600);

  serwo.attach(SERVO);

/* rozejrzyj się w zakresie od 0 stopni (patrz na jedną burtę)
 *  do 180 stopni (patrz na prawą burtę). Wydrukuj na konsoli
 *  kierunek patrzenia i najbliżej widziany obiekt (pojedynczy pomiar)
 */
  for(byte angle = 0; angle < 180; angle+= 20) {
    lookAndTellDistance(angle);
    delay(500);
  }
  
/* patrz przed siebie */
  serwo.write(90);

}

void loop() { 
//  w.setSpeed(200);
//  w.forward();
//  checkPath();
//  delay(200);
  }

void checkPath(){
  if(lookAndTellDistance(90) <= 5){
    w.stop();
    if(lookAndTellDistance(0) > 5){
      serwo.write(90);
      w.turnRight(90);
    }else if(lookAndTellDistance(180) > 5){
      serwo.write(90);
      w.turnLeft(90);
    }else{
      rotRight(180);
      if(lookAndTellDistance(90) < 5){
        serwo.write(90);
        delay(250);
        rotRight(20);
        delay(250);
        rotLeft(40);
        delay(250);
        rotRight(20);
        Serial.println("Nie mam gdzie jechać");
        delay(10000);
      }
    }
  }
}

int lookAndTellDistance(byte angle) {
  
  unsigned long tot;      // czas powrotu (time-of-travel)
  unsigned int distance;

  Serial.print("Patrzę w kącie ");
  Serial.print(angle);
  serwo.write(angle);
  
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

  Serial.print(": widzę coś w odległości ");
  Serial.println(distance);

  return distance;
}
