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
volatile int counter = 0;
volatile int turns = 0;

void setup() {
  pinMode(TRIG, OUTPUT);    // TRIG startuje sonar
  pinMode(ECHO, INPUT);     // ECHO odbiera powracający impuls
  w.attach(8,2,6,5,4,11);
  Serial.begin(9600);

  serwo.attach(SERVO);

/* rozejrzyj się w zakresie od 0 stopni (patrz na jedną burtę)
 *  do 180 stopni (patrz na prawą burtę). Wydrukuj na konsoli
 *  kierunek patrzenia i najbliżej widziany obiekt (pojedynczy pomiar)
 */
//  for(byte angle = 0; angle < 180; angle+= 20) {
//    lookAndTellDistance(angle);
//    delay(500);
//  }
  
/* patrz przed siebie */
  serwo.write(90);
  delay(500);
    serwo.write(0);
  delay(500);
    serwo.write(180);
  delay(500);
     serwo.write(90);
  delay(500);
  w.goForward(130);
  w.turnLeft(90);
  w.setSpeed(200);
}

void loop() { 
  if (turns < 2){
    w.forward();
    checkPath();
    delay(200);
  } else {
    w.setSpeed(200);
    while(counter > w.cntr){
      w.forward();
    }
    w.stop();
    w.turnRight(90);
    w.goForward(200);
    delay(100000);
  }
}

void checkPath(){
  int disctance = lookAndTellDistance(90);
  if(lookAndTellDistance(70) <= 70 || lookAndTellDistance(110) <= 70){
    w.stop();
    counter = w.cntr;
    if(lookAndTellDistance(180) > 50){
      serwo.write(90);
      w.turnLeft(90);
    }else if(lookAndTellDistance(0) > 50){
      serwo.write(90);
      w.turnRight(90);
    }else{
      w.turnRight(180);
      if(lookAndTellDistance(90) < 50){
        delay(200);
        serwo.write(90);
        delay(250);
        w.turnRight(20);
        delay(250);
        w.turnLeft(40);
        delay(250);
        w.turnRight(20);
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

  Serial.print(": widzę coś w odległości ");
  Serial.println(distance);

  return distance;
}
