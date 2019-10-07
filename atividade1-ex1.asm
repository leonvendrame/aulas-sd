.INCLUDE "m328Pdef.inc"

.EQU LED0 = PB0
.EQU LED1 = PB1
.EQU LED2 = PB2
.EQU LED3 = PB3
.EQU LED4 = PB4

START:
    IN R16, DDRB
    ORI R16, 0b00011111
    OUT DDRB, R16
	; SBI DDRB, LED0
	; SBI DDRB, LED1
	; SBI DDRB, LED2
	; SBI DDRB, LED3
	; SBI DDRB, LED4

MAIN:
    CLEAR:
        LDI R18, 0
        IN R16, PORTB
        ANDI R16, 0xE0
        OUT PORTB, R16
	INCREMENT:
        RCALL DELAY
        INC R18
        CPI R18, 32
        BREQ CLEAR
        IN R16, PORTB
        ANDI R16, 0xE0
        OR  R16, R18
        OUT PORTB, R16
        RJMP INCREMENT

DELAY:
    LDI R16, 83 ; 83 = ~996ms
    DELAY_LOOP:
        DEC R0
        BRNE DELAY_LOOP
        DEC R1
        BRNE DELAY_LOOP
        DEC R16
        BRNE DELAY_LOOP
    RET
