segment .text
	global encrypt

encrypt:	; void encrypt(void *data, size_t data_len, void *text, size_t text_len);
			; 				rdi			rsi					rdx			rcx
	push rbp
	mov rbp, rsp
	xor r8, r8
	xor rax, rax ; init registers
routine:
	mov al, byte [rdi + r8] ; get the octet
	add al, ah
	xor [rdx], al ; modify the octet
	inc rdx
	inc r8
	cmp r8, rsi ; cypher by a key that change each loop
	jne loopinstr ; apply to each bits of data
	add ah, byte 42 ; offset 42 to change the key
	xor r8, r8
loopinstr:
	loop routine ; loop while != rcx
	mov rsp, rbp
	pop rbp
	ret
