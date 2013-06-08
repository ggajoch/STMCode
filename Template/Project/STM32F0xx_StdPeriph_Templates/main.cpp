#include "main.h"


RCC_ClocksTypeDef RCC_Clocks;

struct MineUSART1
{
	char buffer[33];
	void init()
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
		USARTInit.USART_BaudRate = 9600;
		USARTInit.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USARTInit.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USARTInit.USART_Parity = USART_Parity_No;
		USARTInit.USART_StopBits = USART_StopBits_1;
		USARTInit.USART_WordLength = USART_WordLength_8b;
		
		USART_Init(USART1,&USARTInit);
		
		USART_Cmd(USART1, ENABLE);
	}
	static inline void waitForTransmit()
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	}
	static inline void sendByte(uint8_t x)
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		USART_SendData(USART1, x);
	}
	void print(uint8_t data)
	{
		this->sendByte(data);
	}
	void put(uint8_t data)
	{
		this->sendByte(data);
	}
	void print(int16_t data)
	{
		sprintf(this->buffer,"%d",data);
		this->print(buffer);
	}
	void println(int16_t data)
	{
		sprintf(this->buffer,"%d",data);
		this->print(buffer);
		this->newline();
	}
	void print(int32_t data)
	{
		sprintf(this->buffer,"%d",data);
		this->print(buffer);
	}
	void println(int32_t data)
	{
		sprintf(this->buffer,"%d",data);
		this->print(buffer);
		this->newline();
	}
	void newline()
	{
		this->put((uint8_t) '\r');
		this->put((uint8_t) '\n');
	}
	void println(uint8_t data)
	{
		this->print(data);
		this->newline();
	}
	void print(char * out)
	{
		uint8_t i = 0;
		while (*out)
		{
			this->sendByte(*out);
			out++;
		}
	}
	void println(char * out)
	{
		this->print(out);
		this->newline();
	}
	/*template<class T>
	 void println(T out)
	 {
	 this->print(out);
	 this->newline();
	 }*/
	bool available()
	{
		return (USART_GetFlagStatus(USART1, USART_FLAG_RXNE));
	}
	uint8_t read()
	{
		while( USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
		return USART_ReceiveData(USART1);
	}
};

int main(void)
{
  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);

  /* Add your application code here
     */
	MineUSART1 Serial;
	Serial.init();
	
	
	
	/* Infinite loop */
  while (1)
  {
		uint8_t x = Serial.read();
		Serial.print("Wpisales: ");
		Serial.println(x);
  }
}
