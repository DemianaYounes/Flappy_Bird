/*
 * FlappyBird.c
 *
 * Created: 11/8/2024 4:34:19 AM
 *  Author: Demiana Younes
 */ 

#define F_CPU   8000000
#include "avr/delay.h"
#include "StdTypes.h"
#include "Utils.h"
#include "MemMap.h"

#include "DIO_Interface.h"

#include "Ex_Interrupt_Interface.h"
#include "Timer_Interface.h"
#include "Graphical_LCD_Interface.h"

#include "FixedPoint_Interface.h"

#include "Stack_Interface.h" 
#include "Queue_Interface.h"

#include "FlappyBird_Interface.h"
#include "FlappyBird_Private.h"
#include "FlappyBird_Config.h"



static volatile s8 BirdFlag=START_PAGE_FOR_BIRD;
static volatile u8 Flag=0;
static volatile u8 PauseStartFlag=PAUSE_VALUE;
static volatile u8 ObstacleStart[4]={50,80,70,100},ObstacleEnd[4]={60,90,90,110};
static volatile u8 ObstacleTop[2]={6,2};
static volatile u8 TotalScore=0;


extern void(*currentState)(void);
CircularQueue_type Queue;

static void CallBack_INT0()
{
	BirdFlag--;
	if(BirdFlag<0)
	{
		BirdFlag=0;
	}
	Flag=1;
}
static void CallBack_INT1()
{
	TotalScore=0;
	PauseStartFlag=5;
	Stack_Pop();
}
void CallBack_Timer1()
{
	static u8 Local_u8Counter=0;
	if(Local_u8Counter==3)
	{
		Enqueue(&Queue,1);
		Local_u8Counter=0;
	}
	Local_u8Counter++;
}
void FlappyBird_voidInit()
{
	DIO_Init();
	EXI_TriggerEdge(EX_INT0,FALLING_EDGE);
	EXI_TriggerEdge(EX_INT1,FALLING_EDGE);
	EXI_SetCallBack(EX_INT0,CallBack_INT0); 
	EXI_SetCallBack(EX_INT1,CallBack_INT1);
	Timer1_Init(TIMER1_CTC_OCRA_TOP_MODE,TIMER1_SCALER_64);
	Timer1_OCRA1Mode(OCRB_NON_INVERTING);
	OCR1A=124999;   //1 second
	Timer1_OCA_SetCallBack(CallBack_Timer1);
	GLCD_voidInit();
	sei();  // Enable global interrupts
	EXI_Enable(EX_INT0);
	EXI_Enable(EX_INT1);
	Timer1_OCA_InterruptEnable();
	CircularQueue_Init(&Queue);
	Stack_Push(FlappyBird_voidGameOver);
	Stack_Push(FlappyBird_voidStartGame);
}

void FlappyBird_voidRunnable()
{
	while(1)
	{
		if(READ_BIT(PauseStartFlag,0)==1)
		{
			if (currentState != NULL)
			{
				currentState();
			}
			else
			{
				/*Do Nothing*/
			}
		}
		else
		{
			FlappyBird_voidPressButtonWaiting();
		}
	}
}



static void FlappyBird_voidStartGame()
{
	u8 Local_u8Iterator,Local_u8GameOver=0,Local_u8Iterator2;
    u8 Local_u8Offest=0;
	u8 Local_u8StartCounter=0;
	u8 Local_u8BirdSpeed=0;
	GLCD_voidClear();
	while(READ_BIT(PauseStartFlag,2)==START_VALUE)
	{	
		Local_u8BirdSpeed=FlappyBird_voidBirdSpeed();
		if(IsEmpty(&Queue)!=QUEUE_EMPTY)
		{
			DeQueue(&Queue,&Local_u8StartCounter);
		}
		if(Local_u8StartCounter==1)
		{
			FlappyBird_ClearObstacles(Local_u8Offest);
			FlappyBird_ResetObstacles();
			FlappyBird_DrawObstacles(Local_u8Offest);
			Local_u8StartCounter=0;
		}
		else
		{
			FlappyBird_ClearObstacles(Local_u8Offest);
			Local_u8Offest+=10;
			FlappyBird_DrawObstacles(Local_u8Offest);
			TotalScore+=2;
		}
		for(Local_u8Iterator=1;Local_u8Iterator<128;Local_u8Iterator++)
		{
			Local_u8GameOver=FlappyBird_CheckObstacles(Local_u8Offest,Local_u8Iterator);
			if(Local_u8GameOver==1)
			{
				CLR_BIT(PauseStartFlag,2);
				break;
			}
			MovementBird(Local_u8Iterator,Local_u8BirdSpeed);
			if(Local_u8Iterator==90)
			{
				FlappyBird_ClearObstacles(Local_u8Offest);
				Local_u8Offest+=10;
				FlappyBird_DrawObstacles(Local_u8Offest);
				TotalScore++;
			}
			
		}
		GLCD_voidClearDot(BirdFlag,Local_u8Iterator);
		if(Local_u8GameOver==1)
		{
			SET_BIT(PauseStartFlag,1);
			Stack_Pop();
			Stack_Push(FlappyBird_voidGameOver);
			Stack_Push(FlappyBird_voidStartGame);
			break;
		}
	}	
}

static void MovementBird(u8 Copy_u8Iterator,u8 Local_u8BirdSpeed)
{
	GLCD_voidPrintBird(BirdFlag,Copy_u8Iterator);
	_delay_ms(2);
	GLCD_voidClearDot(BirdFlag,Copy_u8Iterator-1);
	if(Copy_u8Iterator%Local_u8BirdSpeed==0)
	{
		GLCD_voidClearBird(BirdFlag,Copy_u8Iterator);
		BirdFlag++;
		if(BirdFlag>7)
		{
			BirdFlag=7;
		}
	}
	if(Flag==1)
	{
		GLCD_voidClearBird(BirdFlag+1,Copy_u8Iterator-1);
		Flag=0;
	}
}
static void FlappyBird_DrawObstacles(u8 Copy_u8Offest)
{
	GLCD_voidDrawBox((ObstacleStart[0]+Copy_u8Offest)%127,(ObstacleEnd[0]+Copy_u8Offest)%127,ObstacleTop[0],7);
	GLCD_voidDrawBox((ObstacleStart[1]+Copy_u8Offest)%127,(ObstacleEnd[1]+Copy_u8Offest)%127,ObstacleTop[0],7);
	GLCD_voidDrawBox((ObstacleStart[2]+Copy_u8Offest)%127,(ObstacleEnd[2]+Copy_u8Offest)%127,0,ObstacleTop[1]);
	GLCD_voidDrawBox((ObstacleStart[3]+Copy_u8Offest)%127,(ObstacleEnd[3]+Copy_u8Offest)%127,0,ObstacleTop[1]);
}

static u8 FlappyBird_CheckObstacles(u8 Copy_u8Offest, u8 Copy_u8birdX)
{
	u8 Local_u8Iterator,Local_u8GameOver=0;
	for(Local_u8Iterator=0;Local_u8Iterator<4;Local_u8Iterator++)
	{
		Local_u8GameOver=FlappyBird_CheckObstacles2((ObstacleStart[Local_u8Iterator]+Copy_u8Offest)%127,(ObstacleEnd[Local_u8Iterator]+Copy_u8Offest)%127,Copy_u8birdX,Local_u8Iterator);
		if(Local_u8GameOver==1)
		{
			break;
		}
	}
	return Local_u8GameOver;
}

static void FlappyBird_ResetObstacles()
{
	static u8 ObstacleReset=0;
	// Reset obstacle positions
	ObstacleStart[0] = 0 ; ObstacleEnd[0] = 10+ObstacleReset;
	ObstacleStart[1] = 50; ObstacleEnd[1] = 60+ObstacleReset;
	ObstacleStart[2] = 70; ObstacleEnd[2] = 80+ObstacleReset;
	ObstacleStart[3] = 10; ObstacleEnd[3] = 20+ObstacleReset;
	ObstacleTop[0]   = 5;  ObstacleTop[1] = 2;
	ObstacleReset+=5;
	if(ObstacleReset==35)
	{
		ObstacleReset=0;
	}
}

static void FlappyBird_ClearObstacles(u8 Copy_u8Offest)
{
	GLCD_voidClearBox((ObstacleStart[0]+Copy_u8Offest)%127,(ObstacleEnd[0]+Copy_u8Offest)%127,ObstacleTop[0],7);
	GLCD_voidClearBox((ObstacleStart[1]+Copy_u8Offest)%127,(ObstacleEnd[1]+Copy_u8Offest)%127,ObstacleTop[0],7);
	GLCD_voidClearBox((ObstacleStart[2]+Copy_u8Offest)%127,(ObstacleEnd[2]+Copy_u8Offest)%127,0,ObstacleTop[1]);
	GLCD_voidClearBox((ObstacleStart[3]+Copy_u8Offest)%127,(ObstacleEnd[3]+Copy_u8Offest)%127,0,ObstacleTop[1]);
}

static u8 FlappyBird_voidGameOver()
{
	GLCD_voidClear();
	GLCD_voidDrawSadFace(20,1);
	GLCD_voidWriteString(1,70,"Game Over");
	GLCD_voidWriteString(3,60,"Total Score");
	GLCD_voidWriteString(4,60,"=");
	GLCD_voidWriteNymber(4,70,TotalScore);
	while(READ_BIT(PauseStartFlag,1)==1)
	{
		GLCD_voidCryMoving(20,3);
		BirdFlag=3;
		_delay_ms(20);
	}
}


static u8 FlappyBird_CheckObstacles2(u8 Copy_u8Start,u8 Copy_u8End ,u8 Copy_u8Iterator,u8 Copy_u8Part)
{
	u8 Local_u8Status=0;
	if( ((BirdFlag>=ObstacleTop[0] && Copy_u8Part<2) || (BirdFlag<=ObstacleTop[1] && Copy_u8Part>=2)) && (Copy_u8Iterator>=Copy_u8Start && Copy_u8Iterator<=Copy_u8End) )
	{
		Local_u8Status=1;
	}
	return Local_u8Status;
}

static void FlappyBird_voidPressButtonWaiting()
{
	GLCD_voidWriteString(1,40,"Press to start");
	GLCD_voidBigBirdMoving(20,3);
}

static u8 FlappyBird_voidBirdSpeed()
{
	static float Local_BirdSpeed=3.5;
	static u8 Local_u8IncreaseValue=10;
	u32 Local_u8BirdSpeed;
	Local_u8BirdSpeed=FixedPoint_Subtraction(Local_BirdSpeed,((float)Local_u8IncreaseValue/100.0));
	Local_u8BirdSpeed=FixedPoint_ConvertFixedPointToFloatMultipleInTen(Local_u8BirdSpeed);
	Local_u8IncreaseValue+=10;
	return Local_u8BirdSpeed;
}

