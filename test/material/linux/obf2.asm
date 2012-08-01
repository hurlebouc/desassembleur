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
    cmp eax, 3
    je a
    
b:
    add eax, ebx
    cmp eax, 39
    je c
    
e:
    mov eax, ecx
    and esp, ebp
    
d:
    sub eax, ebx
    cmp eax, 3
    jne e
    hlt
    
a:
    mov ebx, 36
    cmp ebx, 36
    je b
    
c:
    imul ecx, ebx, 2
    cmp ecx, 72
    je d

