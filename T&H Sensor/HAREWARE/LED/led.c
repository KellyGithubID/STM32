#include "led.h"
#include "stm32f10x.h"

void LED_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC , ENABLE);  //GPIO C

	

//LED -> PC13
		 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;        //端口13
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度50MHz
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
//LED -> PC14
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
//LED -> PC15
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}

