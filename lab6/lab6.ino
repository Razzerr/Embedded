#include <Servo.h>
#include <IRremote.h>
#include "TimerOne.h"
#include "Wheels.h"

#define KEY1 0xFFA25D
#define KEY2 0xFF629D
#define KEY3 0xFFE21D
#define KEY4 0xFF22DD
#define KEY5 0xFF02FD
#define KEY6 0xFFC23D
#define KEY7 0xFFE01F
#define KEY8 0xFFA857
#define KEY9 0xFF906F
#define KEY0 0xFF9867
#define KEYStar 0xFF6897
#define KEYHash 0xFFB04F
#define KEYOk 0xFF38C7
#define KEYUp 0xFF18E7
#define KEYDown 0xFF4AB5
#define KEYRight 0xFF5AA5
#define KEYLeft 0xFF10EF

#define SPEED 255
#define RECV_PIN 2
// piny dla sonaru (HC-SR04)
#define TRIG A4
#define ECHO A5
// pin kontroli serwo (musi być PWM)
#define SERVO 3

Servo serwo;
Wheels w;
IRrecv irrecv(RECV_PIN);
int speed = 0;

void setup() {
//  pinMode(TRIG, OUTPUT);    // TRIG startuje sonar
//  pinMode(ECHO, INPUT);     // ECHO odbiera powracający impuls
  w.attach(8,12,6,5,4,13);
  int step = 0;
  bool accessed = false;
  Serial.begin(9600);
//  serwo.attach(SERVO);
  
  irrecv.enableIRIn();
  decode_results results;
  while (!accessed){
    delay(200);
    irrecv.decode(&results); 
    Serial.println(results.value, HEX);
    switch (results.value){
      case KEY1: 
        if (step == 0){
          step = 1;
        } else if (step != 1){
          step = 0;
        }
        break;
      case KEY9: 
        if (step == 1){
          step = 2;
        } else if (step != 2){
          step = 0;
        }
        break;
      case KEY7: 
        if (step == 2){
          step = 3;
        } else if (step != 3){
          step = 0;
        }
        break;
      case KEY2: 
        if (step == 3){
          step = 4;
        } else if (step != 4){
          step = 4;
        }
        break;
      case KEYOk:
        accessed = true;
        break;
      case 0xFF:
        break;
      case 0xFFFFFFFF:
        break;
      default:
        step = 0;
        break;
    }
    irrecv.resume();
  }
 
}

void loop() { 
  delay(200);
  decode_results results;
  irrecv.decode(&results); 
    switch (results.value){
      case KEYOk:
        speed = 0;
        w.setSpeed(0);
        w.stop();
        break;
      case KEYUp:
        if (speed == 0) {
          speed = 150;
        } else if (speed == 150) {
          speed = 200;
        } else {
          speed = 255;
        }
        w.setSpeed(speed);
        w.forward();
        break;
      case KEYDown:
        if (speed == 0) {
          speed = 150;
        } else if (speed == 150) {
          speed = 200;
        } else {
          speed = 255;
        }
        w.setSpeed(speed);
        w.back();
        break;
      case KEYLeft:
        w.setSpeed(150);
        w.turnLeft(30);
        break;
      case KEYRight:
        w.setSpeed(150);
        w.turnRight(30);
        break;
    }
  irrecv.resume();
//  int distance = lookAndTellDistance(90);
//  distance = (distance + lookAndTellDistance(90))/2;
//  if (distance <= 95){
//    w.setSpeed(200-2*distance);
//    w.back();
//  } else if (distance >= 105){
//    w.setSpeed(2*distance-180);
//    w.forward();
//  } else {
//    w.stop();
//  }
//  delay(50); 
}

int lookAndTellDistance(byte angle) {
  unsigned long tot;      // czas powrotu (time-of-travel)
  unsigned int distance;

  serwo.write(angle);
  delay(50);  
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
