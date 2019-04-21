#include "sds011.h"
#include "usart.h"

void SDS011_Init(void)
{
	//GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3|RCC_APB2Periph_GPIOB, ENABLE);	//使能USART3，GPIOB时钟
  
	//USART3_TX   GPIOB.10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PA.10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB.10
   
  //USART3_RX	  GPIOB.11初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PA11
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB.11
	
	//Usart3 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
  //USART 初始化设置
	USART_InitStructure.USART_BaudRate = 9600;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART3, &USART_InitStructure); //初始化串口3
  USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);//开启串口接受中断
  USART_Cmd(USART3, ENABLE);                    //使能串口3
}

void SDS011_Get_Value(u16 *pm2, u16 *pm10)
{
	u8 i;
	u8 result[10];
	
	*pm2 = 0;
	*pm10 = 0;
	
	for(i=0;i<10;++i)
	{
		while(USART_GetFlagStatus(USART3, USART_FLAG_RXNE)!=SET);
		result[i] = USART_ReceiveData(USART3);
		if(result[i]==0xC0)		//通过指令号判断信号来源
		{
			i = 1;
		}
	}
	
	*pm2 |= ((result[3]<<8)|result[2]);
	*pm10 |= ((result[5]<<8)|result[4]);
	
	
	
//	for(i=0;i<10;++i)
//	{
//		USART_SendData(USART1, result[i]);
//		delay_ms(12);
//		while(USART_GetFlagStatus(USART1, USART_FLAG_TC)!=SET);
//		USART_RX_STA = 0;
//	}
//	while(USART_GetFlagStatus(USART3, USART_FLAG_RXNE)!=SET);
//	temp = USART_ReceiveData(USART3);
//	*pm2 |= temp;
//	
//	
//	USART_SendData(USART1, temp);
//	delay_ms(12);
//	while(USART_GetFlagStatus(USART1, USART_FLAG_TC)!=SET);
//	USART_RX_STA = 0;
//	
//	
//	while(USART_GetFlagStatus(USART3, USART_FLAG_RXNE)!=SET);
//	temp = USART_ReceiveData(USART3);
//	*pm2 |= (temp<<8);
//	
//	
//	USART_SendData(USART1, temp);
//	delay_ms(12);
//	while(USART_GetFlagStatus(USART1, USART_FLAG_TC)!=SET);
//	USART_RX_STA = 0;
//	
//	
//	while(USART_GetFlagStatus(USART3, USART_FLAG_RXNE)!=SET);
//	temp = USART_ReceiveData(USART3);
//	*pm10 |= temp;
//	
//	
//	USART_SendData(USART1, temp);
//	delay_ms(12);
//	while(USART_GetFlagStatus(USART1, USART_FLAG_TC)!=SET);
//	USART_RX_STA = 0;
//	
//	
//	while(USART_GetFlagStatus(USART3, USART_FLAG_RXNE)!=SET);
//	temp = USART_ReceiveData(USART3);
//	*pm10 |= (temp<<8);
//	
//	
//	USART_SendData(USART1, temp);
//	delay_ms(12);
//	while(USART_GetFlagStatus(USART1, USART_FLAG_TC)!=SET);
//	USART_RX_STA = 0;
//	
//	
//	for(i=0;i<4;++i)
//	{
//		while(USART_GetFlagStatus(USART3, USART_FLAG_RXNE)!=SET);
//		temp = USART_ReceiveData(USART3);
//		
//		
//		USART_SendData(USART1, temp);
//		delay_ms(12);
//		while(USART_GetFlagStatus(USART1, USART_FLAG_TC)!=SET);
//		USART_RX_STA = 0;
//	
//	
//	}
}
