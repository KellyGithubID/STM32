#include "sds011.h"
#include "usart.h"

void SDS011_Init(void)
{
	//GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3|RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��USART3��GPIOBʱ��
  
	//USART3_TX   GPIOB.10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PA.10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB.10
   
  //USART3_RX	  GPIOB.11��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PA11
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB.11
	
	//Usart3 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
  //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = 9600;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART3, &USART_InitStructure); //��ʼ������3
  USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);//�������ڽ����ж�
  USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���3
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
		if(result[i]==0xC0)		//ͨ��ָ����ж��ź���Դ
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
