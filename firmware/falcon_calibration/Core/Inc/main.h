/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void USB_CDC_RxHandler(uint8_t* buffer, uint32_t length);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RADIO_INT_CE_Pin GPIO_PIN_13
#define RADIO_INT_CE_GPIO_Port GPIOC
#define RADIO_INT_IRQ_Pin GPIO_PIN_14
#define RADIO_INT_IRQ_GPIO_Port GPIOC
#define RADIO_EXT_IRQ_Pin GPIO_PIN_15
#define RADIO_EXT_IRQ_GPIO_Port GPIOC
#define PWM_IN_Pin GPIO_PIN_0
#define PWM_IN_GPIO_Port GPIOA
#define RADIO_EXT_CE_Pin GPIO_PIN_1
#define RADIO_EXT_CE_GPIO_Port GPIOA
#define GPS_RX_Pin GPIO_PIN_2
#define GPS_RX_GPIO_Port GPIOA
#define GPS_TX_Pin GPIO_PIN_3
#define GPS_TX_GPIO_Port GPIOA
#define RADIO_EXT_CS_Pin GPIO_PIN_4
#define RADIO_EXT_CS_GPIO_Port GPIOA
#define RADIO_SCK_Pin GPIO_PIN_5
#define RADIO_SCK_GPIO_Port GPIOA
#define RADIO_MISO_Pin GPIO_PIN_6
#define RADIO_MISO_GPIO_Port GPIOA
#define RADIO_MOSI_Pin GPIO_PIN_7
#define RADIO_MOSI_GPIO_Port GPIOA
#define LED_HEARTBEAT_Pin GPIO_PIN_0
#define LED_HEARTBEAT_GPIO_Port GPIOB
#define RADIO_INT_CS_Pin GPIO_PIN_1
#define RADIO_INT_CS_GPIO_Port GPIOB
#define ENC_5_Pin GPIO_PIN_2
#define ENC_5_GPIO_Port GPIOB
#define ENC_4_Pin GPIO_PIN_10
#define ENC_4_GPIO_Port GPIOB
#define ENC_3_Pin GPIO_PIN_12
#define ENC_3_GPIO_Port GPIOB
#define ENC_2_Pin GPIO_PIN_13
#define ENC_2_GPIO_Port GPIOB
#define ENC_1_Pin GPIO_PIN_8
#define ENC_1_GPIO_Port GPIOA
#define DEBUG_TX_Pin GPIO_PIN_9
#define DEBUG_TX_GPIO_Port GPIOA
#define DEBUG_RX_Pin GPIO_PIN_10
#define DEBUG_RX_GPIO_Port GPIOA
#define IMU_CS_Pin GPIO_PIN_15
#define IMU_CS_GPIO_Port GPIOA
#define IMU_SCK_Pin GPIO_PIN_3
#define IMU_SCK_GPIO_Port GPIOB
#define IMU_MISO_Pin GPIO_PIN_4
#define IMU_MISO_GPIO_Port GPIOB
#define IMU_MOSI_Pin GPIO_PIN_5
#define IMU_MOSI_GPIO_Port GPIOB
#define LED_ERROR_Pin GPIO_PIN_6
#define LED_ERROR_GPIO_Port GPIOB
#define LED_INDICATOR_Pin GPIO_PIN_7
#define LED_INDICATOR_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
