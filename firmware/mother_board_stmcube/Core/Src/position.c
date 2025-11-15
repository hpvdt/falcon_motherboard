/*
 * position.c
 *
 *  Created on: Nov 13, 2025
 *      Author: savo
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "position.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_fmpi2c.h"

#include "bosch_wrappers.h"
#include "../../BNO055_SensorAPI/bno055.h"

static int8_t bosch_write_i2c_by_addr(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t len);
static int8_t bosch_read_i2c_by_addr(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t len);
static void bosch_delay_ms(u32 ms) {
	HAL_Delay(ms);
}


static uint32_t bno055_i2c_timeout_ms = 100;
static const uint8_t BNO055_ADDRESS = 0x28;
static struct bno055_t bno055 = {
		.dev_addr = BNO055_ADDRESS,
		.chip_id = BNO055_CHIP_ID_REG,
		.delay_msec = bosch_delay_ms,
		.bus_read = bosch_read_i2c_by_addr,
		.bus_write = bosch_write_i2c_by_addr,
};
static FMPI2C_HandleTypeDef* bno055_bus;

static int8_t bosch_write_i2c_by_addr(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t len) {
	return HAL_FMPI2C_Mem_Write(bno055_bus, dev_addr << 1, reg_addr, 1, reg_data, len, bno055_i2c_timeout_ms);
}

static int8_t bosch_read_i2c_by_addr(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t len) {
	return HAL_FMPI2C_Mem_Read(bno055_bus, dev_addr << 1, reg_addr, 1, reg_data, len, bno055_i2c_timeout_ms);
}
HAL_StatusTypeDef pos_setup(FMPI2C_HandleTypeDef* bus, uint32_t i2c_timeout_ms) {
	bool any_error = false;

	bno055_bus = bus;
	bno055_i2c_timeout_ms = i2c_timeout_ms;

	BNO055_RETURN_FUNCTION_TYPE bno_ret;
	bno_ret = bno055_init(&bno055);
	printf("BNO055 setup result: %d\n\r", bno_ret);
	if (bno_ret == BNO055_SUCCESS) {
		bno_ret = bno055_set_power_mode(BNO055_POWER_MODE_NORMAL);
		printf("BNO055 power mode result: %d\n\r", bno_ret);
		if (bno_ret != BNO055_SUCCESS) any_error = true;
		bno_ret = bno055_set_operation_mode(BNO055_OPERATION_MODE_NDOF);
		printf("BNO055 operation mode result: %d\n\r", bno_ret);
		if (bno_ret != BNO055_SUCCESS) any_error = true;
	}
	else any_error = true;

	if (any_error) return HAL_ERROR;
	return HAL_OK;
}

HAL_StatusTypeDef pos_orientation_update(struct OrientationState* target) {
	bool any_error = false;

	BNO055_RETURN_FUNCTION_TYPE bno_ret;

    struct bno055_euler_double_t euler_hrp;
    bno_ret = bno055_convert_double_euler_hpr_deg(&euler_hrp);
    if (bno_ret != BNO055_SUCCESS) any_error = true;


    // Fusion
    if (bno_ret == BNO055_SUCCESS) {
    	target->pitch_deg = euler_hrp.p;
    	target->roll_deg = euler_hrp.r;
    	target->yaw_deg = euler_hrp.h;
    }

    if (any_error) return HAL_ERROR;
    return HAL_OK;
}
