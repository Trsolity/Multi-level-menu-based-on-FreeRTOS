#ifndef _DISPLAY_H
#define _DISPLAY_H	 

#include "MPU6050.h"
#include "oled.h"

/*宏定义区*/


/*外部变量声明区*/


/*函数声明区*/

void Display_Menu(u8 x);//显示菜单

void Display_Cursor(u8 x,u8 y,u8 z,u8 i);//显示光标(矩形框）

void Display_Down_Cursor(u8 x,u8 i);//显示底部的光标(矩形框）

void Display_Time(void);//显示时间分页面

void Display_MPU6050(void);//显示mpu6050数据

void Display_Choice(u8 c);

#endif

