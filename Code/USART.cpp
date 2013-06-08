#include "main.h"


RCC_ClocksTypeDef RCC_Clocks;

struct MineUSART
{
	char buffer[33];
	USART_TypeDef * USARTx;
	MineUSART()
	{
	}
	void init(USART_TypeDef * USARTxx, GPIO_TypeDef * GPIOx, uint16_t TxPin, uint16_t RxPin, int32_t baudrate)
	{
		this->USARTx = USARTxx;
		#if defined(USART1)
		if( USARTxx == USART1 )
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
		}
		#endif
		#if defined(USART2) //TODO: extend
		#endif
		
		#if defined(GPIOA)
		if( GPIOx == GPIOA )
		{
			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
		}
		#endif
		
		
		GPIO_PinAFConfig(GPIOx, RxPin, GPIO_AF_1);
		GPIO_PinAFConfig(GPIOx, TxPin, GPIO_AF_1);
		
		
		GPIO_InitTypeDef GPIOInit;
		GPIOInit.GPIO_Mode = GPIO_Mode_AF;
		GPIOInit.GPIO_OType = GPIO_OType_PP;
		GPIOInit.GPIO_Pin = RxPin | TxPin;
		GPIOInit.GPIO_PuPd = GPIO_PuPd_UP;
		GPIOInit.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOx,&GPIOInit);
		
		USART_InitTypeDef USARTInit;
		USARTInit.USART_BaudRate = baudrate;//115200;//9600;
		USARTInit.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USARTInit.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USARTInit.USART_Parity = USART_Parity_No;
		USARTInit.USART_StopBits = USART_StopBits_1;
		USARTInit.USART_WordLength = USART_WordLength_8b;
		
		USART_Init(USARTxx,&USARTInit);
		
		USART_Cmd(USARTxx, ENABLE);
	}
	void waitForTransmit()
	{
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	}
	void sendByte(uint8_t x)
	{
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
		USART_SendData(USARTx, x);
	}
	void print(uint8_t data)
	{
		sendByte(data);
	}
	void put(uint8_t data)
	{
		sendByte(data);
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
	bool available()
	{
		return (USART_GetFlagStatus(USARTx, USART_FLAG_RXNE));
	}
	uint8_t read()
	{
		while( USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET);
		return USART_ReceiveData(USARTx);
	}
};

int main(void)
{
  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);

  /* Add your application code here
     */
	MineUSART Serial;
	Serial.init(USART1, GPIOA, GPIO_Pin_9, GPIO_Pin_10, 9600);
	
	/* Infinite loop */
  while (1)
  {
		//uint8_t x = Serial.read();
		//Serial.print("Wpisales: ");
		//Serial.println(x);
  }
}
