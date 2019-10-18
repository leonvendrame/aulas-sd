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
    
	SBI PORTB, LED4

MAIN:
    LDI ZL, LOW(INTERVAL << 1)
    LDI ZH, HIGH(INTERVAL << 1)
	LPM R19, Z+

	SPEED_UP:
        LDI R20, 0
        LPM R19, Z+
        CPI R19, 255
        BREQ SPEED_DOWN
        SU_CYCLE:
            CALL DELAY
            CALL TURN_ON_NEXT_LED
            CPI R20, 4
            BREQ SPEED_UP
            INC R20
            RJMP SU_CYCLE

    SPEED_DOWN:
        SBIW ZL, 2
        LDI R20, 0
        LPM R19, Z+
        CPI R19, 0
        BREQ MAIN
        SD_CYCLE:
            CALL DELAY
            CALL TURN_ON_NEXT_LED
            CPI R20, 4
            BREQ SPEED_DOWN
            INC R20
            RJMP SD_CYCLE

	RJMP MAIN

TURN_ON_NEXT_LED:
	SBIS PORTB, LED0
	RJMP NEXT_LED 

    CALL RESTART_LEDS
    RET

    NEXT_LED:
        IN 	R16, PORTB
        MOV R17, R16
        ANDI R17, 0x1F
        LSR R17
        MOV R18, R16
        ANDI R18, 0xE0
        OR 	R17, R18
        OUT PORTB, R17
	RET

RESTART_LEDS:
	CBI PORTB, LED0
	SBI PORTB, LED4
	RET

DELAY:
	MOV R2, R19
	DELAY_LOOP:
		DEC R0
		BRNE DELAY_LOOP
		DEC R1
		BRNE DELAY_LOOP
		DEC R2
		BRNE DELAY_LOOP
	RET

INTERVAL: .db 0, 42, 36, 32, 28, 22, 18, 17, 16, 15, 14, 12, 10, 8, 7, 6, 5, 4, 3, 2, 1, 255, 255
