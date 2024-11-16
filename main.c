/*
 * EmbeddedAdvancedTasks.c
 *
 * Created: 10/21/2024 4:32:56 PM
 * Author : Demiana
 */ 

#define F_CPU 8000000
#include <util/delay.h>
#include "StdTypes.h"

#include "DIO_Interface.h"
#include "Ex_Interrupt_Interface.h"

#include "LCD_Interface.h"
#include "TrafficLight_Interface.h"
#include "FixedPoint_Interface.h"
#include "EventBased_Interface.h"

#include "Graphical_LCD_Interface.h"
#include "Queue_Interface.h"


#define TRAFFIC_LIGHT    0
#define FLOATING         1
#define EVENT_BASED      2
#define TEST             3
#define FLAPPY_BIRD      4
#define APPLICATION      FLAPPY_BIRD

#if APPLICATION == TRAFFIC_LIGHT
int main(void)
{
    /* Replace with your application code */
	TrafficLight_voidInit();
    while (1) 
    {
		TrafficLight_voidRunnable();
    }
}
#elif APPLICATION ==FLOATING


int main(void)
{
	/* Replace with your application code */
	DIO_Init();
	LCD_Init();
	LCD_SetCurser(0,0);
	s32 num;
	num=FixedPoint_Division(5.5,7.5);
	num=FixedPoint_ConvertFixedPointToFloat(num);
	LCD_SetCurser(0,0);
	LCD_WriteNumber(num/10);
	LCD_SetCurser(0,2);
	LCD_WriteString(".");
	LCD_WriteNumber(num%10);
	while (1)
	{
		
	}
}

#elif APPLICATION==EVENT_BASED
int main()
{
	EventBased_Init();
	while(1)
	{
		EventBased_Runnable();
	}
}
#elif APPLICATION==TEST

int main()
{
	DIO_Init();
	GLCD_voidInit();
	u8 i;
	//GLCD_voidWriteString(0,3,"Welcome to Flappy Bird");

	//GLCD_voidWriteNymber(5,1,2);
	u8 start=10,end=20;
	u8 flag=0;
	while(1)
	{
		for(i=1;i<128;i++)
		{
			GLCD_voidPrintBird(3,i);
			if(flag==0)
			{
				GLCD_voidDrawBox(start,end,0,2);
				GLCD_voidDrawBox(start+30,end+30,0,1);
				GLCD_voidDrawBox(start+10,end+10,5,7);
				GLCD_voidDrawBox(start+60,end+60,6,7);
				
				flag=1;
			}
			
			_delay_ms(2);
			//GLCD_voidWriteNymber(5,3,i);
			GLCD_voidClearDot(3,i-1);
			if(i%20==0)
			{
				GLCD_voidClearBox(start,end,0,2);
				GLCD_voidClearBox(start+30,end+30,0,1);
				GLCD_voidClearBox(start+10,end+10,5,7);
				GLCD_voidClearBox(start+60,end+60,6,7);
				start+=10;
				end+=10;
				if(start>=124 || end>=124)
				{
					start=0;
					end=20;
				}
				flag=0;
			}
		}
		flag=0;
		GLCD_voidClearDot(3,i);
		//GLCD_voidWriteString(5,3,"   ");
// 		GLCD_voidClearBox(start,end,0,2);
// 		GLCD_voidClearBox(start+30,end+30,0,1);
// 		GLCD_voidClearBox(start+10,end+10,5,7);
// 		GLCD_voidClearBox(start+60,end+60,6,7);
// 		start+=10;
// 		end+=10;
// 		if(start>=124 || end>=124)
// 		{
// 			start=0;
// 			end=20;
// 		}
// 		for(i=1;i<128;i++)
// 		{
// 			GLCD_voidPrintBird(3,i);
// 			_delay_ms(4);
// 			GLCD_voidWriteNymber(5,3,i);
// 			GLCD_voidClearDot(3,i-1);
// 		}
// 		GLCD_voidClearDot(3,i);
// 		GLCD_voidWriteString(5,3,"   ");
		
	}
}

#elif APPLICATION==FLAPPY_BIRD
int main()
{
	FlappyBird_voidInit();
	while(1)
	{
		FlappyBird_voidRunnable();
	}
}
#endif
