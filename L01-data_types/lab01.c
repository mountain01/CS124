//	Lab01.c	2014/06/16
#include <msp430.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "lab01.h"

int main(void)
{
    signed char biggest_signedChar, next_signedChar, smallest_signedChar;
    signed int biggest_signedInt, next_signedInt, smallest_signedInt;
    signed long biggest_signedLong, next_signedLong, smallest_signedLong;
    unsigned char biggest_unsignedChar, next_unsignedChar;
    unsigned int biggest_unsignedInt, next_unsignedInt;
    unsigned long biggest_unsignedLong, next_unsignedLong;
	lab01_init();

	TERMINAL("\n\r** INTEGERS *************");
	TERMINAL("\n\r*** Largest/Smallest ****");
  	//TERMINAL1("Signed int: 21000 + 200 = %d", 21000 + 200);

  	next_signedChar = 1;						// start w/1
    biggest_signedChar = 1;
    while((next_signedChar <<= 1) > 0)
    {
    	biggest_signedChar = (biggest_signedChar << 1) + 1;;
    }
    smallest_signedChar = 1;
    smallest_signedChar >>=2;
  	next_unsignedChar = 1;						// start w/1
    biggest_unsignedChar = 1;
    while((next_unsignedChar <<= 1) !=    0)
    {
    	biggest_unsignedChar = (biggest_unsignedChar << 1) + 1;;
    }
    next_signedInt = 1;
    biggest_signedInt = 1;
    while((next_signedInt <<=1) > 0){
    	biggest_signedInt = (biggest_signedInt << 1) + 1;
    }
    next_unsignedInt = 1;
    biggest_unsignedInt = 1;
    while((next_unsignedInt <<=1) != 0){
    	biggest_unsignedInt = (biggest_unsignedInt << 1) + 1;
    }
    next_signedLong = 1;
    biggest_signedLong = 1;
    while((next_signedLong <<=1) > 0){
    	biggest_signedLong = (biggest_signedLong << 1) + 1;
    }
    next_unsignedLong = 1;
    biggest_unsignedLong = 1;
    while((next_unsignedLong <<=1) != 0){
    	biggest_unsignedLong = (biggest_unsignedLong << 1) + 1;
    }

    //Floating point comparison
    float f1 = .01;
    float sum1 = 0.0;
    int i;
    for (i = 0; i<100;i++){
    	sum1 = sum1 + f1;
    }
    float f2 = .0001;
    float sum2 = 0.0;
    for(i=0;i<10000;i++){
    	sum2 += f2;
    }

    //Order of operations
    float sum3 = .00000001 + (1-1);
    float sum4 = (.00000001 + 1) - 1;
    float sum5 = .00004 + (3 - 3);
    float sum6 = (.00004 + 3) - 3;

    //output final answers
	TERMINAL3("Signed char: Largest: %d Smallest: %d", biggest_signedChar, next_signedChar);
	TERMINAL2("Largest unsigned char = %u (0x%2x)", biggest_unsignedChar);
	TERMINAL3("Signed int: Largest: %d Smallest: %d", biggest_signedInt, next_signedInt);
	TERMINAL2("Largest unsigned int = %u (0x%2x)", biggest_unsignedInt);
	TERMINAL3("Signed long: Largest: %ld Smallest: %ld", biggest_signedLong, next_signedLong);
	TERMINAL2("Largest unsigned long = %lu (0x%2x)", biggest_unsignedLong);
	TERMINAL("\n\r******* Fixed Point *****");
	TERMINAL3("Largest Q16.16 = %f (0x%2x)", (((long)biggest_signedLong)/65536.0), biggest_signedLong);
	TERMINAL3("Smallest Q16.16 = %f (0x%2x)", (((long)next_signedLong)/65536.0), next_signedLong);
	TERMINAL("\n\r******* Overflow ********");
	TERMINAL2("Char overflow: %d 0x%2x",((biggest_signedChar <<=1) + 2));
	TERMINAL2("Int overflow: %d 0x%2x",((biggest_signedInt <<=1) + 2));
	TERMINAL2("Long overflow: %ld 0x%2x",((biggest_signedLong <<=1) + 2));
	TERMINAL("\n\r**** FLoat Comparison ***");
	TERMINAL3("Example 1: \n\r\t\Sum of 100 pennies (%f) vs .01 * 100 (%f)", sum1, (f1*100));
	TERMINAL3("Example 2: \n\r\t\Sum of 1000 .0001 (%f) vs .0001 * 10000 (%f)", sum2, (f2*10000));
	TERMINAL("\n\r***Order of Operations***");
	TERMINAL("\n\rExample 1: .00000001 + 1 - 1");
	TERMINAL2("\tSUM1 = .00000001 + (1 - 1) == %f (0x%041x)", sum3);
	TERMINAL2("\tSUM2 = (.00000001 + 1) - 1 == %f (0x%041x)", sum4);
	TERMINAL("\n\rExample 2: .00004 - 3 + 3");
	TERMINAL2("\tSUM3 = .00004 + (3 - 3) == %f (0x%041x)", sum5);
	TERMINAL2("\tSUM4 = (.00004 + 3) - 3 == %f (0x%041x)", sum6);
    return 0;
}
