#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "main.h"
#include "stm32f1xx_hal.h"
#include "adc.h"

extern ADC_HandleTypeDef hadc1;

#define ADC_NUM_SAMPLES  15

uint16_t adc_data[ADC_NUM_CHANNELS];
volatile uint32_t adc_data_avg[ADC_NUM_CHANNELS];
volatile uint32_t distance;

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	static uint32_t adc_data_sum[ADC_NUM_CHANNELS] = { 0 };
	static uint32_t num_conv = 0;
	uint32_t n;

	if(hadc == &hadc1)
	{
		for(n = 0 ; n < ADC_NUM_CHANNELS ; n++)
			adc_data_sum[n] += adc_data[n];

		if(num_conv++ > ADC_NUM_SAMPLES)
		{
			num_conv = 0;

			for(n = 0 ; n < ADC_NUM_CHANNELS ; n++)
			{
				adc_data_avg[n] = adc_data_sum[n] / ADC_NUM_SAMPLES;
				//distance = 27.728 * pow(map(adc_data_avg[0], 0, 4095, 0, 5000)/1000.0, -1.2045);
				adc_data_sum[n] = 0;
			}
		}
	}
}

//uint32_t test_bateria()
//{
 //divisor de tens�o (8,4 x 11000)/50000 = 1,848 v equivale a 2216,45 ad
 //Carga m�xima = 8,4 v -> tens�o no micro = 1,848 v = 2296,76 adc
 //Carga minima = 7v -> tens�o no micro = 1,54 v   = 1911,466666 adc
 // adc_data_avg[4] x 3,3 v/ 4096

  //uint32_t bateria = ((adc_data_avg[4] * 3.3)/4096);
  //return bateria;
//}


void adc_init(void)
{
	uint32_t n;

	for(n = 0 ; n < ADC_NUM_CHANNELS ; n++)
		adc_data_avg[n] = 0;

	HAL_ADC_Start_DMA(&hadc1,(uint32_t *)&adc_data[0],ADC_NUM_CHANNELS);
}


void adc_loop(void) {

	/* adc_data_avg[0]-sensor_sharp_direita - 400 longe
	 * adc_data_avg[1]-sensor_sharp_esq - 400 longe - 2000 satura -
	 * adc_data_avg[2]-sensor_direita - branco (236) - preto (4428)
	 * adc_data_avg[3]-senosr_esq - branco (117) - preto (3200)
	 * adc_data_avg[4] - CARGA BATERIA
	 * adc_data_avg[5] - SENSOR CENTRAL - 1800 muito perto satura, 600 quando estiver longe
     */
//	uint8_t obstaculo = HAL_GPIO_ReadPin(SENSOR_OBST_GPIO_Port, SENSOR_OBST_Pin);

	/*if ((adc_data_avg[3] < 800) && (adc_data_avg[2] < 800))// detectar 3 sensores da frente
	{
	    HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_RESET);//MOTOR ESQUERDA
	    HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_RESET);

	    motor_frente(14000, 14000);

	}
	if((adc_data_avg[5] >= 2100) && (adc_data_avg[3] < 800) && (adc_data_avg[2] < 800))
	{
	motor_frente(19000, 19000);
    HAL_Delay(300);
	}

	// if ((adc_data_avg[0] > 3000) && (adc_data_avg[1] > 3000) && (adc_data_avg[2] < 1300) && (adc_data_avg[3] < 700))// detectar 3 sensores da frente
	//{
//	    motor_frente(10000, 10000);
	//    HAL_Delay(300);
	//}

	// if ((adc_data_avg[0] > 3000) && (adc_data_avg[1] < 3000) && (adc_data_avg[2] < 1300) && (adc_data_avg[3] < 700))// detectar 3 sensores da frente
		//{

		  //  motor_frente(10000, 8000);
		    //HAL_Delay(100);
		//}
//	 if ((adc_data_avg[0] < 3000) && (adc_data_avg[1] > 3000) && (adc_data_avg[2] < 1300) && (adc_data_avg[3] < 700))// detectar 3 sensores da frente
	//	{

		//    motor_frente(8000, 10000);
		  //  HAL_Delay(100);

		//}
	else if ((adc_data_avg[3] >= 800) && (adc_data_avg[2] >= 800))//sensor esquerda
	{
	   HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_SET);
	   HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_SET);

	   motor_tras(15000, 15000);//4200, 9500
	   HAL_Delay(300);
	   motor_frente(8000, 13000);
	   HAL_Delay(400);

	}

	else if ((adc_data_avg[3] >= 800)&&(adc_data_avg[2] < 800)) //sensor esquerda
	{

		HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_SET);//MOTOR ESQUERDA
	    HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_RESET);
		motor_tras(15000, 15000);//4200, 9500
		HAL_Delay(300);
		motor_frente(13000, 8000);//4200, 9500
		HAL_Delay(400);


	}
	else if ((adc_data_avg[2] >= 800)&&(adc_data_avg[3] < 800)) //sensor direita
	{

		HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_RESET);//MOTOR ESQUERDA
	    HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_SET);
		motor_tras(15000, 15000);//4200, 9500
		HAL_Delay(300);
		motor_frente(8000, 13000);//4200, 9500
		HAL_Delay(400);

	}*/
}







