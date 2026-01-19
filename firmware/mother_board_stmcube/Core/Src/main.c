/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "can.h"
#include "fmpi2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include <stdint.h>
#include "usbd_cdc_if.h"
#include "string.h"
#include "led_operation.h"
#include "can_wrapper.h"
#include "wing_modules.h"
#include "ina219.h"
#include "atmosphere.h"
#include "position.h"
#include "../../stm32_hal_nrf24_library/NRF24.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
TIM_HandleTypeDef* led_err_clk = &htim4;
uint32_t led_err_chn = TIM_CHANNEL_1;
TIM_HandleTypeDef* led_ind_clk = &htim4;
uint32_t led_ind_chn = TIM_CHANNEL_2;
TIM_HandleTypeDef* led_heartbeat_clk = &htim3;
uint32_t led_heartbeat_chn = TIM_CHANNEL_3;

const uint32_t I2C_TIMEOUT = 10; // Timeout to be used for I2C interactions

struct NRFConfig radio_ext = {
	.hspiX = &hspi1,
	.spi_w_timeout = 1000,
	.spi_r_timeout = 1000,
	.spi_rw_timeout = 1000,

	.csn_gpio_port = RADIO_EXT_CS_GPIO_Port,
	.csn_gpio_pin = RADIO_EXT_CS_Pin,

	.ce_gpio_port = RADIO_EXT_CE_GPIO_Port,
	.ce_gpio_pin = RADIO_EXT_CE_Pin,
};

struct NRFConfig radio_int = {
	.hspiX = &hspi1,
	.spi_w_timeout = 1000,
	.spi_r_timeout = 1000,
	.spi_rw_timeout = 1000,

	.csn_gpio_port = RADIO_INT_CS_GPIO_Port,
	.csn_gpio_pin = RADIO_INT_CS_Pin,

	.ce_gpio_port = RADIO_INT_CE_GPIO_Port,
	.ce_gpio_pin = RADIO_INT_CE_Pin,
};

#define PLD_S 4

uint8_t rx_addr[5] = {'1', 'N', 'o', 'd', 'e'};
uint8_t tx_addr[5] = {'2', 'N', 'o', 'd', 'e'};

uint8_t radio_tx_buffer[PLD_S];
uint8_t radio_rx_buffer[PLD_S];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_DEVICE_Init();
  MX_CAN1_Init();
  MX_FMPI2C1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_SPI1_Init();
  MX_SPI3_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM2_Init();
  MX_TIM1_Init();
  MX_TIM8_Init();
  /* USER CODE BEGIN 2 */

	struct LEDMoBoConfig led_config = {
		  .error_timer = led_err_clk,
		  .error_channel = led_err_chn,
		  .indicator_timer = led_ind_clk,
		  .indicator_channel = led_ind_chn,
		  .heartbeat_timer = led_heartbeat_clk,
		  .heartbeat_channel = led_heartbeat_chn,
	};
	led_setup(&led_config);

//	wing_setup(&hcan1);
	uint8_t usb_tx_buffer[500] = {0};
	uint16_t usb_tx_length = 0;

	HAL_StatusTypeDef ret = ina219_setup(&hfmpi2c1, I2C_TIMEOUT);
	printf("INA219 setup result: %d\n\r", ret);

	ret = atmo_setup(&hfmpi2c1, I2C_TIMEOUT);

	ret = pos_setup(&hfmpi2c1, I2C_TIMEOUT);

	/*
	 * SPI Frequency           = 10 Mhz
Channel                 = 76 (~ 2476 MHz)
Model                   = nRF24L01+
RF Data Rate            = 1 MBPS
RF Power Amplifier      = PA_LOW
RF Low Noise Amplifier  = Enabled
CRC Length              = 16 bits
Address Length          = 5 bytes
Static Payload Length   = 4 bytes
Auto Retry Delay        = 1500 microseconds
Auto Retry Attempts     = 15 maximum
Packets lost on
    current channel     = 0
Retry attempts made for
    last transmission   = 15
Multicast               = Disabled
Custom ACK Payload      = Disabled
Dynamic Payloads        = Disabled
Auto Acknowledgment     = Enabled
Primary Mode            = TX
TX address              = 0x65646f4e31
pipe 0 ( open ) bound   = 0x65646f4e31
pipe 1 ( open ) bound   = 0x65646f4e32
	 */

	struct NRFConfig* target_radio = &radio_int;
	  csn_high(*target_radio);
	  ce_high(*target_radio);

	  HAL_Delay(5);

	  ce_low(*target_radio);

	  nrf24_init(*target_radio, &htim8);

	  nrf24_auto_ack_all(*target_radio, auto_ack);
	  nrf24_en_ack_pld(*target_radio, disable);
	  nrf24_dpl(*target_radio, disable);

	  nrf24_set_crc(*target_radio, en_crc, _2byte);

	  nrf24_tx_pwr(*target_radio, n6dbm);
	  nrf24_data_rate(*target_radio, _1mbps);
	  nrf24_set_channel(*target_radio, 76);
	  nrf24_set_addr_width(*target_radio, 5);

	  nrf24_set_rx_dpl(*target_radio, 0, disable);
	  nrf24_set_rx_dpl(*target_radio, 1, disable);
	  nrf24_set_rx_dpl(*target_radio, 2, disable);
	  nrf24_set_rx_dpl(*target_radio, 3, disable);
	  nrf24_set_rx_dpl(*target_radio, 4, disable);
	  nrf24_set_rx_dpl(*target_radio, 5, disable);

	  nrf24_pipe_pld_size(*target_radio, 0, PLD_S);
	  nrf24_pipe_pld_size(*target_radio, 1, PLD_S);

	  nrf24_auto_retr_delay(*target_radio, 4); // Delay is in increments of 250us
	  nrf24_auto_retr_limit(*target_radio, 15);

	  nrf24_open_tx_pipe(*target_radio, tx_addr);
	  nrf24_open_rx_pipe(*target_radio, 1, rx_addr);

	  nrf24_listen(*target_radio);


	  target_radio = &radio_ext;
	  	  csn_high(*target_radio);
	  	  ce_high(*target_radio);

	  	  HAL_Delay(5);

	  	  ce_low(*target_radio);

	  	  nrf24_init(*target_radio, &htim8);

	  	  nrf24_auto_ack_all(*target_radio, auto_ack);
	  	  nrf24_en_ack_pld(*target_radio, disable);
	  	  nrf24_dpl(*target_radio, disable);

	  	  nrf24_set_crc(*target_radio, en_crc, _2byte);

	  	  nrf24_tx_pwr(*target_radio, n6dbm);
	  	  nrf24_data_rate(*target_radio, _1mbps);
	  	  nrf24_set_channel(*target_radio, 76);
	  	  nrf24_set_addr_width(*target_radio, 5);

	  	  nrf24_set_rx_dpl(*target_radio, 0, disable);
	  	  nrf24_set_rx_dpl(*target_radio, 1, disable);
	  	  nrf24_set_rx_dpl(*target_radio, 2, disable);
	  	  nrf24_set_rx_dpl(*target_radio, 3, disable);
	  	  nrf24_set_rx_dpl(*target_radio, 4, disable);
	  	  nrf24_set_rx_dpl(*target_radio, 5, disable);

	  	  nrf24_pipe_pld_size(*target_radio, 0, PLD_S);
	  	  nrf24_pipe_pld_size(*target_radio, 1, PLD_S);

	  	  nrf24_auto_retr_delay(*target_radio, 4); // Delay is in increments of 250us
	  	  nrf24_auto_retr_limit(*target_radio, 15);

	  	  nrf24_open_tx_pipe(*target_radio, rx_addr);
	  	  nrf24_open_rx_pipe(*target_radio, 0, rx_addr);

	  	  nrf24_stop_listen(*target_radio);
	  	  ce_high(radio_ext);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//	  if (HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_LOAD_FIFO) > 0) {
//		  CAN_RxHeaderTypeDef rx_header = {0};
//		  uint8_t rx_buffer[8] = {0};
//		  HAL_CAN_GetRxMessage(&hcan1, CAN_LOAD_FIFO, &rx_header, rx_buffer);
//
//		  wing_record_strain(rx_header.StdId, rx_buffer);
//	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

		uint32_t data_sent = HAL_GetTick();
		nrf24_transmit(radio_ext, &data_sent, 4);

		if(nrf24_data_available(radio_int)){
			uint32_t data_received = 0;
			nrf24_receive(radio_int, radio_rx_buffer, sizeof(radio_rx_buffer));
			data_received = nrf24_uint8_t_to_type(radio_rx_buffer, sizeof(data_received));
			printf("TX: %ld\tRX: %ld\r\n", data_sent, data_received);
		}
		else printf("TX: %ld\r\n", data_sent);

      led_operate(&led_config);

//		uint32_t current_tick = HAL_GetTick();
//		const uint32_t PRINT_PERIOD_TICKS = 1000;
//		static uint32_t next_print_tick = 0;
//		if (current_tick > next_print_tick) {
//			usb_tx_length = print_nice_loading(usb_tx_buffer, sizeof(usb_tx_buffer));
//			next_print_tick = current_tick + PRINT_PERIOD_TICKS;
//		}
//		else buffer[0] = '\0'; // Nullify the buffer

//      if (usb_tx_length > 0) {
//    	  CDC_Transmit_FS(usb_tx_buffer, usb_tx_length);
//    	  wing_setup(&hcan1);
//      }
      HAL_Delay(1000);

//      struct AtmoConditions atmo_cond = {0};
//      ret = atmo_conditions_update(&atmo_cond);
//      printf("[%d]T: %.2f\tH: %.2f\tP: %.2f\tCO2: %d\n\r", ret, atmo_cond.temperature_c,
//    		  atmo_cond.humidity_rel, atmo_cond.static_pressure_pa, atmo_cond.co2_ppm);
//      struct OrientationState orientation = {0};
//      ret = pos_orientation_update(&orientation);
//      printf("[%d]R: %6.1f\tP: %6.1f\tY: %6.1f\n\r", ret, orientation.roll_deg, orientation.pitch_deg, orientation.yaw_deg);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 10;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
