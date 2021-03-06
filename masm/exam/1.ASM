DATA1 SEGMENT
    ORG 2000H   
    SUR1 DW 1,-1,100,100,100,-5,-6,-7,0ffh,0ah
    N EQU $-SUR1
DATA1 ENDS
DATA2 SEGMENT
    ORG 3000H
    SUR2 DW N DUP(?) ;N=20
DATA2 ENDS

CODES SEGMENT
    ASSUME CS:CODES,DS:DATA1,ES:DATA2
START:
	MOV AX,DATA1
    MOV DS,AX
    MOV AX,DATA2
    MOV ES,AX 
    
    LEA SI,SUR1
    LEA DI,SUR2
    MOV CX,N/2
AGAIN:
    MOV AL,[SI] ;因为传送的是16位数，传送时使用的是al要传送20次，ax要10次
    MOV ES:[DI],AL
    ADD SI,2
    ADD DI,2
    DEC CX   
    JNZ AGAIN   
    
    MOV AH,4CH   
    INT 21H
CODES ENDS
    END START