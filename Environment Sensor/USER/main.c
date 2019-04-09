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
 DHT11温湿度传感器
 SGP30传感器
 CH340串口转USB
 SGP30:VCC, SCL, SDA, GND
 对应接口: 3V, PB6, PB7, GND
 DHT11:VCC, DATA, GND
 对应接口: 3V, PB11, GND
 CH340:TXD, RXD, GND
 对应接口: PA10, PA9, GND
************************************************/ 

int main(void)
{
	//标志信号，用于判断串口传输的气体数据
	u8 char_c = 'c';		//CO2, TVOC
	u8 char_t = 't';		//温度
	u8 char_h = 'h';		//湿度
	
	u16 t=0;			//用于设置时间间隔
	u16 co2 = 0;
	u16 tvoc = 0;
	u8 temperature = 100;  	    
	u8 humidity = 100;  
	u8 temp = 0;		//用于临时存储温度值
	u8 humi = 0;		//用于临时存储湿度值
 
	delay_init();		  	 //延时初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级 
	
	uart_init(9600);
	
	DHT11_Init();
	
	SGP30_Init();
	SGP30_Start_Measure();

//传感器初始化需要时间间隔
	delay_ms(50000);
	delay_ms(50000);
	delay_ms(50000);
	delay_ms(50000);
	delay_ms(50000);
	
	while(1)
	{
		if(t==500)		//经过一定时间间隔后读取传感器数据，避免数据连续刷新
		{
			
			t = 0;
			DHT11_Read_Data(&temp,&humi);		//读取温湿度
			
			if (temp != temperature)		//温度有变化则刷新温度值
			{
				temperature = temp;
				
				USART_SendData(USART1, char_t);
				delay_ms(12);
				USART_SendData(USART1, temperature);
				delay_ms(12);
				
				while(USART_GetFlagStatus(USART1, USART_FLAG_TC)!=SET);
				USART_RX_STA = 0;
			}
			if (humi != humidity)		//湿度有变化则刷新湿度值
			{
				humidity = humi;
				
				USART_SendData(USART1, char_h);
				delay_ms(12);
				USART_SendData(USART1, humidity);
				delay_ms(12);
				
				while(USART_GetFlagStatus(USART1, USART_FLAG_TC)!=SET);
				USART_RX_STA = 0;
			}
			
			
			Read_Air_Quality(&co2, &tvoc);		//读取CO2与TVOC含量
		
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
