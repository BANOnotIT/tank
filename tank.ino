// ----- настройки -----

// подключите один мотор к клемме: M1+ и M1-
// а второй к клемме: M2+ и M2-
// Motor shield использует четыре контакта 4, 5, 6, 7 для управления моторами
// 4 и 7 — для направления, 5 и 6 — для скорости
#define SPEED_L      5
#define DIR_L        4
#define DIR_L_I      false

#define SPEED_R      6
#define DIR_R        7
#define DIR_R_I      true


// ---- \настройки/ ----
#include <SoftwareSerial.h>

char vel = 0b11111111;


void setup() {

  // настраиваем выводы платы 4, 5, 6, 7 на вывод сигналов
  for (int i = 4; i < 8; i++) {
    pinMode(i, OUTPUT);
  }

  Serial.begin(9600);
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }

  analogWrite(SPEED_L, HIGH);

  controlMotors();

}

void loop() {
  delay(50);

  if (Serial.available() != 2) {
    return;
  }


  char val = Serial.read();

  bool changed = vel != val;

  vel = val;

  if (changed)
    controlMotors();

}


void controlMotors() {

  // Левый мотор
  analogWrite(
    DIR_L,
    (vel & 0b00001000) ?
    HIGH :
    LOW
  );
  analogWrite(SPEED_L, (vel & 0b00001111) ? HIGH : LOW);


  /*// Правый мотор
  analogWrite(
    DIR_R,
    vel[1] >> 7 ?
    HIGH :
    LOW
  );
  analogWrite(SPEED_R, vel[1] & SPEED_MASK << 1);
*/
}
