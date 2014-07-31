/*
 * morse2.h
 *
 *  Created on: Jul 29, 2014
 *      Author: Matt
 */

#ifndef MORSE2_H_
#define MORSE2_H_


#define myCLOCK    1200000                 // 1.2 Mhz clock
#define WDT_CTL    WDT_MDLY_0_5            // WD: Timer, SMCLK, 0.5 ms
#define WDT_CPI    500                     // WDT Clocks Per Interrupt (@1 Mhz)
#define WDT_IPS    myCLOCK/WDT_CPI         // WDT Interrupts Per Second
#define STACK      0x0600                  // top of stack
#define ELEMENT    WDT_IPS*240/1000        // (WDT_IPS * 6 / WPM) / 5


#endif /* MORSE2_H_ */
