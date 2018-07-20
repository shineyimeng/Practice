DATAS SEGMENT
    X EQU 10
    Y EQU 5
    Z DW 0
DATAS ENDS

CODES SEGMENT
    ASSUME CS:CODES,DS:DATAS
START:
	MOV AX,DATAS
    MOV DS,AX
    
    MOV BL,X
    MOV CL,Y
    SUB BL,CL
    MOV AL,BL
    MOV AH,0
    MOV BH,7
    MUL BH
    ADD CL,2
    DIV CL
    MOV Z,AX
        
    MOV AH,4CH   
    INT 21H
CODES ENDS
    END START