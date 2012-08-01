extern printf,exit
section .data
out:	db "%d",0xa,0x0	

x: 	 dd 0
z: 	 dd 0
t: 	 dd 0


section .text
global main

main:
    mov eax, 3
    jmp a
    
b:
    add eax, ebx
    jmp c
    
e:
    mov eax, ecx
    and esp, ebp
    
d:
    sub eax, ebx
    hlt
    
a:
    mov ebx, 36
    jmp b
    
c:
    imul ecx, ebx, 2
    jmp d

