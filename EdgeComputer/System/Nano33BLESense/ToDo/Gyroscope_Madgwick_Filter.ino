#include "Arduino_LSM9DS1.h"
#include "MadgwickAHRS.h"

float gyro_x_cal, gyro_y_cal, gyro_z_cal, x, y, z; 

// initialize a Madgwick filter:
Madgwick filter;

// sensor's sample rate is fixed at 104 Hz:
//const float sensorRate = 104;
//const float sensorRate = 104.00;

const float sensorRate =IMU.gyroscopeSampleRate(); // Die Arduino_LSM9DS1 Bibliothek für den Nano 33 BLE Sense enthält eine Funktion zur Abfrage der Samplerate


void setup() {
  Serial.begin(9600);
  // attempt to start the IMU:
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU");
    // stop here if you can't access the IMU:
    while (true);
  }

 for (int cal_int = 0; cal_int < 2000 ; cal_int ++){              //Run this code 2000 times
    IMU.readGyroscope(x, y, z);                                   //Read the raw acc and gyro data from the MPU-6050
    gyro_x_cal += x;                                              //Add the gyro x-axis offset to the gyro_x_cal variable
    gyro_y_cal += y;                                              //Add the gyro y-axis offset to the gyro_y_cal variable
    gyro_z_cal += z;                                              //Add the gyro z-axis offset to the gyro_z_cal variable

  }
  gyro_x_cal /= 2000;                                             //Divide the gyro_x_cal variable by 2000 to get the avarage offset
  gyro_y_cal /= 2000;                                             //Divide the gyro_y_cal variable by 2000 to get the avarage offset
  gyro_z_cal /= 2000;                                             //Divide the gyro_z_cal variable by 2000 to get the avarage offset


  
  // start the filter to run at the sample rate:
  filter.begin(sensorRate);
}

void loop() {
  // values for acceleration and rotation:
  float xAcc, yAcc, zAcc;
  float xGyro, yGyro, zGyro;

  // values for orientation:
  float roll, pitch, heading;
  // check if the IMU is ready to read:
  if (IMU.accelerationAvailable() &&
      IMU.gyroscopeAvailable()) {
    // read accelerometer &and gyrometer:
    IMU.readAcceleration(xAcc, yAcc, zAcc);
    IMU.readGyroscope(xGyro, yGyro, zGyro);

    // 
    xGyro -= gyro_x_cal;
    yGyro -= gyro_y_cal;
    zGyro -= gyro_z_cal;
  

    // update the filter, which computes orientation:
    filter.updateIMU(xGyro, yGyro, zGyro, xAcc, yAcc, zAcc);

    // print the heading, pitch and roll
    roll = filter.getRoll();
    pitch = filter.getPitch();
    heading = filter.getYaw();
    Serial.print("Orientation: ");
    Serial.print(heading);
    Serial.print(" ");
    Serial.print(pitch);
    Serial.print(" ");
    Serial.println(roll);
 
  }
}
