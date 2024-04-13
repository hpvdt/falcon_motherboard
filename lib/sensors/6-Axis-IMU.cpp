#include "6-Axis-IMU.h"

SPIClass IMUspi(PB15, PB14, PB13); // Initialize specific SPI bus.
ICM42688 IMU(IMUspi, PB10);        // initializes an ICM chip object, specifying
                                   // protocol + pin select for serial communication
                                   // defaults to 8MHz clock, but a third parameter can be
                                   // passed to specify up to 24 MHz.
                                   // PB10 is chip select pin.

void setupIMU() {
    // hi

      // start communication with IMU
  SerialUSB.println("Initializing...");
  int status = IMU.begin();

  if (status < 0)
  {
    SerialUSB.println("IMU initialization failed");
    SerialUSB.print("Status: ");
    SerialUSB.println(status);
    // Not doing anything after initialization fails... maybe change this later
  }
}


void measureIMU(double* accX, double* accY, double* accZ, double* gyrX, double* gyrY, double* gyrZ, double* tempIMU) {

  IMU.getAGT(); // grab newest readings for accel. and gyro.
  
  *accX = IMU.accX();
  *accY = IMU.accY();
  *accZ = IMU.accZ();

  *gyrX = IMU.gyrX();
  *gyrY = IMU.gyrY();
  *gyrZ = IMU.gyrZ();

  *tempIMU = IMU.temp();

}

void printIMU() {
  SerialUSB.println("ICM42688 Readings ~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  SerialUSB.println("Accelerometer Readings");
  SerialUSB.print("a_x: ");
  SerialUSB.println(IMU.accX());
  SerialUSB.print("a_y: ");
  SerialUSB.println(IMU.accY());
  SerialUSB.print("a_z: ");
  SerialUSB.println(IMU.accZ());
  SerialUSB.println("Gyroscope Readings");
  SerialUSB.print("g_x: ");
  SerialUSB.println(IMU.gyrX());
  SerialUSB.print("g_y: ");
  SerialUSB.println(IMU.gyrY());
  SerialUSB.print("g_z: ");
  SerialUSB.println(IMU.gyrZ());
  SerialUSB.print("ICM42688 Temperature: ");
  SerialUSB.println(IMU.temp());
}