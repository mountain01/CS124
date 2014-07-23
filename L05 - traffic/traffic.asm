;*******************************************************************************
;   Lab 5b - traffic.asm
;
;   Description:  1. Turn the large green LED and small red LED on and
;                    delay 20 seconds while checking for orange LED.
;                    (If orange LED is on and 10 seconds has expired, immediately
;                    skip to next step.)
;                 2. Turn large green LED off and yellow LED on for 5 seconds.
;                 3. Turn yellow LED off and large red LED on.
;                 4. If orange LED is on, turn small red LED off and small green
;                    LED on.  After 5 seconds, toggle small green LED on and off
;                    for 6 seconds at 1 second intervals.  Finish by toggling
;                    small green LED on and off for 4 seconds at 1/5 second
;                    intervals.
;                    Else, turn large red LED on for 5 seconds.
;                 5. Repeat the stoplight cycle.
;
;   I certify this to be my source code and not obtained from any student, past
;   or current.
;
;*******************************************************************************
;                            MSP430F2274
;                  .-----------------------------.
;            SW1-->|P1.0^                    P2.0|<->LCD_DB0
;            SW2-->|P1.1^                    P2.1|<->LCD_DB1
;            SW3-->|P1.2^                    P2.2|<->LCD_DB2
;            SW4-->|P1.3^                    P2.3|<->LCD_DB3
;       ADXL_INT-->|P1.4                     P2.4|<->LCD_DB4
;        AUX INT-->|P1.5                     P2.5|<->LCD_DB5
;        SERVO_1<--|P1.6 (TA1)               P2.6|<->LCD_DB6
;        SERVO_2<--|P1.7 (TA2)               P2.7|<->LCD_DB7
;                  |                             |
;         LCD_A0<--|P3.0                     P4.0|-->LED_1 (Green)
;        i2c_SDA<->|P3.1 (UCB0SDA)     (TB1) P4.1|-->LED_2 (Orange) / SERVO_3
;        i2c_SCL<--|P3.2 (UCB0SCL)     (TB2) P4.2|-->LED_3 (Yellow) / SERVO_4
;         LCD_RW<--|P3.3                     P4.3|-->LED_4 (Red)
;   TX/LED_5 (G)<--|P3.4 (UCA0TXD)     (TB1) P4.4|-->LCD_BL
;             RX-->|P3.5 (UCA0RXD)     (TB2) P4.5|-->SPEAKER
;           RPOT-->|P3.6 (A6)          (A15) P4.6|-->LED 6 (R)
;           LPOT-->|P3.7 (A7)                P4.7|-->LCD_E
;                  '-----------------------------'
;
;*******************************************************************************
;*******************************************************************************
            .cdecls  C,LIST,"msp430.h"      ; MSP430

            .asg    "bis.b #0x08,&P4OUT",RED_ON
            .asg    "bic.b #0x08,&P4OUT",RED_OFF
            .asg    "xor.b #0x08,&P4OUT",RED_TOGGLE
            .asg    "bit.b #0x08,&P4OUT",RED_TEST

COUNT       .equ    0x98c7
DELAY_20	.equ	200
DELAY_5		.equ	50
DELAY_1		.equ	10
DELAY_10	.equ	100
DELAY_5th	.equ	2

;------------------------------------------------------------------------------
            .text                           ; beginning of executable code
            .retain                         ; Override ELF conditional linking
;-------------------------------------------------------------------------------
start:      mov.w   #__STACK_END,SP         ; init stack pointer
            mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; stop WDT
            bis.b   #0x4f,&P4DIR            ; set P4.3 as output
            bic.b	#0x4f,&P4OUT
            bic.b	#0x10,&P3OUT
            bis.b	#0x10,&P3DIR
            bic.b  #0x0f,&P1SEL          ; select GPIO
          	bic.b  #0x0f,&P1DIR          ; configure P1.0 as inputs
          	bis.b  #0x0f,&P1OUT          ; use pull-up
          	bis.b  #0x0f,&P1REN          ; enable pull-up
          	bis.b  #0x0f,&P1IES          ; trigger on high to low transition
          	bis.b  #0x0f,&P1IE           ; P1.0 interrupt enabled
          	bic.b  #0x0f,&P1IFG          ; P1.0 IFG cleared
          	bis.w  #GIE,SR               ; enable general interrupts

trafficLoop:
            mov.w	#0,r4
            bis.b	#0x40,&P4OUT
			xor.b	#0x01,&P4OUT
            push    #DELAY_10
            call	#callDelay
			cmp		#1,r4
				jeq	yellow
            push    #DELAY_10
            call	#callDelay
yellow:     xor.b	#0x05,&P4OUT
            push	#DELAY_5
            call	#callDelay
            xor.b	#0x0c,&P4OUT
            bit.b	#0x02,&P4OUT
            	jc	pedLoop
            push	#DELAY_5
            call	#callDelay
            bic.b	#0x08,&P4OUT
            jmp		trafficLoop

pedLoop:	bis.b	#0x10,&P3OUT
			bic.b	#0x42,&P4OUT
			push	#DELAY_5
			call 	#callDelay
			push	#3
			push	#DELAY_1
			call	#blinkLoop
			push	#10
			push	#DELAY_5th
			call	#blinkLoop
            bic.b	#0x0a,&P4OUT
            bic.b	#0x10,&P3OUT
            jmp		trafficLoop

blinkLoop:
			dec		4(sp)
			push 	4(sp)
			call 	#callDelay
			bic.b	#0x10,&P3OUT
			push	4(sp)
			call	#callDelay
			bis.b	#0x10,&P3OUT
			cmp		#0,4(sp)
				jne	blinkLoop
			mov.w	@sp+,0(sp)
			mov.w	@sp+,0(sp)
			ret



callDelay:  push	r15

callDelay_2:
			mov.w   #COUNT,r15              ;    use R15 as delay counter 2 cycles
            call 	#delayloop				;	5 cycles
            dec.w   4(sp)					;	1 cycles
                jz endLoop
            jmp callDelay_2					;	2 cycles

endLoop:	pop		r15
			mov.w	@sp+,0(sp)
			ret


;1/10 second loop subroutine

delayloop:  sub.w   #1,r15                  ;    delay over? 1 cycles
              jne   delayloop               ;    n 2 cycles
            ret                             ;    y, toggle led 3 cycles


;------------------------------------------------------------------------------
;	Port 1 ISR
;
P1_ISR:
          bic.b  #0x0f,&P1IFG          ; clear P1.0 Interrupt Flag
          bis.b  #0x02,&P4OUT          ; turn on green LED
          mov.w	#1,r4
          reti

          .sect  ".int02"              ; P1 interrupt vector
          .word  P1_ISR



;-------------------------------------------------------------------------------
;           Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect 	.stack

;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .word   start                   ; start address
            .end
