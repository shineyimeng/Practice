DATAS SEGMENT
    SUR DW 0,-1,07FH,07FH,07FH,07FH,07FH,07FH,07FH,07FH
    N EQU $-SUR
    POSTIVE DB 0
    NEGATIVE DB 0
    ZERO DB 0
DATAS ENDS

CODES SEGMENT
    ASSUME CS:CODES,DS:DATAS
START:
	MOV AX,DATAS
    MOV DS,AX
  
    LEA SI,SUR
    MOV CX,N/2
LOP:
    MOV BX,[SI]
    CMP BX,0
    JG NEXT1
    JL NEXT2
    INC ZERO
    JMP EXIT
NEXT1:
    INC POSTIVE
    JMP EXIT
NEXT2:
    INC NEGATIVE
EXIT:
    ADD SI,2
    LOOP LOP
    
    MOV AH,4CH   
    INT 21H
CODES ENDS
    END START