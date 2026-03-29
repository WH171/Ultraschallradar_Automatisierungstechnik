#include <Wire.h>
#include <LSM9DS1.h>

#define LSM9DS1_SCK  13
#define LSM9DS1_MISO 12
#define LSM9DS1_MOSI 11
#define LSM9DS1_CS_AG 10
#define LSM9DS1_CS_M 9

LSM9DS1 imu;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  if (!imu.begin()) {
    Serial.println("Failed to communicate with LSM9DS1.");
    while (1);
  }
  
  imu.calibrate();
}

void loop() {
  imu.readGyro();
  float gyroZ = imu.calcGyro(imu.g.z);

  float angle = gyroZ * 0.01; // ajustez ce facteur selon vos besoins
  
  if (angle > 2) {
    Serial.println("Rotation vers la droite");
    Serial.print("Angle: ");
    Serial.println(angle);
  } else if (angle < -2) {
    Serial.println("Rotation vers la gauche");
    Serial.print("Angle: ");
    Serial.println(angle);
  }
}
