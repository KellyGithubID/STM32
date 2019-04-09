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
 12864_IIC_OLED_STM32驱动
 OLED显示驱动模块: SSD1306
 DHT11温湿度传感器
 IIC: SCL, SDA
 对应接口: PB0, PB1
 DHT11:VCC, DATA, GND
 对应接口: 3V, PB11, GND
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
 
	delay_init();		  	 //延时初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级 
	
	uart_init(9600);
	
	DHT11_Init();
	OLED_Init();		    //初始化OLED
  initial_olcd();		  //初始化
	clear_screen();			//清屏
	
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

