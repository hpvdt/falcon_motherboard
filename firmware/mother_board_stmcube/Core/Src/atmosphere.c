/*
 * atmosphere.c
 *
 *  Created on: Nov 12, 2025
 *      Author: savo
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "atmosphere.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_fmpi2c.h"
#include "scd4x.h"

#include "bosch_wrappers.h"
#include "../../BME280_SensorAPI/bme280.h"
#include "../../BME280_SensorAPI/bme280_defs.h"
#include "../../BMP3_SensorAPI/bmp3.h"
#include "../../BMP3_SensorAPI/bmp3_defs.h"

static const uint8_t BME280_ADDRESS = 0x77;
static struct BoschI2C bme280_interface;
static struct bme280_dev dev_bme280 = {
		.chip_id = BME280_CHIP_ID,
		.intf = BME280_I2C_INTF,
		.intf_ptr = &bme280_interface,
		.read = bosch_read_i2c_by_intf,
		.write = bosch_write_i2c_by_intf,
		.delay_us = bosch_delay_us,
};

static const uint8_t BMP390_ADDRESS = 0x76;
static struct BoschI2C bmp390_interface;
static struct bmp3_dev dev_bmp390 = {
		.chip_id = BMP390_CHIP_ID,
		.intf = BMP3_I2C_INTF,
		.intf_ptr = &bmp390_interface,
		.read = bosch_read_i2c_by_intf,
		.write = bosch_write_i2c_by_intf,
		.delay_us = bosch_delay_us,
};

HAL_StatusTypeDef atmo_setup(FMPI2C_HandleTypeDef* bus, uint32_t i2c_timeout_ms) {
	bool any_error = false;

	HAL_StatusTypeDef ret_scd4x = scd4x_setup(bus, i2c_timeout_ms);
	printf("SCD41 setup result: %d\n\r", ret_scd4x);
	if (ret_scd4x != HAL_OK) any_error = true;

	bosch_adjust_i2c_timeout(i2c_timeout_ms);
	bme280_interface.i2c_handle = bus;
	bme280_interface.address = BME280_ADDRESS;

	uint8_t bme_init = bme280_init(&dev_bme280);
	printf("BME280 setup result: %d\n\r", bme_init);
	if (bme_init < 0) any_error = true;
	else {
		// Only bother with setup if chip initialized ok
		struct bme280_settings settings = {
				.osr_p = BME280_OVERSAMPLING_4X,
				.osr_t = BME280_OVERSAMPLING_4X,
				.osr_h = BME280_OVERSAMPLING_4X,
				.filter = BME280_FILTER_COEFF_4,
				.standby_time= BME280_STANDBY_TIME_0_5_MS,
		};
		bme_init = bme280_set_sensor_settings(BME280_SEL_ALL_SETTINGS, &settings, &dev_bme280);
		printf("BME280 settings result: %d\n\r", bme_init);
		if (bme_init < 0) any_error = true;
		bme_init = bme280_set_sensor_mode(BME280_POWERMODE_NORMAL, &dev_bme280);
		printf("BME280 mode result: %d\n\r", bme_init);
		if (bme_init < 0) any_error = true;
	}

	bmp390_interface.i2c_handle = bus;
	bmp390_interface.address = BMP390_ADDRESS;
	uint8_t bmp_init = bmp3_init(&dev_bmp390);
	printf("BMP390 setup result: %d\n\r", bmp_init);
	if (bmp_init < 0) any_error = true;
	else {
		struct bmp3_settings settings = {
				.op_mode = BMP3_MODE_NORMAL,
				.press_en = BMP3_ENABLE,
				.temp_en = BMP3_ENABLE,
				.odr_filter.press_os = BMP3_OVERSAMPLING_4X,
				.odr_filter.temp_os = BMP3_OVERSAMPLING_4X,
				.odr_filter.iir_filter = BMP3_IIR_FILTER_COEFF_7,
				.odr_filter.odr = BMP3_ODR_50_HZ,
				.int_settings.output_mode = BMP3_INT_PIN_OPEN_DRAIN,
				.adv_settings.i2c_wdt_en = BMP3_DISABLE,
		};
		bmp_init = bmp3_set_sensor_settings(BMP3_SEL_ALL, &settings, &dev_bmp390);
		printf("BMP390 settings result: %d\n\r", bmp_init);
		if (bmp_init < 0) any_error = true;
		bmp3_set_op_mode(&settings, &dev_bmp390);
		printf("BMP390 mode result: %d\n\r", bmp_init);
		if (bmp_init < 0) any_error = true;
	}


	if (any_error) return HAL_ERROR;
	return HAL_OK;
}

HAL_StatusTypeDef atmo_conditions_update(struct AtmoConditions* target) {
	bool any_error = false; // Used to keep track if any interface has errors

	HAL_StatusTypeDef ret_scd4x = HAL_OK;
	ret_scd4x = scd4x_read_co2(&target->co2_ppm);
	if (ret_scd4x != HAL_OK) {
		printf("CO2 update issue: %d\n\r", ret_scd4x);
		any_error = true;
	}

    struct bme280_data bme_data;
    int8_t ret_bme280 = bme280_get_sensor_data(BME280_ALL, &bme_data, &dev_bme280);
    if (ret_bme280 != BME280_OK) {
    	if (ret_bme280 < 0) {
    		printf("BME280 read error: %d", ret_bme280);
    		any_error = true;
			return HAL_ERROR;
    	}
    	else printf("BME280 read warning: %d", ret_bme280);
    }

    struct bmp3_data bmp_data;
    int8_t ret_bmp390 = bmp3_get_sensor_data(BMP3_PRESS_TEMP, &bmp_data, &dev_bmp390);
    if (ret_bmp390 != BMP3_OK) {
    	if (ret_bmp390 < 0) {
    		printf("BMP390 read error: %d", ret_bmp390);
    		any_error = true;
			return HAL_ERROR;
    	}
    	else printf("BMP390 read warning: %d", ret_bmp390);
    }

    // Fusion of different sensor data
    if (ret_bme280 == BME280_OK && ret_bmp390 == BMP3_OK) {
    	float mean;
		target->humidity_rel = bme_data.humidity;

		// Taking basic mean
		mean = (bme_data.pressure + bmp_data.pressure) / 2.0;
		target->static_pressure_pa = mean;
		mean = (bme_data.temperature + bmp_data.temperature) / 2.0;
		target->temperature_c = mean;
    }
    else {
		if (ret_bme280 == BME280_OK) {
			target->humidity_rel = bme_data.humidity;
			target->static_pressure_pa = bme_data.pressure;
			target->temperature_c = bme_data.temperature;
		}
		if (ret_bmp390 == BMP3_OK) {
			target->static_pressure_pa = bmp_data.pressure;
			target->temperature_c = bmp_data.temperature;
		}
    }

    if (any_error) return HAL_ERROR;
    return HAL_OK;
}
