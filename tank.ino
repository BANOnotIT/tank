// ----- настройки -----

// подключите один мотор к клемме: M1+ и M1-
// а второй к клемме: M2+ и M2-
// Motor shield использует четыре контакта 4, 5, 6, 7 для управления моторами
// 4 и 7 — для направления, 5 и 6 — для скорости
#define SPEED_L      5
#define DIR_L        4
#define DIR_L_I      true

#define SPEED_R      6
#define DIR_R        7
#define DIR_R_I      true


// ---- \настройки/ ----
#include <SoftwareSerial.h>

#define SPEED_MASK 0b01111111
#define DIR_MASK 0b10000000

char vel[2];


void setup() {

  // настраиваем выводы платы 4, 5, 6, 7 на вывод сигналов
  for (int i = 4; i < 8; i++) {
    pinMode(i, OUTPUT);
  }

  Serial.begin(9600);
  while (!Serial) { ; }  // wait for serial port to connect. Needed for native USB port only

}

void loop() {

  if (Serial.available() == 2) {
  	vel[0] = Serial.read();
  	vel[1] = Serial.read();
  }

  while (Serial.available())
  	Serial.read();

}

void controlMotors() {

  analogWrite(
  	DIR_L, 
  	(bool)(vel[0] & DIR_MASK) ^ DIR_L_I ?
  	  HIGH :
  	  LOW
  );
  analogWrite(SPEED_L, vel[0] & SPEED_MASK);


  analogWrite(
  	DIR_R, 
  	(bool)(vel[1] & DIR_MASK) ^ DIR_R_I ?
  	  HIGH :
  	  LOW
  );
  analogWrite(SPEED_R, vel[1] & SPEED_MASK);

}
