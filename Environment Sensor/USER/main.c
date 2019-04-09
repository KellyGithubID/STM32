#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include <stdio.h>
#include <string.h>
#include "dht11.h"
#include "sgp30.h"
#include "usart.h"

/************************************************    
 STM32F103C8T6     
 DHT11��ʪ�ȴ�����
 SGP30������
 CH340����תUSB
 SGP30:VCC, SCL, SDA, GND
 ��Ӧ�ӿ�: 3V, PB6, PB7, GND
 DHT11:VCC, DATA, GND
 ��Ӧ�ӿ�: 3V, PB11, GND
 CH340:TXD, RXD, GND
 ��Ӧ�ӿ�: PA10, PA9, GND
************************************************/ 

int main(void)
{
	//��־�źţ������жϴ��ڴ������������
	u8 char_c = 'c';		//CO2, TVOC
	u8 char_t = 't';		//�¶�
	u8 char_h = 'h';		//ʪ��
	
	u16 t=0;			//��������ʱ����
	u16 co2 = 0;
	u16 tvoc = 0;
	u8 temperature = 100;  	    
	u8 humidity = 100;  
	u8 temp = 0;		//������ʱ�洢�¶�ֵ
	u8 humi = 0;		//������ʱ�洢ʪ��ֵ
 
	delay_init();		  	 //��ʱ��ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 
	
	uart_init(9600);
	
	DHT11_Init();
	
	SGP30_Init();
	SGP30_Start_Measure();

//��������ʼ����Ҫʱ����
	delay_ms(50000);
	delay_ms(50000);
	delay_ms(50000);
	delay_ms(50000);
	delay_ms(50000);
	
	while(1)
	{
		if(t==500)		//����һ��ʱ�������ȡ���������ݣ�������������ˢ��
		{
			
			t = 0;
			DHT11_Read_Data(&temp,&humi);		//��ȡ��ʪ��
			
			if (temp != temperature)		//�¶��б仯��ˢ���¶�ֵ
			{
				temperature = temp;
				
				USART_SendData(USART1, char_t);
				delay_ms(12);
				USART_SendData(USART1, temperature);
				delay_ms(12);
				
				while(USART_GetFlagStatus(USART1, USART_FLAG_TC)!=SET);
				USART_RX_STA = 0;
			}
			if (humi != humidity)		//ʪ���б仯��ˢ��ʪ��ֵ
			{
				humidity = humi;
				
				USART_SendData(USART1, char_h);
				delay_ms(12);
				USART_SendData(USART1, humidity);
				delay_ms(12);
				
				while(USART_GetFlagStatus(USART1, USART_FLAG_TC)!=SET);
				USART_RX_STA = 0;
			}
			
			
			Read_Air_Quality(&co2, &tvoc);		//��ȡCO2��TVOC����
		
			USART_SendData(USART1, char_c);
			delay_ms(12);
			USART_SendData(USART1, co2>>8);
			delay_ms(12);
			USART_SendData(USART1, co2&0x00ff);
			delay_ms(12);
			USART_SendData(USART1, tvoc>>8);
			delay_ms(12);
			USART_SendData(USART1, tvoc&0x00ff);
			delay_ms(12);
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC)!=SET);
			USART_RX_STA = 0;
		}
		
		
		++t;
	}
}
