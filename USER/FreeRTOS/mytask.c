#include "key.h"
#include "timer.h"
#include "display.h"
#include "mytask.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "event_groups.h"

/*--------------------------任务句柄---------------------------------*/

static TaskHandle_t StartTask_Handler = NULL;				//创建任务句柄
static TaskHandle_t Key_Task_Handler = NULL;				//Key_Task句柄
static TaskHandle_t Display_Menu_Task_Handler = NULL;		//Display_Menu_Task句柄
static TaskHandle_t Display_Time_Task_Handler = NULL;		//Display_Time_Task句柄
static TaskHandle_t Display_MPU6050_Task_Handler = NULL;	//Display_MPU6050_Task句柄

//static TaskHandle_t TaskGenericFunction_Handle = NULL;	//通用任务句柄


/*-------------------------内核对象句柄------------------------------*/

//static SemaphoreHandle_t CountSem_Handle = NULL;  		//计数型信号量句柄
static SemaphoreHandle_t BinarySem_Handle = NULL;			//二值信号量句柄
static QueueHandle_t Test_Queue = NULL;  					//消息队列句柄
static EventGroupHandle_t Event_Handle = NULL;  			//事件组句柄


/*-------------------------全局变量声明------------------------------*/

static volatile int AppTaskCreaterun = 0;  	// 创建任务运行时AppTaskCreaterun=1
static volatile int flagTask1run = 0;     	// 任务1运行时flagTask1run=1
static volatile int flagTask2run = 0;     	// 任务2运行时flagTask2run=1
u8 Key_Scan = 0;

u8 key1_down = 1;
u8 key2_down = 2;
u8 key3_down = 3;
u8 key4_down = 4;

/*--------------------------任务优先级-----------------------------*/

#define START_TASK_PRIO			1

#define KEY_TASK_PRIO			4
#define MENU_TASK_PRIO			4
#define TIME_TASK_PRIO			3
#define MPU6050_TASK_PRIO		3

#define TASK5_TASK_PRIO			2

/*-------------------------任务堆栈大小----------------------------*/
	
#define START_STK_SIZE 			128 

#define KEY_TASK_SIZE 			64
#define MENU_TASK_SIZE 			128
#define TIME_TASK_SIZE 			200  
#define MPU6050_TASK_SIZE 		128  

#define TASK5_STK_SIZE 			64 

/*--------------------------宏定义---------------------------------*/

#define QUEUE_LEN			4	//队列的长度，最大可包含多少个消息
#define QUEUE_SIZE			4	//队列中每个消息的大小（字节）

#define KEY1_EVENT (0x01 << 0)	//设置事件掩码的位0
#define KEY3_EVENT (0x01 << 1)	//设置事件掩码的位1
#define KEY4_EVENT (0x01 << 2)	//设置事件掩码的位2

/*--------------------------函数声明-------------------------------*/

void start_task(void *pvParameters);				//创建任务

void Key_Task(void *pvParameters);					//Key_Task任务的实现
void Display_Menu_Task(void *pvParameters);			//Display_Menu_Task任务的实现
void Display_Time_Task(void *pvParameters);			//Time_Task任务的实现
void Display_MPU6050_Task(void *pvParameters);		//Display_MPU6050_Task任务的实现

void FreeRTOS_Init(void)
{
	BaseType_t xReturn = pdPASS;

	//创建开始任务
    xReturn = xTaskCreate((TaskFunction_t )start_task,  //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄 
	if(pdPASS == xReturn)
		vTaskStartScheduler();          //开启任务调度
	
	while (1);
}
/*--------------------------各函数任务主体---------------------------------*/

//开始任务函数
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区

	/* 创建BinarySem_Handle */
	BinarySem_Handle = xSemaphoreCreateBinary();
	if(NULL != BinarySem_Handle)
		printf("BinarySem_Handle二值信号量创建成功!\r\n");
	
	/* 创建Test_Queue */
	Test_Queue = xQueueCreate((UBaseType_t) QUEUE_LEN,
								(UBaseType_t) QUEUE_SIZE);
	if(NULL != Test_Queue)
		printf("Test_Queue消息队列创建成功!\r\n");
	
	/* 创建Event_Handle */
	Event_Handle = xEventGroupCreate();
	if(NULL != Test_Queue)
		printf("Event_Handle事件组创建成功!\r\n");		
			
	//创建Key_Task任务
    xTaskCreate((TaskFunction_t )Key_Task,             
                (const char*    )"Key_Task",           
                (uint16_t       )KEY_TASK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )KEY_TASK_PRIO,        
                (TaskHandle_t*  )&Key_Task_Handler); 			
	printf("创建Key_Task成功!\r\n");
	
	//创建OLED显示任务
    xTaskCreate((TaskFunction_t )Display_Menu_Task,             
                (const char*    )"Display_Menu_Task",           
                (uint16_t       )MENU_TASK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )MENU_TASK_PRIO,        
                (TaskHandle_t*  )&Display_Menu_Task_Handler);  
	printf("创建Display_Menu_Task成功!\r\n");
				
	//创建Display_Time_Task任务
    xTaskCreate((TaskFunction_t )Display_Time_Task,             
                (const char*    )"Display_Time_Task",           
                (uint16_t       )TIME_TASK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )TIME_TASK_PRIO,        
                (TaskHandle_t*  )&Display_Time_Task_Handler); 			
	printf("创建Display_Time_Task成功!\r\n");
				
	//创建Display_Time_Tools_Task任务
    xTaskCreate((TaskFunction_t )Display_MPU6050_Task,             
                (const char*    )"Display_MPU6050_Task",           
                (uint16_t       )MPU6050_TASK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )MPU6050_TASK_PRIO,        
                (TaskHandle_t*  )&Display_MPU6050_Task_Handler); 			
	printf("创建Display_MPU6050_Task成功!\r\n");
				
				
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
				
}

//void Receive_Task(void *pvParameters)
//{
//	BaseType_t xReturn = pdTRUE;//定义一个创建信息返回值
//	u32 r_queue;
//	while(1)
//	{
//		xReturn = xQueueReceive(Test_Queue,		//消息队列的句柄
//								&r_queue,		//接收的消息内容
//								portMAX_DELAY);	//等待时间：一直等
//		if (pdTRUE == xReturn)


//void Send_Task(void *pvParameters)
//{
//	BaseType_t xReturn = pdTRUE;//定义一个创建信息返回值
//	u32 send_data1 = 111111;
//	xReturn = xQueueSend(Test_Queue,		//消息队列的句柄
//						&send_data1,		//发送的消息内容
//						0);					//等待时间：0		
//	if (pdPASS == xReturn)

	
void Key_Task(void *pvParameters)
{
	
	while(1)
	{		
//		printf("已经进入了Key_task任务函数\r\n");
		if(Key_Scan == key1_down)
		{
			/* KEY1被按下 */
			Key_Scan = 0;
			printf("KEY1被按下\r\n");
			xEventGroupSetBits(Event_Handle,KEY1_EVENT);
		}
		
		if(Key_Scan == key3_down)
		{
			/* KEY3被按下 */
			Key_Scan = 0;
			printf("KEY2被按下\r\n");
			xEventGroupSetBits(Event_Handle,KEY3_EVENT);
		}
		
		if(Key_Scan == key4_down)
		{
			/* KEY4被按下 */
			Key_Scan = 0;
			printf("KEY3被按下\r\n");
			xEventGroupSetBits(Event_Handle,KEY4_EVENT);
		}
		
		vTaskDelay(5);			//每20ms扫描一次
	}
}

//菜单显示任务函数
void Display_Menu_Task(void *pvParameters)
{
	
	EventBits_t r_event;
	BaseType_t xReturn = pdPASS;//定义一个创建信息返回值

	static signed char x = 0;//表示菜单页面内容选项（按键计数：0,1,2,3,4）
	static signed char y = 0;//表示光标所在位置（0,1,2）
	static signed char z = 0;//(z-1)表示光标向下超过屏幕的次数（0,1,2，3）
	static signed char i = 0;//(i-1)表示光标向上超过屏幕的次数（0,1,2，3）
	
	static signed char j = 0;//表示显示第几张页面（0,1,2）

	vTaskSuspend(Display_Time_Task_Handler);		//挂起时间界面显示任务
	vTaskSuspend(Display_MPU6050_Task_Handler);		//挂起陀螺仪数据显示任务

	Display_Menu(0);		//首次进入菜单，菜单页面以及光标位置的初始化
	Display_Cursor(0,0,0,1);

	while(1)
	{
//		printf("已经进入了Display_Task任务函数\r\n");
		xReturn = xSemaphoreTake(BinarySem_Handle,10);
		if(xReturn == pdTRUE)
		{
			Display_Menu(j);
			Display_Cursor(0,x,y,1);
			printf("进入菜单显示函数，二值信号量已被获取!\r\n");
		}
		
		r_event = xEventGroupWaitBits(Event_Handle,
									KEY1_EVENT|KEY3_EVENT|KEY4_EVENT,
									pdTRUE,
									pdFALSE,
									portMAX_DELAY);
		
		if (r_event == KEY1_EVENT)
		{
//			OLED_Clear();					//按键1按下，表示要切换界面了，先清屏				
	
			if(x == 0)						//如果光标停在选项0上面
			{
				OLED_Clear();					//按键1按下，表示要切换界面了，先清屏	
				xSemaphoreGive(BinarySem_Handle);	//释放一个二值信号量
				printf("菜单显示函数挂起，要进入时间界面任务了!\r\n");

				vTaskResume(Display_Time_Task_Handler);		//恢复时间界面显示任务
				printf("成功切换，时间界面显示函数已经进入!\r\n");
				vTaskSuspend(NULL);							//挂起菜单显示任务

			}
			if(x == 1)						//如果光标停在选项1上面
			{
				OLED_Clear();					//按键1按下，表示要切换界面了，先清屏
				printf("菜单显示函数挂起，要进入陀螺仪数据显示任务了!\r\n");

				vTaskResume(Display_MPU6050_Task_Handler);	//恢复陀螺仪界面显示任务
				
				vTaskSuspend(NULL);							//挂起菜单显示任务
			}

		}
		if ((r_event & (KEY3_EVENT|KEY4_EVENT)) == KEY3_EVENT)
		{
			i=0;
			Display_Cursor(0,x,y,0);		//显示光标位置，参数0表示清空
			x++,y++;
			if (x>4) x = 4;		
			if (y>2) y = 2;		
			if (y == 2) 
			{
				z++;			//光标向下超过屏幕次数加1，并清零向上的超出次数
			}
			//这里的3 = (总工具选项数) - (屏幕能容纳的选项数) + 1
			if (z>3) z = 4;		//先让z等于4，防止持续按下按键3时因满足下条件语句导致其不断清屏,	
								//应尽量避免清屏操作

			if((4>z) && (z>1) && (y == 2))	//有瑕疵，光标到达最后一个页面时向下翻仍会刷屏，可以改但就更臃肿了，那我把它看成特性吧
			{
				j++;
				if(j>2) j=2;
				OLED_Clear();				//要切换菜单界面显示，先清屏
				Display_Menu(j);
			}
	
			printf("--下，选中的工具选项= %d, 光标位置= %d, 下移显示页面= %d, 上移值= %d\r\n",x,y,j,i);
			
			if (z == 4) z = 3;				//在此时改变z的值
			Display_Cursor(0,x,y,1);		//参数1表示填充
					
		}
		if ((r_event & (KEY3_EVENT|KEY4_EVENT)) == KEY4_EVENT)
		{
			z=0;
			Display_Cursor(0,x,y,0);		//显示光标位置，参数0表示清空
			x--,y--;
			if (x<0) x = 0;		
			if (y<0) y = 0;		
			if (y == 0) 
			{
				i++;			//向上超过屏幕次数加1，并清零向下的超出次数
			}
			if (i>3) i = 4;		//先让i等于4，防止持续按下按键4时因满足下条件语句导致其不断清屏,	
								//应尽量避免清屏操作

			if((4>i) && (i>1) && (y == 0))
			{
				j--;
				if(j<0) j=0;
				OLED_Clear();			//要切换菜单界面显示，先清屏
				Display_Menu(j);
			}
			
			Display_Cursor(0,x,y,1);	//显示光标位置，参数1表示填充
			
			printf("++上，选中的工具选项= %d, 光标位置= %d, 下移值= %d, 上移显示页面= %d\r\n",x,y,z,j);
			if (i == 4) i = 3;			//在此时改变i的值
		}

		vTaskDelay(5);			//每20ms扫描一次
	}
}

//时间界面显示任务函数
void Display_Time_Task(void *pvParameters)
{
	EventBits_t r_event;
	BaseType_t xReturn = pdPASS;//定义一个创建信息返回值

	static signed char x = 0;
	static signed char y = 0;
	printf("进入了时间界面显示任务11111111!\r\n");
//	vTaskSuspend(Display_Menu_Task_Handler);		//挂起菜单显示任务

//	Display_Time();

	while(1)
	{
		Display_Time();
		printf("进入了时间界面显示任务，已在其中!\r\n");

		xReturn = xSemaphoreTake(BinarySem_Handle,5);
		if(xReturn == pdTRUE)		//如果是刚从别的任务跳转过来的，对刷新的屏幕显示有要求，应初始化
		{			
			x = 0;y = 0;
			Display_Cursor(1,x,x,1);		//进入该任务，光标放在第一个工具选项上
			printf("进入时间显示任务，二值信号量已被获取!\r\n");
		}

		r_event = xEventGroupWaitBits(Event_Handle,
									KEY1_EVENT|KEY3_EVENT|KEY4_EVENT,
									pdTRUE,
									pdFALSE,
									10);
		
		if (r_event == KEY1_EVENT)
		{

			if(x == 3)					//如果光标停在“back”键上面
			{
				OLED_Clear();			//按键1按下，表示要切换界面了				
				xSemaphoreGive(BinarySem_Handle);//释放一个二值信号量
				printf("在时间显示任务中，释放一个二值信号量!\r\n");

				vTaskResume(Display_Menu_Task_Handler);		//恢复菜单显示任务
				vTaskSuspend(NULL);		//挂起时间显示任务
			}
		}
		
		if ((r_event & (KEY3_EVENT|KEY4_EVENT)) == KEY3_EVENT)
		{
			Display_Cursor(1,y,y,0);		//表示清空光标
			Display_Down_Cursor(0,0);		//清除底部光标

			x++,y++;
			if (y>2) y=2;	//2:屏幕上显示的工具选项
			if (x>3) x=3;	//3:总共的选项（包括返回键）
			if (x==3) 
			{
				Display_Down_Cursor(0,1);	//计数4次，光标到达底部，填充底部，页面为0；
				Display_Cursor(1,y,y,0);		//表示清除光标
			}
			if((0<=x) && (x<3))
			{
				Display_Down_Cursor(0,0);		//清除底部光标
				Display_Cursor(1,y,y,1);		//表示填充光标
			}
			
						
		}
		if ((r_event & (KEY3_EVENT|KEY4_EVENT)) == KEY4_EVENT)
		{
			Display_Cursor(1,x,x,0);		//表示清空光标
			Display_Down_Cursor(0,0);		//清除底部光标
	
			if (x!=3) y--;
			x--;
			if (x<0) x=0;
			if (y<0) y=0;
			Display_Cursor(1,x,x,1);		//表示填充光标
		}

		vTaskDelay(5);			//每20ms扫描一次
	}
}

//陀螺仪界面显示任务
void Display_MPU6050_Task(void *pvParameters)
{
	EventBits_t r_event;
	
	while(1)
	{
		Display_MPU6050();
		Display_Down_Cursor(1,1);	//显示底部的光标
		printf("进入了陀螺仪数据显示任务!\r\n");

		r_event = xEventGroupWaitBits(Event_Handle,
									KEY1_EVENT|KEY3_EVENT|KEY4_EVENT,
									pdTRUE,
									pdFALSE,
									10);
		
		if (r_event == KEY1_EVENT)
		{
			OLED_Clear();				//按键1按下，表示要切换界面了				
			xSemaphoreGive(BinarySem_Handle);	//释放一个二值信号量

			vTaskResume(Display_Menu_Task_Handler);		//恢复菜单显示任务
			vTaskSuspend(NULL);			//挂起陀螺仪显示任务
		}		

		vTaskDelay(5);			//每20ms扫描一次
	}
}

//空闲任务钩子函数
void vApplicationIdleHook(void)
{
	AppTaskCreaterun = 0;  // 创建任务运行时AppTaskCreaterun=1
	flagTask1run = 0;     // 任务1运行时flagTask1run=1
	flagTask2run = 0;     // 任务2运行时flagTask2run=1	
}
