//	pongEvents.c	04/16/2014
//******************************************************************************
//******************************************************************************
//	ALL THESE FUNCTIONS REQUIRE REWORK!
//******************************************************************************
//******************************************************************************
//
#include "msp430x22x4.h"
#include <stdlib.h>
#include <stdio.h>
#include "RBX430-1.h"
#include "RBX430_lcd.h"
#include "pong.h"

extern volatile int16 dx, dy;
extern volatile uint16 TB0_tone_on;
extern volatile uint16 sys_event;		// pending events

extern volatile uint16 seconds;			// seconds counter
extern volatile uint16 WDT_adc_cnt;
extern volatile uint16 WDT_lcd_cnt;
extern volatile uint16 ball_speed;

BALL* ball;								// game ball object
PADDLE* rightPaddle;					// right paddle object
PADDLE* leftPaddle;
volatile int16 dx, dy;					// ball delta change
volatile enum MODE game_mode;			// game mode
int count,P1Score,P2Score,winner;

extern const uint16 pong_image[];		// 2 paddle image

BALL* myBall;							// **replace with malloc'd struct**
PADDLE* myPaddle;						// **replace with malloc'd struct**

//------------------------------------------------------------------------------
//	ball factory (fix by malloc'ing ball struct)
//
BALL* new_ball(int x, int y)
{
	// initialize ball values
	myBall = (BALL*)malloc(sizeof(BALL));
	myBall->x = x;						// set horizontal position
	myBall->y = y;						// set vertical position
	myBall->old_x = myBall->x;			// set old values
	myBall->old_y = myBall->y;
	return myBall;
} // end init_ball


//------------------------------------------------------------------------------
//	paddle factory (fix by malloc'ing paddle struct)
//
PADDLE* new_paddle(int channel, int x)
{
	myPaddle = (PADDLE*)malloc(sizeof(PADDLE));
	myPaddle->channel = channel;			// ADC input channel
	myPaddle->x = x;						// horizontal paddle position
	myPaddle->potValue = -1;				// default all other values
	myPaddle->y = -1;
	myPaddle->old_y = -1;
	return myPaddle;					// return pointer to paddle
} // end init_ball


//------------------------------------------------------------------------------
//	new game event (fix)
//
void NEW_GAME_event()
{
	lcd_volume(345);						// **increase as necessary**
	lcd_backlight(ON);
	if(game_mode != EOG){					// turn on LCD
		lcd_clear();							// clear LCD
		lcd_wordImage(pong_image, 25, 35, 1);
		lcd_mode(LCD_2X_FONT);					// turn 2x font on
		lcd_cursor(20, 30);						// set display coordinates
		printf("P O N G");
		lcd_mode(~LCD_2X_FONT);					// turn 2x font off
		lcd_cursor(30, 5);
		lcd_printf("PRESS ANY SWITCH");

		//sys_event |= SWITCH_1;
	}
	game_mode = IDLE;						// idle mode

	return;
} // end NEW_GAME_event


//------------------------------------------------------------------------------
//	ADC event - paddle potentiometer and draw paddle (fix)
//
void ADC_READ_event(PADDLE* paddle)
{
	int pot;
	if(paddle->channel == 6){
		pot = 1023-ADC_read(paddle->channel);	// sample potentiometer
	} else {
		pot = ADC_read(paddle->channel);
	}

	// check for paddle position change
	if ((abs(pot - paddle->potValue) > POT_THRESHHOLD))
	{
		int offset = 1023 / 146;
		paddle->potValue = pot;			// save old value
		paddle->y = (pot /offset)+7;			// update paddle position (fix)

		drawPaddle(paddle);				// draw paddle
	}
	return;
} // end ADC_READ_event


//------------------------------------------------------------------------------
//	TimerA event - move ball (fix)
//
void MOVE_BALL_event(BALL* ball)
{
	int hit = 1;
	ball->x += dx;						// increment x coordinate
	ball->y += dy;						// update y coordinate
	if ((ball->y >= HD_Y_MAX-5) || (ball->y < 5))
	{
		dy = -dy;
	}
	drawBall(ball);						// ok, draw ball in new position

	//left paddle
	if((ball->x <=3)){
		//top
		if((ball->y < leftPaddle->y+8) && (ball->y > leftPaddle->y+4)){
			dy += dy > 0 ? 2 : -2;
		}//top middle
		else if((ball->y < leftPaddle->y+5) && (ball->y > leftPaddle->y)){
			dy += dy > 0 ? 1 : -1;
		}//bottom
		else if((ball->y > leftPaddle->y-8) && (ball->y < leftPaddle->y-4)){
			dy += dy > 0 ? 2 : -2;
		}//bottom middle
		else if((ball->y > leftPaddle->y-5) && (ball->y < leftPaddle->y)){
			dy += dy > 0 ? 1 : -1;
		} else if(ball->y == leftPaddle->y){
			dy = 0;
		}
		else {
			sys_event |= MISSED_BALL;
			P1Score++;
			winner = 1;
			hit = 0;
			lcd_rectangle(ball->x-2,ball->y-2,5,5,16);
		}
	}
	//right paddle
	else if((ball->x >=HD_X_MAX-5)){
		//top
		if((ball->y < rightPaddle->y+8) && (ball->y > rightPaddle->y+4)){
			dy += dy > 0 ? 2 : -2;
		}//top middle
		else if((ball->y < rightPaddle->y+5) && (ball->y > rightPaddle->y)){
			dy += dy > 0 ? 1 : -1;
		}//bottom
		else if((ball->y > rightPaddle->y-8) && (ball->y < rightPaddle->y-4)){
			dy += dy > 0 ? 2 : -2;
		}//bottom middle
		else if((ball->y > rightPaddle->y-5) && (ball->y < rightPaddle->y)){
			dy += dy > 0 ? 1 : -1;
		} else if(ball->y == rightPaddle->y){
			dy = 0;
		}
		else {
			sys_event |= MISSED_BALL;
			P2Score++;
			winner = 2;
			hit = 0;
			lcd_rectangle(ball->x-2,ball->y-2,5,5,16);
		}
	}
	else {
		hit = 0;
	}
	if (hit)
	{
		dx = -dx;						// reverse ball direction
		BEEP(BEEP_COUNT, 20);
		TACCR0 -= 5;
	}
	return;
} // end MOVE_BALL_event

void SWITCH_1_event(){
	if(game_mode == IDLE){
		//game = PLAYER2;
		count = 4;
		game_mode = COUNT;
	} else {
		sys_event |= NEW_GAME;
	}

return;
}

void START_GAME_event(){

//	//free ball/paddles
//	if(ball != NULL){
//		free(ball);
//		ball = NULL;
//	}
//	if(leftPaddle != NULL){
//		free(leftPaddle);
//		leftPaddle = NULL;
//	}
//	if(rightPaddle != NULL){
//		free(rightPaddle);
//		rightPaddle = NULL;
//	}

	//initialize all variables
	P1Score = P2Score = 0;
	winner = rand() % 2;

	drawGame();


	//************************************************************
	//	START_GAME EVENT (move to own event handler
	//************************************************************

		// initialize variables...

		// manufacture and draw paddle (malloc!!)
		rightPaddle = new_paddle(RIGHT_POT, 157);
		leftPaddle = new_paddle(LEFT_POT, 1);
		WDT_adc_cnt = 1;					// start sampling the paddles
		sys_event |= NEW_RALLY;
		return;

}

void drawGame(){
	lcd_rectangle(0,158,160,2,1);
	lcd_rectangle(79,2,2,156,1);
	lcd_rectangle(0,0,160,2,1);
}

void drawScores(){
	if(game_mode == EOG){
		if(P2Score == 4) P2Score++;
		if(P2Score == 4) P1Score++;
	}
	lcd_mode(LCD_2X_FONT);
	lcd_cursor(40,140);
	printf("%d",P2Score);
	lcd_cursor(120,140);
	printf("%d",P1Score);
	lcd_mode(~LCD_2X_FONT);
}

void LCD_UPDATE_event(){
	if(game_mode == PLAY){
		//Update scores
		drawScores();
		//redraw center lines
		drawGame();
		//redraw paddles;
		drawPaddle(leftPaddle);
		drawPaddle(rightPaddle);
	} else if (game_mode == COUNT && count){
		count--;
		lcd_clear();
		lcd_mode(LCD_2X_FONT);
		lcd_cursor(70,70);
		if(count > 0){
			printf("%d",count);
		}else {
			printf("GO");
		}
		lcd_mode(~LCD_2X_FONT);
		//display (GO,1,2,3) [count]
		if(count == 0 ){
			lcd_clear();
			game_mode=IDLE;
			sys_event |= START_GAME;
		}
	}
	return;
}

void NEW_RALLY_event(){

	//************************************************************
	//	NEW_RALLY EVENT (move to own event handler
	//************************************************************

		// manufacture new ball (NEW_RALLY)
		ball = new_ball(80, 80);

		// serve ball
		dx = winner == 1 ? -1 : 1;			// delta x
		dy = rand() % 2 ? -1 : 1;			// delta y
		ball_speed = BALL_SPEED;			// interrupt rate
		TACCR0 = ball_speed;				// start timer
		WDT_lcd_cnt = 1;					// start LCD update (ONLY WHEN DEFINED)
		game_mode = PLAY;					// enter play mode
	return;
}
void END_GAME_event(){
	//mode = EOG
	game_mode = EOG;
	//free paddles
	free(leftPaddle);
	leftPaddle = NULL;
	free(rightPaddle);
	rightPaddle = NULL;
	//ouput final scores and display appropriate award
	lcd_clear();
	drawScores();
	char player[] = "Player ";
	strcat(player, P1Score>P2Score?"1":"2");
	lcd_mode(LCD_2X_FONT);
	lcd_cursor(5,60);
	printf("%s WINS",player);
	lcd_mode(~LCD_2X_FONT);
	return;
}
void MISSED_BALL_event(){
	//raspberry
	BEEP(BEEP_COUNT + 8000, 40);
	//mode = idle
	game_mode = IDLE;
	//update scores
	//free ball
	free(ball);
	//stop timer a
	TACCR0 = 0;
	//if score < 5 new rally else end game
	if(P1Score < 5 &&  P2Score < 5){
		sys_event |= NEW_RALLY;
	} else {
		sys_event |= END_GAME;
	}
	return;
}
void SWITCH_2_event(){
	return;
}
void SWITCH_4_event(){
	return;
}

