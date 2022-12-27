#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "stm32f1xx_hal.h"
#include "motor3.h"



static bool flipLeft = false;
static bool flipRight = false;

extern TIM_HandleTypeDef htim1;

//MOTOR A E B, DIRE��O 1 E 2

#define HW_MOTA_IN1_PORT   AIN1_1_GPIO_Port
#define HW_MOTA_IN1_PIN    AIN1_1_Pin
#define HW_MOTA_IN2_PORT   AIN2_1_GPIO_Port
#define HW_MOTA_IN2_PIN    AIN2_1_Pin
#define HW_MOTB_IN1_PORT   AIN1_2_GPIO_Port
#define HW_MOTB_IN1_PIN    AIN1_2_Pin
#define HW_MOTB_IN2_PORT   AIN2_2_GPIO_Port
#define HW_MOTB_IN2_PIN    AIN2_2_Pin

//PWM - TIMER E CHANNELS
#define HW_TIMER_PORT      htim1
#define TIM_CHANNEL_A      TIM_CHANNEL_1
#define TIM_CHANNEL_B 	   TIM_CHANNEL_2

// initialize timer1 to generate the proper PWM outputs to the motor drivers
void motor3_init()
{
	HAL_TIM_Base_Start(&HW_TIMER_PORT);
	HAL_TIM_PWM_Start(&HW_TIMER_PORT, TIM_CHANNEL_A);//MOTA_PWM
	HAL_TIM_PWM_Start(&HW_TIMER_PORT, TIM_CHANNEL_B);//MOTB_PWM
	HAL_GPIO_WritePin(HW_MOTA_IN1_PORT, HW_MOTA_IN1_PIN, GPIO_PIN_RESET);//MOTOR ESQUERDA
	HAL_GPIO_WritePin(HW_MOTA_IN2_PORT, HW_MOTA_IN2_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(HW_MOTB_IN1_PORT, HW_MOTB_IN1_PIN, GPIO_PIN_RESET);//MOTOR DIREITA
	HAL_GPIO_WritePin(HW_MOTB_IN2_PORT, HW_MOTB_IN2_PIN,GPIO_PIN_RESET);
	__HAL_TIM_SET_COMPARE(&HW_TIMER_PORT, TIM_CHANNEL_A, 0);//A
	__HAL_TIM_SET_COMPARE(&HW_TIMER_PORT, TIM_CHANNEL_B, 0);//B
}
void motor_parada()
{
	HAL_GPIO_WritePin(HW_MOTA_IN1_PORT, HW_MOTA_IN1_PIN, GPIO_PIN_RESET);//MOTOR ESQUERDA
	HAL_GPIO_WritePin(HW_MOTA_IN2_PORT, HW_MOTA_IN2_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(HW_MOTB_IN1_PORT, HW_MOTB_IN1_PIN, GPIO_PIN_RESET);//MOTOR DIREITA
	HAL_GPIO_WritePin(HW_MOTB_IN2_PORT, HW_MOTB_IN2_PIN,GPIO_PIN_RESET);
	__HAL_TIM_SET_COMPARE(&HW_TIMER_PORT, TIM_CHANNEL_A, 19999);//A
	__HAL_TIM_SET_COMPARE(&HW_TIMER_PORT, TIM_CHANNEL_B, 19999);//B
}

void setSpeeds(int leftSpeed, int rightSpeed)
{
  setLeftSpeed(leftSpeed);
  setRightSpeed(rightSpeed);
}

void setLeftSpeed(int speed)
{
  init(); // initialize if necessary
  bool reverse = 0;

    if (speed < 0)
    {
      speed = -speed; // make speed a positive quantity
      reverse = 1;    // preserve the direction
    }

  if (speed > 19999)  // Max
   speed = 19999;



  if (reverse ^ flipLeft) // flip if speed was negative or flipLeft setting is active, but not both
  {
	   __HAL_TIM_SET_COMPARE(&HW_TIMER_PORT, TIM_CHANNEL_A, speed);
	  HAL_GPIO_WritePin(HW_MOTA_IN1_PORT, HW_MOTA_IN1_PIN, GPIO_PIN_SET);
      HAL_GPIO_WritePin(HW_MOTA_IN2_PORT, HW_MOTA_IN2_PIN, GPIO_PIN_RESET);


  }
  else
  {
	   __HAL_TIM_SET_COMPARE(&HW_TIMER_PORT, TIM_CHANNEL_A, speed);
	  HAL_GPIO_WritePin(HW_MOTA_IN1_PORT, HW_MOTA_IN1_PIN, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(HW_MOTA_IN2_PORT, HW_MOTA_IN2_PIN, GPIO_PIN_SET);
  }


}

// set speed for right motor; speed is a number between -400 and 400
void setRightSpeed(int speed)
{
  init(); // initialize if necessary

  bool reverse = 0;

    if (speed < 0)
    {
      speed = -speed; // make speed a positive quantity
      reverse = 1;    // preserve the direction
    }

 if (speed > 19999)  // Max PWM dutycycle
   speed = 19999;


  if (reverse ^ flipRight) // flip if speed was negative or flipRight setting is active, but not both
  {
      __HAL_TIM_SET_COMPARE(&HW_TIMER_PORT, TIM_CHANNEL_B, speed);
	  HAL_GPIO_WritePin(HW_MOTB_IN1_PORT, HW_MOTB_IN1_PIN, GPIO_PIN_SET);
      HAL_GPIO_WritePin(HW_MOTB_IN1_PORT, HW_MOTB_IN2_PIN, GPIO_PIN_RESET);

	//B
  }
  else
  {
      __HAL_TIM_SET_COMPARE(&HW_TIMER_PORT, TIM_CHANNEL_B, speed);
	  HAL_GPIO_WritePin(HW_MOTB_IN1_PORT, HW_MOTB_IN1_PIN, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(HW_MOTB_IN1_PORT, HW_MOTB_IN2_PIN, GPIO_PIN_SET);
  }


}


