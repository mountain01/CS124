#include <msp430.h> 
#include "morse2.h"


extern int beep_cnt;
extern int delay_cnt;
extern int debounce_cnt;

extern void beep(int);
extern void delay(int);
/*
 * main.c
 */

int main_asm(void);
int main(void) {
	
	WDTCTL = WDT_CTL;					//    main_asm:   mov.w   #WDT_CTL,&WDTCTL        ; set WD timer interval
	IE1 = WDTIE;						//                mov.b   #WDTIE,&IE1             ; enable WDT interrupt
	P4DIR |= 0x6f;						//                bis.b   #0x6F,&P4DIR            ; set P4.5 as output (speaker)
	P4OUT &= ~0x67;						//                bic.b	#0x67,&P4OUT
	P4OUT |= 8;							//                bis.b	#8,&P4OUT
	beep_cnt = 0;						//                clr.w   &beep_cnt               ; clear counters
	delay_cnt = 0;						//                clr.w   &delay_cnt
	debounce_cnt = 0;					//                clr.w   &debounce_cnt
	__enable_interrupt();				//                bis.w   #GIE,SR                 ; enable interrupts
	P1SEL &= ~0x0f;						//               bic.b   #0x0f,&P1SEL          ; RBX430-1 push buttons
	P1DIR &= ~0x0f;						//               bic.b   #0x0f,&P1DIR          ; Configure P1.0-3 as Inputs
	P1OUT |= 0x0f;						//               bis.b   #0x0f,&P1OUT          ; pull-ups
	P1IES |= 0x0f;						//               bis.b   #0x0f,&P1IES          ; h to l
	P1REN |= 0x0f						//               bis.b   #0x0f,&P1REN          ; enable pull-ups
	P1IE |= 0x0f;						//               bis.b   #0x0f,&P1IE           ; enable switch interrupts

										//	; output 'A' in morse code (DOT, DASH, space)
										//	loop:
										//				mov.w	#message,r4
										//
										//	getLetter:	mov.b	@r4+,r5
										//				cmp.b	#END,r5
										//				  jeq	loop
										//				cmp.b	#' ',r5
										//				  jeq	space
										//				cmp.b	#':',r5
										//				  jge	isLetter
										//				sub.b	#'0',r5
										//				add.b	r5,r5
										//				mov.w	numbers(r5),r5
										//				jmp		getSequence
										//
										//	isLetter:	sub.b 	#'A',r5
										//				add.b	r5, r5
										//				mov.w	letters(r5),r5
										//
										//	getSequence:	mov.b	@r5+,r6
										//					cmp.b	#END,r6
										//					  jeq	charSpace
										//					cmp.b	#DOT,r6
										//					  jne	doDash
										//
										//
										//				mov.w   #ELEMENT,r15            ; output DOT
	beep(ELEMENT);						//	            call    #beep
										//	            mov.w   #ELEMENT,r15            ; delay 1 element
	delay(ELEMENT):						//	            call    #delay
										//	            jmp 	done
										//
										//	doDash:		mov.w   #ELEMENT*2,r15          ; output DASH
										//	            call    #beep
										//	            mov.w   #ELEMENT,r15            ; delay 1 element
										//	            call    #delay
										//
										//	done:		jmp     getSequence                    ; repeat
										//
										//	charSpace:	mov.w	#ELEMENT*2,r15
										//				call	#delay
										//				jmp		getLetter
										//
										//	space:      mov.w   #ELEMENT*6,r15          ; output space
										//	            call    #delay                  ; delay
										//	            jmp		getLetter
	return 0;
}
