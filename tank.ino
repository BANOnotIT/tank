// ----- настройки -----

// подключите один мотор к клемме: M1+ и M1-
// а второй к клемме: M2+ и M2-
// Motor shield использует четыре контакта 4, 5, 6, 7 для управления моторами
// 4 и 7 — для направления, 5 и 6 — для скорости
//
// DIR_*_INV - инвертировать значения, приходящее с управления,
#define SPEED_L      5
#define DIR_L        4
#define DIR_L_INV    false

#define SPEED_R      6
#define DIR_R        7
#define DIR_R_INV    true


// ---- \настройки/ ----
#include <SoftwareSerial.h>


/*

Маска состояния:
 L   R
 --------
 00000000
 |||||^^^ - Скорость правого мотора
 ||||^ - Направление правого мотора
 |^^^ - Скорость левого мотора
 ^ - Направление левого мотора

*/


char stateMask;


void setup() {

  // настраиваем выводы платы 4, 5, 6, 7 на вывод сигналов
  for (int i = 4; i < 8; i++) {
    pinMode(i, OUTPUT);
  }

  Serial.begin(9600);
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }

  controlMotors();

}

void loop() {
  delay(50);

  if (!Serial.available()) {
    return;
  }

  char val = Serial.read();
  bool changed = stateMask != val;

  
  if (changed){
  	stateMask = val;

  	controlMotors();
  }

}


void controlMotors() {

  // Левый мотор
  char leftMask = stateMask >> 4;
  analogWrite(
    DIR_L, getDirectrionFromMask(leftMask, DIR_L_I)
  );
  analogWrite(SPEED_L, getVelocityFromMask(leftMask));


  // Правый мотор
  char rightMask = stateMask & 0b1111;
  analogWrite(
    DIR_R, getDirectrionFromMask(rightMask, DIR_R_I)
  );
  analogWrite(SPEED_R, getVelocityFromMask(rightMask));

}



char getDirectrionFromMask(char mask, bool invert) {

  if (((mask & 0b1000) == 0b1000) != invert)
  	return HIGH;
  else 
  	return LOW;

}

char getVelocityFromMask(char mask) {

  return (mask & 0b0111) * 16;

}