//Finally prints out heading, roll and pitch reasonably well

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>


/* Set the delay between fresh samples */
const uint16_t BNO055_SAMPLERATE_DELAY_MS = 100;

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);


//Need info: pitch roll heading
//heading v yaw??
float pitch=-1;
float roll=-1;
float heading=-1;

void orientation_measure(){//unneeded wrapper function
  sensors_event_t orientationData;
  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);//get orientation
  pitch=orientationData.orientation.pitch;
  roll=orientationData.orientation.roll;
  heading=orientationData.orientation.heading;//some issues with the .heading, .pitch stuff

}

void setup(void)
{
  Serial.begin(9600);//for bluetooth
  Serial.println("----------------");
  Serial.println("PAIN SUFFERING]!!!!!!!!!!"); Serial.println("");
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // Serial.println("Orientation Sensor Test"); Serial.println("");

  while (!Serial) {
    delay(10);  // wait for serial port to open!
  }
  Serial.println("Orientation Sensor Test"); Serial.println("");

  /* Initialise the sensor */
  if (!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
  }
  // orientation_measure();
  // Serial.print("Wrapper function test:\n h:");
  // Serial.print(heading);
  // Serial.print(", pitch:");
  // Serial.print(pitch);
  // Serial.print(", roll:");
  // Serial.println(roll);

  delay(1000);
  uint8_t system, gyro, accel, mag = 0;

  Serial.println(bno.getMode());
  //bno.setMode(Adafruit_BNO055::OPERATION_MODE_NDOF); //https://forums.adafruit.com/viewtopic.php?f=25&p=503208
  
  bno.setMode(OPERATION_MODE_NDOF); //bullshit
 
  Serial.print("Normal NDOF mode check: "); //https://forum.pjrc.com/threads/55581-bno055-problem-code
  Serial.println(bno.getMode());
//  bno.setMode(adafruit_bno055_opmode_t NDOF);
  
  //Serial.println(bno.isFullyCalibrated());//Calibration is 0

  Serial.println("Calibration of 3 means fully calibrated, h=heading, p=pitch, r=roll");
  Serial.println("All measurements in degrees");
}

void loop(void)
{
  //could add VECTOR_ACCELEROMETER, VECTOR_MAGNETOMETER,VECTOR_GRAVITY...
  sensors_event_t orientationData;

  bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
 
  //bno.setMode(adafruit_bno055_opmode_t NDOF_MODE);
  bno.enterNormalMode(); //MAIN ADDITION THAT PREVENTS IT ENTERING LOW POWER MODE always

uint8_t system, gyro, accel, mag = 0;

  bno.getCalibration(&system, &gyro, &accel, &mag);
  Serial.print("Cal: Sys,");
  Serial.print(system);
  Serial.print(",Gyro,");
  Serial.print(gyro);
  Serial.print(",Accel,");
  Serial.print(accel);
  Serial.print(",Mag,");
  Serial.println(mag); //shows calibration

  Serial.print("h:,");
  Serial.print(orientationData.orientation.x);//direct print
  Serial.print(",p:,");
  Serial.print(orientationData.orientation.z);
  //Serial.print(orientationData.orientation.)
  Serial.print(",r:,");//tabs suck in b
  Serial.print(orientationData.orientation.y);
  Serial.print(',');
  // Serial.print(" is calibrated: ");
  // Serial.print(bno.isFullyCalibrated());

  




  delay(1000);//BNO055_SAMPLERATE_DELAY_MS
}









