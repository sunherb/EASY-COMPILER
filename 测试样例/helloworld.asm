SECTION .data 
msg db ' ' , 0h
global _start
    tmpB db 'hello world!$'
call quit
codesg segment
 _start:
    mov eax,msg
    mov edx,eax
    LEA edx,tmpB
    mov eax,09
    int 21H
quit:
    mov exb,0
   mov eax,1
   int 80h
