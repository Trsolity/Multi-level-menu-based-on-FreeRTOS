#include "led.h"

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

//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PA端口时钟
	
	 GPIO_InitStructure.GPIO_Pin = LED0_PIN;				 //LED0-->PA.0 端口配置
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.0
	 GPIO_SetBits(GPIOA,LED0_PIN);						 	 //PA.0 输出高

	 GPIO_InitStructure.GPIO_Pin = LED1_PIN;	    		 //LED1-->PA.1 端口配置, 推挽输出
	 GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
	 GPIO_SetBits(GPIOA,LED1_PIN); 						 	 //PA.1 输出高 
	
	GPIO_InitStructure.GPIO_Pin=LED2_PIN;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_SetBits(GPIOA,LED2_PIN);						     //设置管脚为高电平，使LED初始状态为熄灭状态
	
	GPIO_InitStructure.GPIO_Pin=LED3_PIN;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_SetBits(GPIOA,LED3_PIN);						     //设置管脚为高电平，使LED初始状态为熄灭状态
	
	GPIO_InitStructure.GPIO_Pin=LED4_PIN;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_SetBits(GPIOA,LED4_PIN);						     //设置管脚为高电平，使LED初始状态为熄灭状态
	
	GPIO_InitStructure.GPIO_Pin=LED5_PIN;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_SetBits(GPIOA,LED5_PIN);						     //设置管脚为高电平，使LED初始状态为熄灭状态
	
	GPIO_InitStructure.GPIO_Pin=LED6_PIN;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_SetBits(GPIOA,LED6_PIN);						     //设置管脚为高电平，使LED初始状态为熄灭状态
	
	GPIO_InitStructure.GPIO_Pin=LED7_PIN;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_SetBits(GPIOA,LED7_PIN);						     //设置管脚为高电平，使LED初始状态为熄灭状态
}
 
