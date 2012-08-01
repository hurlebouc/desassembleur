extern printf,exit
section .data
out:	db "%d",0xa,0x0	

a: 	 dd 0
b: 	 dd 0
c: 	 dd 0
d:   dd 0
e:   dd 0

section .text
global main

main:

    mov dword[a], 1
    mov dword[c], 0
    mov dword[b], 2
boucle:
    mov eax, [a]
    add eax, [b]
    mov dword[d], eax
    mov eax, [b]
    add eax, [c]
    mov [e], eax
    mov dword[c], 4
    cmp dword[d], 3
    je boucle
    mov ebx, [e]





