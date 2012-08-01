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
    mov ebx, 36
    add eax, ebx
    imul ecx, ebx, 2
    sub eax, ebx
    hlt