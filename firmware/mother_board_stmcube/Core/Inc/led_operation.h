#ifndef INC_LED_OPERATION_C_
#define INC_LED_OPERATION_C_

#include <stdbool.h>
#include <stdint.h>

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"

enum LEDChannel {
	LED_ERROR		= 0,
	LED_INDICATOR	= 1,
	LED_HEARTBEAT	= 2,
	LED_COUNT
};

struct LEDMoBoConfig {
	TIM_HandleTypeDef* error_timer;
	uint32_t error_channel;
	TIM_HandleTypeDef* heartbeat_timer;
	uint32_t heartbeat_channel;
	TIM_HandleTypeDef* indicator_timer;
	uint32_t indicator_channel;

	bool indicate_error; // Override error LED to indicate an error in the system
};

HAL_StatusTypeDef led_setup(struct LEDMoBoConfig* user_config);

void led_operate(struct LEDMoBoConfig* user_config);

void led_blink(enum LEDChannel channel, uint_fast8_t count, uint_fast16_t pulse_duration_ms);

#endif /* INC_LED_OPERATION_C_ */
