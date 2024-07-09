#include "timer.h"
#include "led.h"
#include "key.h"
#include "usart.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 

KeyHandler myKeyHandler;


//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;  //先占优先级4级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx					 
}

//通用定时器2中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器2!
void TIM2_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 	//时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; 				//设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 				//设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 		//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 				//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
 	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //清除中断标志位

	TIM_ITConfig( TIM2,TIM_IT_Update|TIM_IT_Trigger,ENABLE);		//使能定时器2更新触发中断
 
	TIM_Cmd(TIM2, ENABLE);  //使能TIMx外设
 	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  				//TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  		//先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  			//从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 				//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure); 
}


//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
//		printf("TIM3输出.......\r\n");
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
}

//定时器2中断服务函数
void TIM2_IRQHandler(void)
{
	
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //溢出中断
	{
		static uint16_t count = 0;
	//**********************************************************//
		if(count%5 == 0 )
		{
//			if((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15) == 0) || (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_14) == 0) ||
//				(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_13) == 0) || (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12) == 0))
			if((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15)  &&
				GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_13) && GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12)) == 0)
			{
				myKeyHandler.status = KEY_DOWN;
			}
			key_handler(&myKeyHandler);
			
			if (myKeyHandler.event == KEY1_PRESS)
				Key_Scan = key1_down;
			else if (myKeyHandler.event == KEY2_PRESS)
				Key_Scan = key2_down;
			else if (myKeyHandler.event == KEY3_PRESS)
				Key_Scan = key3_down;
			else if (myKeyHandler.event == KEY4_PRESS)
				Key_Scan = key4_down;
			
			count=0;
		}
		count++;
	//*************************************************************//
		
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //清除中断标志位
}












