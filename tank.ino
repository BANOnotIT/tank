// ----- настройки -----

// подключите один мотор к клемме: M1+ и M1-
// а второй к клемме: M2+ и M2-
// Motor shield использует четыре контакта 4, 5, 6, 7 для управления моторами
// 4 и 7 — для направления, 5 и 6 — для скорости
//
// DIR_*_INV - инвертировать значения, приходящее с управления
#define SPEED_L      5
#define DIR_L        4
#define DIR_L_INV    true

#define SPEED_R      6
#define DIR_R        7
#define DIR_R_INV    false


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


unsigned char stateMask;


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

  if (!Serial.available()) {
    return;
  }

  unsigned char newMask = Serial.read();
  bool changed = stateMask != newMask;


  if (changed) {
    stateMask = newMask;

    controlMotors();
  }

  delay(50);

}


void controlMotors() {

  Serial.print((int)stateMask);
  Serial.print(": ");

  // Левый мотор
  unsigned char leftMask = stateMask >> 4;
  Serial.print((int)leftMask);
  analogWrite(
    DIR_L, getDirectrionFromMask(leftMask, DIR_L_INV) * 255
  );
  analogWrite(SPEED_L, getVelocityFromMask(leftMask));

  Serial.print(' ');

  // Правый мотор
  unsigned char rightMask = stateMask & 0b1111;
  Serial.print((unsigned int)rightMask);
  analogWrite(
    DIR_R, getDirectrionFromMask(rightMask, DIR_R_INV) * 255
  );
  analogWrite(SPEED_R, getVelocityFromMask(rightMask));
  Serial.print('\n');

}



unsigned char getDirectrionFromMask(unsigned char mask, bool invert) {
  if ((mask >> 3) != invert)
    return HIGH;
  else
    return LOW;

}

unsigned char getVelocityFromMask(unsigned char mask) {

  return (mask & 0b0111) * 16;

}
