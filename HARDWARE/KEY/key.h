#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
 
extern u8 Key_Scan;

extern u8 key1_down;
extern u8 key2_down;
extern u8 key3_down;
extern u8 key4_down;

#define KEY1_PORT		GPIOA
#define KEY1_PIN		GPIO_Pin_15
#define KEY1_PORT_RCC	RCC_APB2Periph_GPIOA

#define KEY2_PORT		GPIOA
#define KEY2_PIN		GPIO_Pin_14
#define KEY2_PORT_RCC	RCC_APB2Periph_GPIOA

#define KEY3_PORT		GPIOA
#define KEY3_PIN		GPIO_Pin_13
#define KEY3_PORT_RCC	RCC_APB2Periph_GPIOA

#define KEY4_PORT		GPIOA
#define KEY4_PIN		GPIO_Pin_12
#define KEY4_PORT_RCC	RCC_APB2Periph_GPIOA

//#define KEY1	PAin(15)
//#define KEY2	PAin(14)
//#define KEY3	PAin(13)
//#define KEY4	PAin(12)

//#define KEY1_PRESS		1
//#define KEY2_PRESS		2
//#define KEY3_PRESS		3
//#define KEY4_PRESS		4

void KEY_Init(void);
//u8 Key_Scan(u8 mode);


/*定义按键状态枚举这个枚举定义了按键可能的两种状态：KEY_DOWN 表示按键按下，KEY_UP 表示按键抬起。*/
typedef enum {
    KEY_DOWN,
    KEY_UP,
} KeyStatus;
 
/*定义按键事件枚举，这个枚举定义了按键可能的三种事件：NULL_PRESS 表示没有按键事件，SHORT_PRESS 表示短按，LONG_PRESS 表示长按。*/
//而我把这些事件改成了分别按下KEY1,KET2,KEY3,KEY4和无按键按下
typedef enum {
	KEY1_PRESS,
    KEY2_PRESS,
	KEY3_PRESS,
	KEY4_PRESS,
	NULL_PRESS
} KeyEvent;
 
/*定义按键配置结构体，这个结构体包含了按键的状态 (status)、事件类型 (event) 和长按计数 (longPressCount)。它用于存储按键的相关信息。*/
typedef struct {
    KeyStatus status;
    KeyEvent event;
    uint16_t longPressCount;
} KeyHandler;
 
void initKeyHandler(KeyHandler *handler);
void key_handler(KeyHandler *handler);
/*
Key_Init 函数可能用于初始化按键硬件，但在提供的代码中并没有具体的实现。
initKeyHandler 函数用于初始化 KeyHandler 结构体的实例，将按键的状态、事件和计数等设置为初始值。
key_handler 函数用于处理按键的状态和事件。它接受一个 KeyHandler 结构体的实例作为参数，并根据实际情况更新结构体的成员。
这些代码提供了一个基本的框架，可以用于处理按键的状态和事件。你需要根据具体的硬件和需求实现 Key_Init 函数以初始化按键硬件，并根据实际情况实现 key_handler 函数。*/

#endif

