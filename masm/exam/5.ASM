DATA SEGMENT
    INFO DB 0DH,0AH,'INPUT STRING:$'
    STR DB 31
        DB ?
        DB 30 DUP(0)
    NUM DB 0
    ENG DB 0
DATA ENDS

CODE SEGMENT
    ASSUME DS:DATA,CS:CODE
START:
    MOV AX,DATA
    MOV DS,AX
    
    LEA DX,INFO
    MOV AH,9
    INT 21H
    
    LEA DX,STR
    MOV AH,10
    INT 21H
    
    LEA SI,STR+2
    MOV CL,[STR+1]
    MOV CH,0
LOP:
    MOV BL,[SI]
    CMP BL,58
    JB NEXT
    INC ENG
    JMP EXIT
NEXT:
    INC NUM
EXIT:
    INC SI
    LOOP LOP
    
    MOV AH,4CH   
    INT 21H
CODE ENDS
    END START