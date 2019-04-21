#ifndef __SDS011_H
#define __SDS011_H

#include "sys.h"
#include "delay.h"

#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收

extern u8 READ_VALUE_CMD[8];
extern u8 READ_PRECISION_CMD[8];

void SDS011_Init(void);
void SDS011_Get_Value(u16 *pm2, u16 *pm10);

#endif
