#include "stm32f0xx_it.h"
#include "main.h"


RCC_ClocksTypeDef RCC_Clocks;

static inline uint32_t USART1Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);
	
	
	GPIO_InitTypeDef GPIOInit;
	GPIOInit.GPIO_Mode = GPIO_Mode_AF;
	GPIOInit.GPIO_OType = GPIO_OType_PP;
	GPIOInit.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIOInit.GPIO_PuPd = GPIO_PuPd_UP;
	GPIOInit.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIOInit);
	
	USART_InitTypeDef USARTInit;
	USARTInit.USART_BaudRate = 115200;
	USARTInit.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USARTInit.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USARTInit.USART_Parity = USART_Parity_No;
	USARTInit.USART_StopBits = USART_StopBits_1;
	USARTInit.USART_WordLength = USART_WordLength_8b;
	
	USART_Init(USART1,&USARTInit);
	
	USART_Cmd(USART1, ENABLE);
	return 0;//left for auto-baudrate detection
}
static inline void USART1waitForTransmit()
{
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}
static inline void USART1sendByte(uint8_t x)
{
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	USART_SendData(USART1, x);
}
static inline void USART1newline()
{
	USART1sendByte((uint8_t) '\r');
	USART1sendByte((uint8_t) '\n');
}
static inline void USART1print(char * out)
{
	while (*out)
	{
		USART1sendByte(*out);
		out++;
	}
}
static inline void USART1print(uint8_t data)
{
	USART1sendByte(data);
}
static inline void put(uint8_t data)
{
	USART1sendByte(data);
}
static inline void print(int16_t data)
{
	char buffer[100];
	sprintf(buffer,"%d",data);
	USART1print(buffer);
}
static inline void USART1println(int16_t data)
{
	char buffer[100];
	sprintf(buffer,"%d",data);
	USART1print(buffer);
	USART1newline();
}
static inline void USART1print(int32_t data)
{
	char buffer[100];
	sprintf(buffer,"%d",data);
	USART1print(buffer);
}
static inline void USART1println(int32_t data)
{
	char buffer[100];
	sprintf(buffer,"%d",data);
	USART1print(buffer);
	USART1newline();
}
static inline void USART1println(uint8_t data)
{
	USART1sendByte(data);
	USART1newline();
}
static inline void USART1println(char * out)
{
	USART1print(out);
	USART1newline();
}
static inline bool USART1available()
{
	return (USART_GetFlagStatus(USART1, USART_FLAG_RXNE));
}
static inline uint8_t USART1read()
{
	while( USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	return USART_ReceiveData(USART1);
}




/*void SysTick_Handler(void)
{
	USART1print("yo!");
}*/
volatile int x = 0;
volatile uint8_t flag = true;
void SysTick_Handler(void)
{
	if( flag )
	{
		flag = 0;
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
		
		USART1print(x);
		USART1print(" ");  
		USART1println(ADC_GetConversionValue(ADC1));
		flag = 1;
	}
}

GPIO_InitTypeDef GPIO_InitStructure;
int main(void)
{
  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  	
  /* Add your application code here
     */
	//SysTick_Type SysTick1;
	//SysTick1.LOAD = 48000;
	//SysTick1.VAL = 0;

	USART1Init();
	USART1print((int32_t)RCC_Clocks.HCLK_Frequency);
	
	SysTick_Config(RCC_Clocks.HCLK_Frequency/10000);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
  /* Configure ADC Channel11 as analog input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  /* ADCs DeInit */  
  ADC_DeInit(ADC1);
  
  /* Initialize ADC structure */
  
	
	

	

	
	
	
	
	ADC_InitTypeDef ADCInit;
	ADC_StructInit(&ADCInit);
	
	ADCInit.ADC_ContinuousConvMode = ENABLE;
	ADCInit.ADC_DataAlign = ADC_DataAlign_Right;
	ADCInit.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;
	ADCInit.ADC_Resolution = ADC_Resolution_12b;
	ADCInit.ADC_ScanDirection = ADC_ScanDirection_Upward;
	ADC_Init(ADC1, &ADCInit);
		
  /* Convert the ADC1 Channel 11 with 239.5 Cycles as sampling time */ 
  ADC_ChannelConfig(ADC1, ADC_Channel_10 , ADC_SampleTime_1_5Cycles);   

  /* ADC Calibration */
  ADC_GetCalibrationFactor(ADC1);
  
  /* Enable ADCperipheral[PerIdx] */
  ADC_Cmd(ADC1, ENABLE);     
  
  /* Wait the ADCEN falg */
  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN)); 
  
  /* ADC1 regular Software Start Conv */ 
  ADC_StartOfConversion(ADC1);


	USART1println("INIT!");
	
	/* Infinite loop */
	
  while (1)
  {
		++x;
		//while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    
    //USART1println(ADC_GetConversionValue(ADC1));
  }
}
