/*
 * EmbeddedAdvancedTasks.c
 *
 * Created: 11/14/2024 4:32:56 PM
 * Author : Demiana
 */ 

#define F_CPU 8000000
#include <util/delay.h>
#include "StdTypes.h"

#include "FlappyBird_Interface.h"

int main()
{
	FlappyBird_voidInit();
	while(1)
	{
		FlappyBird_voidRunnable();
	}
}
