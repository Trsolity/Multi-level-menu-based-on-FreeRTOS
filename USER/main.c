#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "timer.h"
#include "rtc.h" 
#include "key.h"
#include "IIC.h"
#include "oled.h"
#include "MPU6050.h"

#include "mytask.h"

#include "FreeRTOS.h"
#include "task.h"

u8 ID;

/*----------------------------主函数-------------------------------*/

int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4	 
	delay_init();	    				//延时函数初始化	 
	uart_init(115200);					//初始化串口
	LED_Init();		  					//初始化LED
	KEY_Init();
	RTC_Init();
	printf("uart ok!\r\n");
	IIC_Init();							//IIC初始化
	
	OLED_Init();
	OLED_Clear();
	
	MPU6050_initialize();				//MPU6050初始化
	DMP_Init();							//DMP初始化
	TIM2_Init(10,7200-1);		//定时器初始化，完成MPU6050定时采集DMMP数据
	TIM_Cmd(TIM2,ENABLE);				//启动定时器
	
	ID = MPU6050_getDeviceID();
	printf("mpu6050的ID号为：%d\r\n",ID);
	
	
	printf("这是一个FreeRTOS的实验\r\n");
	
	FreeRTOS_Init();
}


