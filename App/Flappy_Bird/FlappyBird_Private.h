/*
 * FlappyBird_Private.h
 *
 * Created: 11/8/2024 4:34:46 AM
 *  Author: Demiana Younes
 */ 


#ifndef FLAPPYBIRD_PRIVATE_H_
#define FLAPPYBIRD_PRIVATE_H_

static void CallBack_INT0(void);
static void CallBack_INT1(void);
static void MovementBird(u8 Copy_u8Iterator,u8 Local_u8BirdSpeed);
static void FlappyBird_voidPressButtonWaiting(void);
static void FlappyBird_voidStartGame(void);
static u8 FlappyBird_voidGameOver(void);
static u8 FlappyBird_CheckObstacles2(u8 Copy_u8Start,u8 Copy_u8End ,u8 Copy_u8Iterator,u8 Copy_u8Part);
static void FlappyBird_ResetObstacles(void);
static void FlappyBird_ClearObstacles(u8 Copy_u8Offest);
static void FlappyBird_DrawObstacles(u8 Copy_u8Offest);
static u8 FlappyBird_CheckObstacles(u8 Copy_u8Offest, u8 Copy_u8birdX);
static u8 FlappyBird_voidBirdSpeed(void);

#endif /* FLAPPYBIRD_PRIVATE_H_ */