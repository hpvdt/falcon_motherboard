/*
 * falcon_data.h
 *
 *  Created on: Nov 16, 2025
 *      Author: savo
 */

#ifndef INC_FALCON_DATA_H_
#define INC_FALCON_DATA_H_

struct OrientationState {
	float roll_deg;
	float pitch_deg;
	float yaw_deg;
};

struct GPSState{
    float altitude_m;
    float latitude_deg;
    float longitude_deg;
    float speed_m_s;
};

struct AtmoConditions {
	float temperature_c;
	float humidity_rel;
	float static_pressure_pa;
	uint16_t co2_ppm;

    struct {
        float x_m_s;
        float y_m_s;
        float z_m_s;
    } wind;
};

struct WingLoading {
	float strain_center;
	float strain_starboard[3];
	float strain_port[3];
	float torsion;
};

struct CompleteAircraftData {
	struct OrientationState orientation;
	struct AtmoConditions atmo;
	struct WingLoading wing_load;

    struct {
        float shaft_w;
        float propellor_w;
    } power;

    struct GPSState gps;
};

#endif /* INC_FALCON_DATA_H_ */
