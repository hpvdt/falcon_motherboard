#include <stdbool.h>
#include <stdint.h>

#include "led_operation.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"

static const uint32_t LED_HEARTBEAT_PERIOD_MS = 20;
static const uint16_t LED_HEARTBEAT_MAXIMUM_DUTY = UINT16_MAX;
static const uint16_t LED_HEARTBEAT_MINIMUM_DUTY = 0;
static const int16_t LED_HEARTBEAT_INCREMENT = 1000;

static const uint16_t LED_ERROR_ACTIVE_DUTY = UINT16_MAX;

struct LEDBlinkTracker {
	bool current_flash_state;

	uint_fast8_t number_remaining;
	uint_fast16_t pulse_half_period_ms;

	uint32_t next_change_tick;
};
static struct LEDBlinkTracker led_pulses[3];

HAL_StatusTypeDef led_setup(struct LEDMoBoConfig* user_config) {
	HAL_StatusTypeDef ret_val = HAL_OK;
	HAL_StatusTypeDef captured_val;

	captured_val = HAL_TIM_PWM_Start(user_config->error_timer, user_config->error_channel);
	if (captured_val != HAL_OK) ret_val = captured_val;
	captured_val = HAL_TIM_PWM_Start(user_config->indicator_timer, user_config->indicator_channel);
	if (captured_val != HAL_OK) ret_val = captured_val;
	captured_val = HAL_TIM_PWM_Start(user_config->heartbeat_timer, user_config->heartbeat_channel);
	if (captured_val != HAL_OK) ret_val = captured_val;

	if (ret_val != HAL_OK) return ret_val;

	const uint16_t STEPS = 100;
	const uint16_t STEP_PERIOD_MS = 20;
	const uint16_t INCREMENT = UINT16_MAX / STEPS;

	TIM_OC_InitTypeDef pwm_config = {0};
	pwm_config.OCMode = TIM_OCMODE_PWM1;
	pwm_config.OCPolarity = TIM_OCPOLARITY_HIGH;
	pwm_config.OCFastMode = TIM_OCFAST_DISABLE;

	for (int32_t duty = UINT16_MAX; duty > 0; duty = duty - INCREMENT) {
		pwm_config.Pulse = duty;

		HAL_TIM_PWM_ConfigChannel(user_config->error_timer, &pwm_config, user_config->error_channel);
		HAL_TIM_PWM_ConfigChannel(user_config->indicator_timer, &pwm_config, user_config->indicator_channel);
		HAL_TIM_PWM_ConfigChannel(user_config->heartbeat_timer, &pwm_config, user_config->heartbeat_channel);

		HAL_Delay(STEP_PERIOD_MS);
	}

	pwm_config.Pulse = 0;
	HAL_TIM_PWM_ConfigChannel(user_config->error_timer, &pwm_config, user_config->error_channel);
	HAL_TIM_PWM_ConfigChannel(user_config->indicator_timer, &pwm_config, user_config->indicator_channel);
	HAL_TIM_PWM_ConfigChannel(user_config->heartbeat_timer, &pwm_config, user_config->heartbeat_channel);

	return HAL_OK;
}

void led_operate(struct LEDMoBoConfig* user_config) {
	const uint32_t CURRENT_TICK = HAL_GetTick();
	static uint32_t heartbeart_change_tick = 0;
	static bool heartbeat_direction = true;
	static int_fast32_t heartbeat_duty = LED_HEARTBEAT_MINIMUM_DUTY;

	TIM_OC_InitTypeDef pwm_config = {0};
	pwm_config.Pulse = 0;
	pwm_config.OCMode = TIM_OCMODE_PWM1;
	pwm_config.OCPolarity = TIM_OCPOLARITY_HIGH;
	pwm_config.OCFastMode = TIM_OCFAST_DISABLE;

	for (uint_fast8_t led = 0; led < LED_COUNT; led++) {
		pwm_config.Pulse = 0;

		if (led_pulses[led].number_remaining > 0) {
			if (CURRENT_TICK < led_pulses[led].next_change_tick) continue;
			led_pulses[led].current_flash_state = !led_pulses[led].current_flash_state;

			if (led_pulses[led].current_flash_state) pwm_config.Pulse = UINT16_MAX;
			else {
				pwm_config.Pulse = 0;
				led_pulses[led].number_remaining--;
			}

			led_pulses[led].next_change_tick = CURRENT_TICK + led_pulses[led].pulse_half_period_ms;

			switch (led) {
			case LED_ERROR:
				HAL_TIM_PWM_ConfigChannel(user_config->error_timer, &pwm_config, user_config->error_channel);
				break;
			case LED_HEARTBEAT:
				HAL_TIM_PWM_ConfigChannel(user_config->heartbeat_timer, &pwm_config, user_config->heartbeat_channel);
				break;
			case LED_INDICATOR:
				HAL_TIM_PWM_ConfigChannel(user_config->indicator_timer, &pwm_config, user_config->indicator_channel);
				break;
			}

		}
		else if (led == LED_HEARTBEAT) {
			if (CURRENT_TICK < heartbeart_change_tick) continue;
			heartbeart_change_tick = CURRENT_TICK + LED_HEARTBEAT_PERIOD_MS;

			if (heartbeat_direction == true) {
				heartbeat_duty = heartbeat_duty + LED_HEARTBEAT_INCREMENT;
				if (heartbeat_duty >= LED_HEARTBEAT_MAXIMUM_DUTY) {
					heartbeat_duty = LED_HEARTBEAT_MAXIMUM_DUTY;
					heartbeat_direction = false;
				}
			}
			else {
				heartbeat_duty = heartbeat_duty - LED_HEARTBEAT_INCREMENT;
				if (heartbeat_duty <= LED_HEARTBEAT_MINIMUM_DUTY) {
					heartbeat_duty = LED_HEARTBEAT_MINIMUM_DUTY;
					heartbeat_direction = true;
				}
			}
			pwm_config.Pulse = heartbeat_duty;

			HAL_TIM_PWM_ConfigChannel(user_config->heartbeat_timer, &pwm_config, user_config->heartbeat_channel);
		}
	}

	if (user_config->indicate_error) {
		pwm_config.Pulse = LED_ERROR_ACTIVE_DUTY;
		HAL_TIM_PWM_ConfigChannel(user_config->error_timer, &pwm_config, user_config->error_channel);
	}
}

void led_blink(enum LEDChannel channel, uint_fast8_t count, uint_fast16_t pulse_duration_ms) {
	if (channel >= LED_COUNT) return;

	led_pulses[channel].current_flash_state = false; // Enter loop "off"
	led_pulses[channel].number_remaining = count;

	uint32_t half_period = pulse_duration_ms / 2;
	led_pulses[channel].pulse_half_period_ms = half_period;
	led_pulses[channel].next_change_tick = HAL_GetTick() + half_period;
}
