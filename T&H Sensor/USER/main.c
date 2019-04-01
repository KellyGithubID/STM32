#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include <stdio.h>
#include <string.h>
#include "OLED_IIC.h"
#include "dht11.h"

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
extern unsigned char hanzi1[];
int main(void)
{
	u8 t=0;			    
	u8 temperature;  	    
	u8 humidity;  
 
	delay_init();		  	 //延时初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级 
	
	DHT11_Init();
	OLED_Init();		    //初始化OLED
  initial_olcd();		  //初始化
	clear_screen();			//清屏

	while(1)
	{
		disp_string_8x16_16x16(1,1,"温度:");
		disp_string_8x16_16x16(3,1,"湿度:");
		
		if(t%10==0)			//每100ms读取一次
		{									  
			DHT11_Read_Data(&temperature,&humidity);	//读取温湿度值	

			display_number_16x8(1,46,temperature);
			display_number_16x8(3,46,humidity);
		}				   
	 	delay_ms(10);
		t++;
		if(t==20)
		{
			t=0;
		}
	} 
}

