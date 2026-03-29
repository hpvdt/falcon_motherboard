/*
 * radio.h
 *
 *  Created on: Jan 22, 2026
 *      Author: savo
 */

#ifndef INC_RADIO_H_
#define INC_RADIO_H_

#include "stm32f4xx_hal.h"
#include "falcon_data.h"

HAL_StatusTypeDef radio_setup();

HAL_StatusTypeDef radio_receive_internal(struct CompleteAircraftData* target);

HAL_StatusTypeDef radio_broadcast_data(struct CompleteAircraftData* target);

#endif /* INC_RADIO_H_ */
