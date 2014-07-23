;*******************************************************************************
;   CS/ECEn 124 Lab 3 - blinky.asm: Software Toggle P1.0
;
;   Description: Quickly blink P1.0 every 10 seconds.  Calculate MCLK, CPI, MIPS
;        Author: Matt Edwards
;
;             MSP430G5223
;             -----------
;            |       P1.0|-->LED1-RED LED
;            |       P1.3|<--S2
;            |       P1.6|-->LED2-GREEN LED
;
; Show all calculations:
;		start: 11 cycles
;			3 instructions
;		mainloop: 6 cycles
;			2 instructions
;		callDelay: 10 cycles x 100 + 2 cycles = 1002 cycles
;			4 instructions x 100 + 1 instruction = 401 instructions
;		delayloop: 3 cycles x 0x8980 = 211200 cycles x 100 + 300 cycles= 10560300 cycles
;			2 instructions x 0x8980 = 70400 instrucions x 100 + 100 instructions = 7040100 instructions
;		total: 11 + 6 + 1002 + 10560300 =  10561315 cycles
;			3 + 2 + 401 + 7040100 = 7040505 instructions
;
;
;   MCLK = 10561315 cycles / 10 second interval = 1056131.5 Mhz
;    CPI = 10561315 cycles/ 7040505 instructions = 1.5001 Cycles/Instruction
;   MIPS = MCLK / CPI / 1000000 =  .70405 MIPS

;
;*******************************************************************************
           .cdecls  C,"msp430.h"            ; MSP430

COUNT      .equ     0x8980                       ; delay count
LOOPCOUNT   .equ    100                     ; call delay subroutine this many times
;------------------------------------------------------------------------------
            .text                           ; beginning of executable code
;------------------------------------------------------------------------------
start:      mov.w   #0x0280,SP              ;    init stack pointer 2 cycles
            mov.w   #WDTPW|WDTHOLD,&WDTCTL  ;    stop WDT 5 cycles
            bis.b   #0x01,&P1DIR            ;    set P1.0 as output  4 cycles

mainloop:   xor.b   #0x01,&P1OUT            ;    toggle P1.0 4 cycles
            mov.w   #LOOPCOUNT,r14			; 		2 cycles

callDelay:  mov.w   #COUNT,r15              ;    use R15 as delay counter 2 cycles
            call #delayloop					;	5 cycles
            dec.w   r14						;	1 cycles
                jz mainloop					;	2 cycles
            jmp callDelay					;	2 cycles


;1/10 second loop subroutine

delayloop:  sub.w   #1,r15                  ;    delay over? 1 cycles
              jne   delayloop               ;    n 2 cycles
            ret                             ;    y, toggle led 3 cycles

;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .word   start                   ; start address
            .end
