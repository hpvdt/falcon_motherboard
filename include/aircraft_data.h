#ifndef _AIRCRAFT_DATA_STRUCTS_H
#define _AIRCRAFT_DATA_STRUCTS_H

typedef struct _IMUAxes {
    struct {
        float x;
        float y;
        float z;
    } lin;

    struct {
        float x;
        float y;
        float z;
    } rot;
} IMUAxes;

typedef struct _AircraftState {

    struct {
        float roll;
        float pitch;
        float yaw;
        float altitude;
        float speed;
        float height;
    } estimate;

    struct {
        float roll;
        float pitch;
        float yaw;
    } orientation;

    struct {
        float altitude;
        float latitude;
        float longitude;
        float speed;
    } gps;

    struct {
        float temperature;
        float pressure;
        float humidity;
        int co2;
    } environment;

    struct {
        float shaft;
        float propellor;
    } power;

    struct {
        float x;
        float y;
        float z;
    } wind;
} AircraftState;

#endif