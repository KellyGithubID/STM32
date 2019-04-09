#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include <stdio.h>
#include <string.h>
#include "OLED_IIC.h"
#include "dht11.h"
#include "usart.h"

/************************************************    
 STM32F103C8T6     
 12864_IIC_OLED_STM32����
 OLED��ʾ����ģ��: SSD1306
 DHT11��ʪ�ȴ�����
 IIC: SCL, SDA
 ��Ӧ�ӿ�: PB0, PB1
 DHT11:VCC, DATA, GND
 ��Ӧ�ӿ�: 3V, PB11, GND
************************************************/ 
//extern unsigned char hanzi1[];

u8 num_to_str(const u8 number, char *str, u8 *len);

int main(void)
{
	u8 char_t = 't';
	u8 char_h = 'h';
	
	u16 t=0;			    
	u8 temperature = 100;  	    
	u8 humidity = 100;  
	u8 temp;
	u8 humi;
//	u16 result;
 
	delay_init();		  	 //��ʱ��ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 
	
	uart_init(9600);
	
	DHT11_Init();
	OLED_Init();		    //��ʼ��OLED
  initial_olcd();		  //��ʼ��
	clear_screen();			//����
	
	while(1)
	{
		if(t==500)
		{
			t = 0;
			DHT11_Read_Data(&temp,&humi);
			
			if (temp != temperature)
			{
				temperature = temp;
				
				USART_SendData(USART1, char_t);
				delay_ms(12);
				USART_SendData(USART1, temperature);
				delay_ms(12);
				
				while(USART_GetFlagStatus(USART1, USART_FLAG_TC)!=SET);
				USART_RX_STA = 0;
			}
			if (humi != humidity)
			{
				humidity = humi;
				
				USART_SendData(USART1, char_h);
				delay_ms(12);
				USART_SendData(USART1, humidity);
				delay_ms(12);
				
				while(USART_GetFlagStatus(USART1, USART_FLAG_TC)!=SET);
				USART_RX_STA = 0;
			}
			display_number_16x8(1,46,temperature);
			display_number_16x8(3,46,humidity);
		}
		++t;
	}
}




u8 num_to_str(const u8 number, char *str, u8 *len)
{
	u8 i;
	u8 cnt = 0;
	u8 tmp = number;
	u8 zero = 0;
	
	char verse[20];
	
	while(tmp>=zero)
	{
		if(cnt < *len)
		{
			verse[cnt++] = '0' + tmp%10;
			tmp /= 10;
		}
		else
		{
			return 1;
		}
	}
	
	*len = cnt;
	
	for(i=0;i<cnt;++i)
	{
		str[i] = verse[cnt-1-i];
	}
	
	return 0;
}

