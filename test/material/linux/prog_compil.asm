extern printf,exit
section .data
out:	db "%d",0xa,0x0	

x: 	 dd 0
z: 	 dd 0
t: 	 dd 0


section .text
global main

atoi:
	push ebx
	push ecx
	mov ebx,eax
	mov eax,0
	mov ecx,0
atoiloop:
	cmp byte[ebx],0
	je atoidone
	imul eax,10
	mov cl,byte[ebx]
	sub cl,'0'
	add eax,ecx
	inc ebx
	jmp atoiloop
atoidone:
	pop ecx
	pop ebx
	ret

main:
	push ebp
	mov ebp,esp

	cmp dword[ebp+8],2
	jne error_end
	mov ebx,[ebp+12]

	mov	eax, [ebx+4]
	call	atoi
	mov	[x], eax



	mov	eax, 0
	mov	[z], eax
beginwhile0:
	mov	eax, [x]
	cmp	eax, 0
	je	endwhile0
	mov	eax, [x]
	and	eax, 1
	mov	[t], eax
	mov	eax, [t]
	mov	ebx, eax
	mov	eax, 0
	shl	 eax, 1
	cmp	eax, ebx
	jne	endif0
	mov	eax, [z]
	shl	 eax, 1
	mov	[z], eax
endif0:
	mov	eax, [t]
	mov	ebx, eax
	mov	eax, 0
	shl	 eax, 1
	inc	eax
	cmp	eax, ebx
	jne	endif1
	mov	eax, [z]
	shl	 eax, 1
	inc	eax
	mov	[z], eax
endif1:
	mov	eax, [x]
	shr	eax, 1
	mov	[x], eax
	jmp	beginwhile0
endwhile0:


	mov	eax, [z]


endprog:
	push eax
	push dword out
	call printf
	add esp,8
	mov esp,ebp
	pop ebp
	mov eax,0
	ret

error_end:
	pop ebp
	mov eax,1
	ret