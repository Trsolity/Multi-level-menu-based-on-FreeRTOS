#include "key.h"
#include "delay.h"
#include "usart.h"

//按键初始化函数 
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(KEY1_PORT_RCC,ENABLE);//使能端口时钟
	RCC_APB2PeriphClockCmd(KEY2_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY3_PORT_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(KEY4_PORT_RCC,ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	//端口重映射(功能关闭）,使作为GPIO管脚使用
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
		
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//配置GPIO管脚模式(上拉输入)，引脚，速度	
	GPIO_InitStructure.GPIO_Pin=KEY1_PIN;//按键1
	GPIO_Init(KEY1_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=KEY2_PIN;//按键2
	GPIO_Init(KEY2_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=KEY3_PIN;//按键3
	GPIO_Init(KEY3_PORT,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=KEY4_PIN;//按键4
	GPIO_Init(KEY4_PORT,&GPIO_InitStructure);
}


void initKeyHandler(KeyHandler *handler) 
{
    handler->status = KEY_UP;
    handler->event = NULL_PRESS;
    handler->longPressCount = 0;
}
 
void key_handler(KeyHandler *handler)
{
	unsigned char keyStatus=1;
	
	static unsigned char KEY1=1;
	static unsigned char KEY2=1;
	static unsigned char KEY3=1;
	static unsigned char KEY4=1;

	if (handler->longPressCount == 0 && handler->status != KEY_DOWN)//如果长按计数为0且按键状态不为NULL_PRESS
	{
		handler->event = NULL_PRESS; // 重置按键状态标志位（没有按键按下）
	}
    if (handler->status == KEY_DOWN) //如果有按键按下
	{
		handler->status = KEY_UP;
		
//		printf("开始按下的按键状态为%d  %d  %d  %d\r\n",GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15),GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_14),
//						GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_13),GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12));

		keyStatus=(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15) &&
				GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_13) && GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12));//读取实际的按键状态到keyStatus变量中
		//		&& GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_14) &&
    } 
	
	if (keyStatus == 0) //此时，按键在保持按下的状态
	{
		handler->longPressCount++;
//		printf("key的值在按键按下开始时为%d\r\n",KEY1);
//		printf("按下时的按键状态为%d  %d  %d  %d\r\n",GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15),GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_14),
//						GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_13),GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12));
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15) == 0) KEY1 = 0;
//		else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_14) == 0) KEY2 = 0;
		else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_13) == 0) KEY3 = 0;
		else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12) == 0) KEY4 = 0;
	}
	else    //按键松开后开始判断按键动作的具体信息
	{
		if (handler->longPressCount >= 4 && handler->longPressCount < 150) 
		{
			if(KEY1 == 0) 
			{
//				printf("key的值在按键松开时为%d\r\n",KEY1);
				handler->event = KEY1_PRESS;
			}
			else if (KEY2 == 0) handler->event = KEY2_PRESS;
			else if (KEY3 == 0) handler->event = KEY3_PRESS;
			else if (KEY4 == 0) handler->event = KEY4_PRESS;
			
		} 
		else 
		{
			handler->event = NULL_PRESS; 
		}
		KEY1 = 1;
		KEY2 = 1;
		KEY3 = 1;
		KEY4 = 1;
		
		handler->longPressCount = 0;
    }
}

//u8 Key_Scan(u8 mode)
//{
//	static u8 key=1;//保存上一次的值
//	if(mode==1)key=1;
//		
//	if(key==1 && (KEY1==0 || KEY2==0 || KEY3==0 || KEY4==0))  //(key=1才有效)
//	{
//		delay_ms(10);//按键按下，延时判断防抖
//		key=0;  //key=0的作用：防止当按键持续按下时，因多次进入函数造成多次检测扫描返回按键结果的情况
//		if(KEY1==0)
//			return KEY1_PRESS;
//		else if(KEY2==0)
//			return KEY2_PRESS;
//		else if(KEY3==0)
//			return KEY3_PRESS;
//		else if(KEY4==0)
//			return KEY4_PRESS;
//	}
//	else if(KEY1==1 && KEY2==1 && KEY3==1 && KEY4==1)//无按键按下
//	{
//		delay_ms(10);//延时判断防抖
//		if(KEY1==1 && KEY2==1 && KEY3==1 && KEY4==1)
//		key=1;
//	}
//	return 0;
//}
