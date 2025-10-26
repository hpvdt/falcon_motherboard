#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_can.h"
#include "wing_modules.h"
#include "can_wrapper.h"
#include <string.h>
#include "ads131m03.h"

static struct WingModuleConfig node[32];
static struct WingLoading current_loading;

void wing_setup(CAN_HandleTypeDef* can) {
	struct CANNodeGeneralConfigCommand default_general = {
		.driving_servo = false,
		.measuring_lidar = false,
		.measuring_strain = false,
		.measuring_torsion = false,
		.operating_indicator = false,
		.update_period_ms = 200,
	};
	struct CANServoConfigCommand default_servo = {
		.misalignment_alarm_sec = 0,
		.position_tolerance_half_percent = 20,
		.potentiometer_bottom = 0,
		.potentiometer_top = 4095,
		.scheme = CONTROL_SCHEME_OPEN,
		.servo_bottom_us = 1500,
		.servo_top_us = 2500,
		.surface = CONTROL_SURFACE_PORT,
	};
	struct CANStrainGaugeConfigCommand default_strain_torsion = {
		.buffer_depth_64_samples = 3,
		.gain = ADS_PGA_GAIN_32,
		.osr = ADS_OSR_RATIO_4096,
		.scaling_factor = 1.0,
		.zero_point = 0,
	};
	struct CANLidarConfigCommand default_lidar = {
		.alarm_level = 3000,
		.buffer_depth = 200,
	};
	struct CANIndicatorConfigCommand default_indicator = {
		.buzzer_period_ms = 2000,
		.disable_buzzer = false,
		.invert_led = 0,
	};

	for (uint_fast8_t n = 0; n < 32; n++) {
		node[n].node_id = n; // Tagged for easier debugging as it might get passed around
		node[n].stress_location = WING_LOCATION_INVALID;
		node[n].general = default_general;
		node[n].indicator = default_indicator;
		node[n].lidar = default_lidar;
		node[n].servo = default_servo;
		node[n].strain = default_strain_torsion;
		node[n].torsion = default_strain_torsion;
	}

	node[7].stress_location = WING_LOCATION_CENTER;
	node[7].general.measuring_strain = true;
	node[7].strain.scaling_factor = 1.0;
	node[7].strain.zero_point = 0;

	node[8].stress_location = WING_LOCATION_STAR_1;
	node[8].general.measuring_strain = true;
	node[8].strain.scaling_factor = 1.0;
	node[8].strain.zero_point = 0;
	node[8].general.measuring_torsion = true;
	node[8].torsion.scaling_factor = 1.0;
	node[8].torsion.zero_point = 0;

	node[5].stress_location = WING_LOCATION_STAR_2;
	node[5].general.measuring_strain = true;
	node[5].strain.scaling_factor = 1.0;
	node[5].strain.zero_point = 0;

	node[2].stress_location = WING_LOCATION_STAR_3;
	node[2].general.measuring_strain = true;
	node[2].strain.scaling_factor = 1.0;
	node[2].strain.zero_point = 0;

	node[4].stress_location = WING_LOCATION_PORT_1;
	node[4].general.measuring_strain = true;
	node[4].strain.scaling_factor = 1.0;
	node[4].strain.zero_point = 0;

	node[6].stress_location = WING_LOCATION_PORT_2;
	node[6].general.measuring_strain = true;
	node[6].strain.scaling_factor = 1.0;
	node[6].strain.zero_point = 0;

	node[1].stress_location = WING_LOCATION_PORT_3;
	node[1].general.measuring_strain = true;
	node[1].strain.scaling_factor = 1.0;
	node[1].strain.zero_point = 0;


	HAL_CAN_Start(can);

	// Configure all nodes before accepting any messages
	for (uint_fast8_t n = 0; n < 32; n++) wing_config_node(can, &node[n]);

	HAL_CAN_Stop(can);

	uint16_t id_filter = CAN_BROADCAST_LOAD_ID_BASE;
	id_filter = (id_filter << 5) | CAN_IGNORE_EXTENDED_ID_HIGH;

	uint16_t mask_filter = CAN_NODE_ADDRESS_MASK;
	mask_filter = (mask_filter << 5) | CAN_IGNORE_EXTENDED_MASK_HIGH;

	CAN_FilterTypeDef filter_for_loads = {
		.FilterMode = CAN_FILTERMODE_IDMASK,
		.FilterScale = CAN_FILTERSCALE_32BIT,
		.FilterActivation = CAN_FILTER_ENABLE, 		// Always enabled
		.FilterFIFOAssignment = CAN_LOAD_FIFO,
		.FilterBank = CAN_READING_LOAD_BANK,
		.FilterIdHigh = id_filter,
		.FilterIdLow = CAN_IGNORE_EXTENDED_ID_LOW,
		.FilterMaskIdHigh = mask_filter,
		.FilterMaskIdLow = CAN_IGNORE_EXTENDED_MASK_LOW,
	};

	HAL_CAN_ConfigFilter(can, &filter_for_loads);

	HAL_CAN_Start(can);
}

void wing_config_node(CAN_HandleTypeDef* can, struct WingModuleConfig* config) {
	const uint32_t CONFIG_TIMEOUT_MS = 5;

	uint8_t tx_buffer[8] = {0};
	const uint32_t BASE_ID = config->node_id | CAN_CONFIG_ID_BASE; // Base value, needs to get specific

	CAN_TxHeaderTypeDef temp_header = {
		.StdId = BASE_ID,
		.RTR = CAN_RTR_DATA,
		.IDE = CAN_ID_STD,
		.ExtId = 0,
		.TransmitGlobalTime = false,
	};
	temp_header.StdId = BASE_ID | (CONFIG_MESSAGE_GENERAL << CAN_CONFIG_BIT_START);
	temp_header.DLC = can_pack_node_general_config_command(tx_buffer, config->general);

	can_send_message(can, temp_header, tx_buffer, CONFIG_TIMEOUT_MS); // All nodes need general config

	// Send only the required additional configuration
	if (config->general.driving_servo) {
		temp_header.StdId = BASE_ID | (CONFIG_MESSAGE_SERVO << CAN_CONFIG_BIT_START);
		temp_header.DLC = can_pack_servo_config_command(tx_buffer, config->servo);
		can_send_message(can, temp_header, tx_buffer, CONFIG_TIMEOUT_MS);
	}
	if (config->general.measuring_lidar) {
		temp_header.StdId = BASE_ID | (CONFIG_MESSAGE_LIDAR << CAN_CONFIG_BIT_START);
		temp_header.DLC = can_pack_lidar_config_command(tx_buffer, config->lidar);
		can_send_message(can, temp_header, tx_buffer, CONFIG_TIMEOUT_MS);
	}
	if (config->general.measuring_strain) {
		temp_header.StdId = BASE_ID | (CONFIG_MESSAGE_STRAIN << CAN_CONFIG_BIT_START);
		temp_header.DLC = can_pack_strain_gauge_config_command(tx_buffer, config->strain);
		can_send_message(can, temp_header, tx_buffer, CONFIG_TIMEOUT_MS);
	}
	if (config->general.driving_servo) {
		temp_header.StdId = BASE_ID | (CONFIG_MESSAGE_TORSION << CAN_CONFIG_BIT_START);
		temp_header.DLC = can_pack_strain_gauge_config_command(tx_buffer, config->torsion);
		can_send_message(can, temp_header, tx_buffer, CONFIG_TIMEOUT_MS);
	}
	if (config->general.operating_indicator) {
		temp_header.StdId = BASE_ID | (CONFIG_MESSAGE_INDICATOR << CAN_CONFIG_BIT_START);
		temp_header.DLC = can_pack_indicator_config_command(tx_buffer, config->indicator);
		can_send_message(can, temp_header, tx_buffer, CONFIG_TIMEOUT_MS);
	}
}

void wing_record_strain(uint16_t can_message_id, uint8_t* rx_buffer) {
	uint_fast8_t node_id = can_message_id & ~CAN_NODE_ADDRESS_MASK;

	enum WingStressLocation location = node[node_id].stress_location;
	if (location == WING_LOCATION_INVALID) return;

	struct CANLoadBroadcast temp;
	can_unpack_load_broadcast(rx_buffer, &temp);

	switch (location) {
	case WING_LOCATION_CENTER:
		current_loading.strain_center = temp.strain_reading;
		break;
	case WING_LOCATION_STAR_1:
		current_loading.strain_starboard[0] = temp.strain_reading;
		current_loading.torsion = temp.torsion_reading;
		break;
	case WING_LOCATION_STAR_2:
		current_loading.strain_starboard[1] = temp.strain_reading;
		break;
	case WING_LOCATION_STAR_3:
		current_loading.strain_starboard[2] = temp.strain_reading;
		break;
	case WING_LOCATION_PORT_1:
		current_loading.strain_port[0] = temp.strain_reading;
		break;
	case WING_LOCATION_PORT_2:
		current_loading.strain_port[1] = temp.strain_reading;
		break;
	case WING_LOCATION_PORT_3:
		current_loading.strain_port[2] = temp.strain_reading;
		break;
	case WING_LOCATION_INVALID: // Fall through to default
	default:
		return;
	}
}

void wing_report_strain(struct WingLoading* destination) {
	memcpy(destination, &current_loading, sizeof(struct WingLoading));
}

void wing_set_strain_zero(CAN_HandleTypeDef* can, uint8_t node, int32_t zero);
void wing_set_strain_scale(CAN_HandleTypeDef* can, uint8_t node, float scale);
void wing_set_strain_gain(CAN_HandleTypeDef* can, uint8_t node, uint8_t gain);

void wing_set_torsion_zero(CAN_HandleTypeDef* can, uint8_t node, int32_t zero);
void wing_set_torsion_scale(CAN_HandleTypeDef* can, uint8_t node, float scale);
void wing_set_torsion_gain(CAN_HandleTypeDef* can, uint8_t node, uint8_t gain);

void wing_flash_node(CAN_HandleTypeDef* can, uint8_t node, uint8_t count, uint16_t period_on_ms, uint16_t period_off_ms) {
	const uint8_t HEART_BEAT_MAX = 80;
	const uint8_t HEART_BEAT_MIN = 0;

	struct CANLightCommand msg = {
			.heart_beat_max_duty = HEART_BEAT_MAX,
			.heart_beat_min_duty = HEART_BEAT_MIN,
			.pulse_count = count,
			.pulse_period_on_ms = period_on_ms,
			.pulse_period_off_ms = period_off_ms,
	};

	uint8_t tx_buffer[8] = {0};
	const uint32_t COMMAND_ID = node | CAN_COMMAND_LIGHT_ID_BASE; // Base value, needs to get specific

	CAN_TxHeaderTypeDef temp_header = {
		.StdId = COMMAND_ID,
		.RTR = CAN_RTR_DATA,
		.IDE = CAN_ID_STD,
		.ExtId = 0,
		.TransmitGlobalTime = false,
	};
	temp_header.DLC = can_pack_light_command(tx_buffer, msg);

	can_send_message(can, temp_header, tx_buffer, 10);
}
