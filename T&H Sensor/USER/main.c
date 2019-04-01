#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include <stdio.h>
#include <string.h>
#include "OLED_IIC.h"
#include "dht11.h"

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
extern unsigned char hanzi1[];
int main(void)
{
	u8 t=0;			    
	u8 temperature;  	    
	u8 humidity;  
 
	delay_init();		  	 //��ʱ��ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 
	
	DHT11_Init();
	OLED_Init();		    //��ʼ��OLED
  initial_olcd();		  //��ʼ��
	clear_screen();			//����

	while(1)
	{
		disp_string_8x16_16x16(1,1,"�¶�:");
		disp_string_8x16_16x16(3,1,"ʪ��:");
		
		if(t%10==0)			//ÿ100ms��ȡһ��
		{									  
			DHT11_Read_Data(&temperature,&humidity);	//��ȡ��ʪ��ֵ	

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

