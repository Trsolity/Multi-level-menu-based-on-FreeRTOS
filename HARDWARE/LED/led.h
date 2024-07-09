#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
//#define LED0 PAout(6)	// PA6
//#define LED1 PAout(5)	// PA5

//定义宏，方便代码的移植和管理：只需要更改后面名称即可
#define LED0_PIN		GPIO_Pin_0
#define LED1_PIN		GPIO_Pin_1
#define LED2_PIN		GPIO_Pin_2
#define LED3_PIN		GPIO_Pin_3
#define LED4_PIN		GPIO_Pin_4
#define LED5_PIN		GPIO_Pin_5
#define LED6_PIN		GPIO_Pin_6
#define LED7_PIN		GPIO_Pin_7

#define LED0	PAout(0)
#define LED1	PAout(1)
#define LED2	PAout(2)
#define LED3	PAout(3)
#define LED4	PAout(4)
#define LED5	PAout(5)
#define LED6	PAout(6)
#define LED7	PAout(7)

void LED_Init(void);//初始化

		 				    
#endif
