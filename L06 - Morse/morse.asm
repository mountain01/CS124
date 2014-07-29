			.title	"morse.asm"
;*******************************************************************************
;     Project:  morse.asm
;      Author:  Matt Edwards
;
; Description:  Outputs a message in Morse Code using a LED and a transducer
;               (speaker).  The watchdog is configured as an interval timer.
;               The watchdog interrupt service routine (ISR) toggles the green
;               LED every second and pulse width modulates (PWM) the speaker
;               such that a tone is produced.
;
;	Morse code is composed of dashes and dots:
;
;        1. A dot is equal to an element of time.
;        2. One dash is equal to three dots.
;        3. The space between parts of the letter is equal to one dot.
;        4. The space between two letters is equal to three dots.
;        5. The space between two words is equal to seven dots.
;
;    5 WPM = 60 sec / (5 * 50) elements = 240 milliseconds per element.
;    element = (WDT_IPS * 6 / WPM) / 5
;
;******************************************************************************
; System equates --------------------------------------------------------------
            .cdecls C,"msp430.h"            ; include c header
myCLOCK     .equ    1200000                 ; 1.2 Mhz clock
WDT_CTL     .equ    WDT_MDLY_0_5            ; WD: Timer, SMCLK, 0.5 ms
WDT_CPI     .equ    500                     ; WDT Clocks Per Interrupt (@1 Mhz)
WDT_IPS     .equ    myCLOCK/WDT_CPI         ; WDT Interrupts Per Second
STACK       .equ    0x0600                  ; top of stack

; External references ---------------------------------------------------------
            .ref    numbers                 ; codes for 0-9
            .ref    letters                 ; codes for A-Z
            .ref    DOT,DASH,END

; Morse Code equates ----------------------------------------------------------
ELEMENT     .equ    WDT_IPS*240/1000        ; (WDT_IPS * 6 / WPM) / 5

; Global variables ------------------------------------------------------------
            .bss    beep_cnt,2              ; beeper flag
            .bss    delay_cnt,2             ; delay flag
           .bss    debounce_cnt,2        ; debounce count

; Program section -------------------------------------------------------------
            .text                           ; program section
message:    .string "HELLO CS 124 WORLD"                 ; PARIS message
            .byte   0
            .align  2                       ; align on word boundary

; power-up reset --------------------------------------------------------------
RESET:      mov.w   #STACK,SP               ; initialize stack pointer
            call    #main_asm               ; call main function
            jmp     $                       ; you should never get here!


; start main function vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
main_asm:   mov.w   #WDT_CTL,&WDTCTL        ; set WD timer interval
            mov.b   #WDTIE,&IE1             ; enable WDT interrupt
            bis.b   #0x6F,&P4DIR            ; set P4.5 as output (speaker)
            bic.b	#0x6f,&P4OUT
            clr.w   &beep_cnt               ; clear counters
            clr.w   &delay_cnt
            clr.w   &debounce_cnt
            bis.w   #GIE,SR                 ; enable interrupts
           bic.b   #0x0f,&P1SEL          ; RBX430-1 push buttons
           bic.b   #0x0f,&P1DIR          ; Configure P1.0-3 as Inputs
           bis.b   #0x0f,&P1OUT          ; pull-ups
           bis.b   #0x0f,&P1IES          ; h to l
           bis.b   #0x0f,&P1REN          ; enable pull-ups
           bis.b   #0x0f,&P1IE           ; enable switch interrupts

; output 'A' in morse code (DOT, DASH, space)
loop:
			mov.w	#message,r4

getLetter:	mov.b	@r4+,r5
			cmp.b	#END,r5
			  jeq	loop
			cmp.b	#' ',r5
			  jeq	space
			cmp.b	#':',r5
			  jge	isLetter
			sub.b	#'0',r5
			add.b	r5,r5
			mov.w	numbers(r5),r5
			jmp		getSequence

isLetter:	sub.b 	#'A',r5
			add.b	r5, r5
			mov.w	letters(r5),r5

getSequence:	mov.b	@r5+,r6
				cmp.b	#END,r6
				  jeq	charSpace
				cmp.b	#DOT,r6
				  jne	doDash


			mov.w   #ELEMENT,r15            ; output DOT
            call    #beep
            mov.w   #ELEMENT,r15            ; delay 1 element
            call    #delay
            jmp 	done

doDash:		mov.w   #ELEMENT*3,r15          ; output DASH
            call    #beep
            mov.w   #ELEMENT,r15            ; delay 1 element
            call    #delay

done:		jmp     getSequence                    ; repeat

charSpace:	mov.w	#ELEMENT*3,r15
			call	#delay
			jmp		getLetter

space:      mov.w   #ELEMENT*7,r15          ; output space
            call    #delay                  ; delay
            jmp		getLetter
; end main function ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


; beep (r15) ticks subroutine -------------------------------------------------
beep:       mov.w   r15,&beep_cnt           ; start beep
			bis.b	#0x40,&P4OUT

beep02:     tst.w   &beep_cnt               ; beep finished?
              jne   beep02                  ; n
            bic.b	#0x40,&P4OUT
            ret                             ; y


; delay (r15) ticks subroutine ------------------------------------------------
delay:      mov.w   r15,&delay_cnt          ; start delay

delay02:    tst.w   &delay_cnt              ; delay done?
              jne   delay02                 ; n
            ret                             ; y
; Port 1 ISR -------------------------------------------------------------------
DEBOUNCE   .equ    10
P1_ISR:    bic.b   #0x0f,&P1IFG          ; acknowledge (put hands down)
           mov.w   #DEBOUNCE,debounce_cnt ; reset debounce count
           reti

; Watchdog Timer ISR ----------------------------------------------------------
WDT_ISR:
			tst.w   &debounce_cnt          ; debouncing?
             jeq   WDT_01                ; n

; debounce switches & process

           dec.w   &debounce_cnt          ; y, decrement count, done?
             jne   WDT_01                ; n
           mov.b   &P1IN,r14             ; read switches
           and.b   #0x0f,r14
           xor.b   #0x0f,r14             ; any switches?
             jeq   WDT_01                ; n
           bit.b	#1,r14
             jeq	WDT_01
           xor.b	#0x20,&P4DIR



WDT_01:		tst.w   &beep_cnt               ; beep on?
              jeq   WDT_02                  ; n
            dec.w   &beep_cnt               ; y, decrement count
            xor.b   #0x20,&P4OUT            ; beep using 50% PWM

WDT_02:     tst.w   &delay_cnt              ; delay?
              jeq   WDT_10                  ; n
            dec.w   &delay_cnt              ; y, decrement count

WDT_10:     reti                            ; return from interrupt

; Interrupt Vectors -----------------------------------------------------------
            .sect   ".int10"                ; Watchdog Vector
            .word   WDT_ISR                 ; Watchdog ISR
            .sect	".int02"
            .word	P1_ISR

            .sect   ".reset"                ; PUC Vector
            .word   RESET                   ; RESET ISR
            .end
