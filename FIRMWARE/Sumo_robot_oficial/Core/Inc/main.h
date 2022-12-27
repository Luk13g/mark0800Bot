/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f1xx_hal.h"

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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SW1_1_Pin GPIO_PIN_14
#define SW1_1_GPIO_Port GPIOC
#define SW1_2_Pin GPIO_PIN_15
#define SW1_2_GPIO_Port GPIOC
#define LED_2_Pin GPIO_PIN_0
#define LED_2_GPIO_Port GPIOA
#define LED_1_Pin GPIO_PIN_1
#define LED_1_GPIO_Port GPIOA
#define CargBat_Pin GPIO_PIN_2
#define CargBat_GPIO_Port GPIOA
#define SLD_Pin GPIO_PIN_3
#define SLD_GPIO_Port GPIOA
#define IR_D_Pin GPIO_PIN_4
#define IR_D_GPIO_Port GPIOA
#define IR_E_Pin GPIO_PIN_5
#define IR_E_GPIO_Port GPIOA
#define SLE_Pin GPIO_PIN_6
#define SLE_GPIO_Port GPIOA
#define SW2_Pin GPIO_PIN_7
#define SW2_GPIO_Port GPIOA
#define SW2_EXTI_IRQn EXTI9_5_IRQn
#define BUZZER_Pin GPIO_PIN_12
#define BUZZER_GPIO_Port GPIOB
#define AIN2_1_Pin GPIO_PIN_14
#define AIN2_1_GPIO_Port GPIOB
#define AIN1_1_Pin GPIO_PIN_15
#define AIN1_1_GPIO_Port GPIOB
#define PWM_1_Pin GPIO_PIN_8
#define PWM_1_GPIO_Port GPIOA
#define PWM_2_Pin GPIO_PIN_9
#define PWM_2_GPIO_Port GPIOA
#define AIN2_2_Pin GPIO_PIN_10
#define AIN2_2_GPIO_Port GPIOA
#define AIN1_2_Pin GPIO_PIN_11
#define AIN1_2_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
