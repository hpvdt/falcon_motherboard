// Code originally written by George Wang, imported from
// the Aircraft_Data repository

#include "BNO_orientation.h"

// Need to put this in the setup function and take the i2c bus as an input, can't use default bus
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);

uint16_t BNO055_SAMPLERATE_DELAY_MS = 100;

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address

void BNO_measurements(float pitch, float roll, float heading)
{ 
    sensors_event_t orientationData;
    bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER); // get orientation, euler vectors ok up to 45 degrees
    pitch = orientationData.orientation.pitch;
    roll = orientationData.orientation.z;
    heading = orientationData.orientation.x; // some issues with the .heading, .pitch stuff
}

void BNO_setup()
{
    
    if (!bno.begin())
    {

        // Something went wrong...
        SerialUSB.println("BNO not worketh");
        while (1)
            ;
    }

    delay(1000);

    bno.setExtCrystalUse(true);
}

/* This driver uses the Adafruit unified sensor library (Adafruit_Sensor),
   which provides a common 'type' for sensor data and some helper functions.

   To use this driver you will also need to download the Adafruit_Sensor
   library and include it in your libraries folder.

   You should also assign a unique ID to this sensor for use with
   the Adafruit Sensor API so that you can identify this particular
   sensor in any data logs, etc.  To assign a unique ID, simply
   provide an appropriate value in the constructor below (12345
   is used by default in this example).

   Connections
   ===========
   Connect SCL to analog 5 for arduino nano
    Actually B6 for STM32?
   Connect SDA to analog 4
    Actually to B7
   Connect VDD (actually VIN) to 3.3-5V DC
    Picked 5V for vin
   Connect GROUND to common ground
    G

   History
   =======
   2015/MAR/03  - First release (KTOWN)
*/