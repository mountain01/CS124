//	life.c	03/25/2014

//	Name: Matt Edwards
//******************************************************************************
//  The Game of Life
//
//  Lab Description:
//
//  The universe of the Game of Life is an infinite two-dimensional orthogonal
//  grid of square cells, each of which is in one of two states, alive or dead.
//  With each new generation, every cell interacts with its eight neighbors,
//  which are the cells horizontally, vertically, or diagonally adjacent
//  according to the following rules:
//
//  1. A live cell stays alive (survives) if it has 2 or 3 live neighbors,
//     otherwise it dies.
//  2. A dead cell comes to life (birth) if it has exactly 3 live neighbors,
//     otherwise it stays dead.
//
//  An initial set of patterns constitutes the seed of the simulation. Each
//  successive generation is created by applying the above rules simultaneously
//  to every cell in the current generation (ie. births and deaths occur
//  simultaneously.)  See http://en.wikipedia.org/wiki/Conway's_Game_of_Life
//
//  Author:    Paul Roper, Brigham Young University
//  Revisions: June 2013   Original code
//             07/12/2013  life_pr, life_cr, life_nr added
//             07/23/2013  generations/seconds added
//             07/29/2013  100 second club check
//             12/12/2013  SWITCHES, display_results, init for port1 & WD
//	           03/24/2014  init_life moved to lifelib.c, 0x80 shift mask
//	                       blinker added, 2x loops
//
//  Built with Code Composer Studio Version: 5.5.0.00090
//******************************************************************************
//  Lab hints:
//
//  The life grid (uint8 life[80][10]) is an 80 row x 80 column bit array.  A 0
//  bit is a dead cell while a 1 bit is a live cell.  The outer cells are always
//  dead.  A boolean cell value (0 or non-zero) is referenced by:
//
//         life[row][col >> 3] & (0x80 >> (col & 0x07))
//
//  Each life cell maps to a 2x2 lcd pixel.
//
//                     00       01             08       09
//  life[79][0-9]   00000000 00000000  ...  00000000 00000000 --> life_pr[0-9]
//  life[78][0-9]   0xxxxxxx xxxxxxxx  ...  xxxxxxxx xxxxxxx0 --> life_cr[0-9]
//  life[77][0-9]   0xxxxxxx xxxxxxxx  ...  xxxxxxxx xxxxxxx0 --> life_nr[0-9]
//  life[76][0-9]   0xxxxxxx xxxxxxxx  ...  xxxxxxxx xxxxxxx0         |
//     ...                                                            |
//  life[75-4][0-9]   ...      ...            ...      ...            v
//     ...
//  life[03][0-9]   0xxxxxxx xxxxxxxx  ...  xxxxxxxx xxxxxxx0
//  life[02][0-9]   0xxxxxxx xxxxxxxx  ...  xxxxxxxx xxxxxxx0
//  life[01][0-9]   0xxxxxxx xxxxxxxx  ...  xxxxxxxx xxxxxxx0
//  life[00][0-9]   00000000 00000000  ...  00000000 00000000
//
//  The next generation can be made directly in the life array if the previous
//  cell values are held in the life_pr (previous row), life_cr (current row),
//  and life_nr (next row) arrays and used to count cell neighbors.
//
//  Begin each new row by moving life_cr values to life_pr, life_nr values to
//  life_cr, and loading life_nr with the row-1 life values.  Then for each
//  column, use these saved values in life_pr, life_cr, and life_nr to
//  calculate the number of cell neighbors of the current row and make changes
//  directly in the life array.
//
//  life_pr[0-9] = life_cr[0-9]
//  life_cr[0-9] = life_nr[0-9]
//  life_nr[0-9] = life[row-1][0-9]
//
//******************************************************************************
//******************************************************************************
// includes --------------------------------------------------------------------
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "msp430.h"
#include "RBX430-1.h"
#include "RBX430_lcd.h"
#include "life.h"

// global variables ------------------------------------------------------------
extern volatile uint16 WDT_Sec_Cnt;		// WDT second counter
extern volatile uint16 seconds;			// # of seconds
extern volatile uint16 switches;		// debounced switch values

uint8 life[NUM_ROWS][NUM_COLS/8];		// 80 x 80 life grid
uint8 life_pr[NUM_COLS/8];				// previous row
uint8 life_cr[NUM_COLS/8];				// current row
uint8 life_nr[NUM_COLS/8];				// next row
uint8* myPR;
uint8* myCR;
uint8* myNR;

uint16 pen = 1;							// ** delete **

//------------------------------------------------------------------------------
//	draw RLE pattern -----------------------------------------------------------
void draw_rle_pattern(int row, int col, const uint8* object)
{
	int y = 0;
	int loopCount = 0;
	while(*object && (*object++ != 'y'));
	while(*object && !isdigit(*object)){
		++object;
	}
	while(isdigit(*object)){
		y = y * 10 + (*object++ - '0');
	}

	int myRow = row + y -1;
	int myCol = col;
	while(*object && (*object++ != '\n'));
	while(*object){
		if(isdigit(*object)){
			loopCount = 0;
			while(isdigit(*object)){
				loopCount = loopCount * 10 + (*object++ - '0');
			}
		}
		else{
			loopCount = 1;
		}
		if(*object == 'b'){
			while(loopCount){
				myCol++;
				loopCount--;
			}
		}
		if(*object == 'o'){
			while(loopCount){
				myCol++;
				lcd_point(myCol<<1,myRow<<1,7);
				life[myRow][myCol/8] |= (0x80 >> (myCol & 0x07));
				loopCount--;
			}
		}
		if(*object == '$'){
			while(loopCount){
				myRow--;
				myCol = col;
				loopCount--;
			}
		}

		if(*object == '!'){
			return;
		}

		object++;
	}
} // end draw_rle_pattern


//------------------------------------------------------------------------------
// main ------------------------------------------------------------------------
void main(void)
{
	RBX430_init(_16MHZ);					// init board
	ERROR2(lcd_init());					// init LCD
	//lcd_volume(376);					// increase LCD brightness
	watchdog_init();					// init watchdog
	port1_init();						// init P1.0-3 switches
	__bis_SR_register(GIE);				// enable interrupts
	P4DIR |= 0x40;

	while (1)							// new pattern seed
	{
		uint16 generation;				// generation counter
		uint16 row, col;

		// setup beginning life generation
			if(switches == 1){
				init_life(LIFE);
			}
			else if(switches == 4){
				init_life(BOMB);
			}
			else if(switches == 8){
				init_life(YOURS);
			}
			else{
				init_life(BIRD);
			}

		WDT_Sec_Cnt = WDT_1SEC_CNT;		// reset WD 1 second counter
		seconds = 0;					// clear second counter
		switches = 0;					// clear switch variable
		generation = 0;					// start generation counter

		while (1)						// next generation
		{
			P4OUT |= 0x40;
			memcpy(life_pr,life[NUM_ROWS-1],sizeof(life[NUM_ROWS-1]));
			memcpy(life_cr,life[NUM_ROWS-2],sizeof(life[NUM_ROWS-1]));
			memcpy(life_nr,life[NUM_ROWS-3],sizeof(life[NUM_ROWS-1]));
			myNR = life_nr;
			myCR = life_cr;
			myPR = life_pr;
			// for each life row (78 down to 1)
			for (row = NUM_ROWS-2; row; row -= 1)
			{
				// for each life column (78 down to 1)
				for (col = NUM_COLS-2; col; col -= 1)
				{
					int curIndex, leftIndex, rightIndex;
					curIndex = leftIndex = rightIndex = col/8;
					int cur = 0x80 >> ((col) % 8);
					int left = 0x80 >> ((col-1) % 8);
					int right = 0x80 >> ((col+1) % 8);
					if(left == 1){
						leftIndex--;
					}
					if(cur == 1){
						rightIndex++;
					}
					//cell calculations
					int cellCount = 0;
					int alive = myCR[curIndex] & cur;
					if(myPR[leftIndex] & left) cellCount++;
					if(myPR[curIndex] & cur) cellCount++;
					if(myPR[rightIndex] & right) cellCount++;
					if(myNR[leftIndex] & left) cellCount++;
					if(myNR[curIndex] & cur) cellCount++;
					if(myNR[rightIndex] & right) cellCount++;
					if(myCR[leftIndex] & left) cellCount++;
					if(myCR[rightIndex] & right) cellCount++;
					if(alive && (cellCount < 2 || cellCount > 3)){
						life[row][curIndex] &= ~cur;
						lcd_point((col)<<1,(row)<<1,6);
					}
					else if(!alive && (cellCount == 3)){
						life[row][curIndex] |= cur;
						lcd_point((col)<<1,(row)<<1,7);
					}

				}
					//copy info to small arrays
				uint8* temp = myPR;
				myPR = myCR;
				myCR = myNR;
				myNR = temp;
				memcpy(myNR,life[row-2],sizeof(life[row]));
				P4OUT &= ~0x40;
			}

			// display life generation and generations/second on LCD
			if (display_results(++generation)) break;
		}
	}
} // end main()
