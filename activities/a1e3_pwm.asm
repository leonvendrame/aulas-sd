.INCLUDE "m328Pdef.inc"

.EQU LED0 = PB0

START:
    SBI DDRB, LED0

MAIN:
    LDI R17, 0
    LDI R18, 10
    LDI R20, 0
    RESTART:
        SBI PORTB, LED0
        MOV R16, R17
        CALL DELAY
        CBI PORTB, LED0
        MOV R16, R18
        SUB R16, R17
        CALL DELAY
        INC R17
        INC R20
        CPI R20, 11
        BREQ REDUCE

        RJMP RESTART

    REDUCE:
    LDI R16, 83
    CALL DELAY
    RJMP MAIN
    NOP

DELAY:
    MOV R2, R16
    CPI R16, 0
    BREQ END_DELAY
    DELAY_LOOP:
        DEC R0
        BRNE DELAY_LOOP
        DEC R1
        BRNE DELAY_LOOP
        DEC R2
        BRNE DELAY_LOOP
    END_DELAY:
    RET
