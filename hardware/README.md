# Motherboard Hardware

These are the collected files for all the custom circuit boards for the Falcon aircraft project. All circuits were designed in KiCad.

## Revision 0.0

An initial design by a former team member, never actually produced. Merely kept for reference.

## Revision 1.0

First revision made by Aryan Ghosh, based on 0.0. Although produced it had some design issues and wasn't developed much from a firmware perspective.

## Revision 2.0

A revision of the previous revision to address some issues as well as implement novel features. Led by Ethan.

## Revision 3.0

A heavy revision of the previous motherboard, spurred by changes in the architecture of the aircraft's embedded system, namely the transition to using a CAN bus for most inter board communication. Other notable changes:

- Removal of the RPi header intended for ANT biometric readings, not used anymore
- On boarding of the orientation sensor (BNO055)
- Removal of the LIDAR header
- Removal of the obsoleted OneWire header and hardware
- Addition of EEPROM for storing possible configuration value outside of the main MCU
- Moving to a new sensor for temperature and humidity
- Embedding of a power sensor to monitor battery use
