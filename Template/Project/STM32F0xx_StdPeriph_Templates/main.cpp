#include "main.h"


RCC_ClocksTypeDef RCC_Clocks;


int main(void)
{
  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);

      
  /* Add your application code here
     */
	
	GPIO_InitTypeDef Init;
	Init.GPIO_Mode = GPIO_Mode_OUT;
	Init.GPIO_OType = GPIO_OType_PP;
	Init.GPIO_Pin = GPIO_Pin_9;
	Init.GPIO_PuPd = GPIO_PuPd_UP;
	Init.GPIO_Speed = GPIO_Speed_50MHz;
  
	
	/* Infinite loop */
  while (1)
  {
		
  }
}
