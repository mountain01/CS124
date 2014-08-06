//	life.c	03/25/2014
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

uint16 pen = 1;							// ** delete **

//------------------------------------------------------------------------------
//	draw RLE pattern -----------------------------------------------------------
void draw_rle_pattern(int row, int col, const uint8* object)
{
	life[75][4] = 0x07;					// ** delete **
	lcd_point(37 << 1, 75 << 1, 7);		// ** delete **
	lcd_point(38 << 1, 75 << 1, 7);		// ** delete **
	lcd_point(39 << 1, 75 << 1, 7);		// ** delete **
	return;
} // end draw_rle_pattern


//------------------------------------------------------------------------------
// main ------------------------------------------------------------------------
void main(void)
{
	RBX430_init(_1MHZ);					// init board
	ERROR2(lcd_init());					// init LCD
	//lcd_volume(376);					// increase LCD brightness
	watchdog_init();					// init watchdog
	port1_init();						// init P1.0-3 switches
	__bis_SR_register(GIE);				// enable interrupts

	while (1)							// new pattern seed
	{
		uint16 generation;				// generation counter
		uint16 row, col;

		// setup beginning life generation
		init_life(BIRD);				// load a new life seed into LCD

			// >>>>>>>> DELETE vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
			lcd_wordImage(life_image, 17, 50, 1);
			lcd_cursor(10, 20);
			printf("\b\tPress Any Key");
			switches = 0;						// clear switches flag
			while (!switches);					// wait for any switch
			// >>>>>>>> DELETE ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

		WDT_Sec_Cnt = WDT_1SEC_CNT;		// reset WD 1 second counter
		seconds = 0;					// clear second counter
		switches = 0;					// clear switch variable
		generation = 0;					// start generation counter

		while (1)						// next generation
		{
			memcpy(life_pr,life[NUM_ROWS-2], sizeof(uint8));
			memcpy(life_cr,life[NUM_ROWS-3], sizeof(uint8));
			memcpy(life_nr,life[NUM_ROWS-4], sizeof(uint8));
			// for each life row (78 down to 1)
			for (row = NUM_ROWS-2; row; row -= 1)
			{
				// for each life column (78 down to 1)
				for (col = NUM_COLS-2; col; col -= 1)
				{
					//cell calculations
					int cellCount = 0;
					int alive = TEST(life_cr,col);
					if(TEST(life_pr,col-1)) cellCount++;
					if(TEST(life_pr,col)) cellCount++;
					if(TEST(life_pr,col+1)) cellCount++;
					if(TEST(life_nr,col-1)) cellCount++;
					if(TEST(life_nr,col)) cellCount++;
					if(TEST(life_nr,col+1)) cellCount++;
					if(TEST(life_cr,col-1)) cellCount++;
					if(TEST(life_cr,col+1)) cellCount++;
					if(alive && (cellCount < 2 || cellCount > 3)){
						KILL(life[row],col);
					}
					else if(!alive && (cellCount == 3)){
						REVIVE(life[row],col);
					}

				}
					//copy info to small arrays
				memcpy(life_pr,life_cr,sizeof(uint8));
				memcpy(life_cr,life_nr,sizeof(uint8));
				memcpy(life_nr,life[row],sizeof(uint8));
			}

			// display life generation and generations/second on LCD
			if (display_results(++generation)) break;
		}
	}
} // end main()
