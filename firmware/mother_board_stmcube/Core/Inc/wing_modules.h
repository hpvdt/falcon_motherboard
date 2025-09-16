#ifndef INC_WING_MODULES_H_
#define INC_WING_MODULES_H_

#include <stdint.h>
#include "can_wrapper.h"

enum WingStressLocation {
	WING_LOCATION_INVALID = 0, // Default (0) should be invalid
	WING_LOCATION_CENTER,
	WING_LOCATION_TORSION_1,
	WING_LOCATION_STAR_1,
	WING_LOCATION_STAR_2,
	WING_LOCATION_STAR_3,
	WING_LOCATION_PORT_1,
	WING_LOCATION_PORT_2,
	WING_LOCATION_PORT_3,
};

struct WingModuleConfig {
	uint_fast8_t node_id;
	enum WingStressLocation stress_location;

	struct CANNodeGeneralConfigCommand general;
	struct CANServoConfigCommand servo;
	struct CANStrainGaugeConfigCommand strain;
	struct CANStrainGaugeConfigCommand torsion;
	struct CANLidarConfigCommand lidar;
	struct CANIndicatorConfigCommand indicator;
};

struct WingLoading {
	float strain_center;
	float strain_starboard[3];
	float strain_port[3];
	float torsion;
};

void wing_setup(CAN_HandleTypeDef* can);
void wing_config_node(CAN_HandleTypeDef* can, struct WingModuleConfig* config);
void wing_record_strain(uint16_t can_message_id, uint8_t* rx_buffer);
void wing_report_strain(struct WingLoading* destination);

#endif /* INC_WING_MODULES_H_ */
