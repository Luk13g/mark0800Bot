#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "stm32f1xx_hal.h"
#include "motor3.h"
#include "adc.h"

volatile uint8_t ligar;
//volatile extern uint32_t adc_data_avg[ADC_NUM_CHANNELS];
//float bateria;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if( GPIO_Pin == GPIO_PIN_7 )
	{
		ligar = 1;
	}
}

/*float test_bateria()
{
 //divisor de tens�o (8,4 x 11000)/50000 = 1,848 v equivale a 2216,45 ad
 //Carga m�xima = 8,4 v -> tens�o no micro = 1,848 v = 2296,76 adc
 //Carga minima = 7v -> tens�o no micro = 1,54 v   = 1911,466666 adc
 // adc_data_avg[4] x 3,3 v/ 4096

  bateria = ((adc_data_avg[4] * 3.3)/4096);
  return bateria;
}*/

void test_init()
{
	adc_init();
	motor3_init();
}

void test_loop ()
{
	if(ligar)
	{
		HAL_Delay(2000);
		while(1)
		{
			setSpeeds(8279, 7999);
		}
		}
}
