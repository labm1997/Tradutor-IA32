section .text
LerChar:
	push	ebp
	mov	ebp,esp
	mov	eax,3
	mov	ebx,0
	mov	ecx,[ebp+8]
	mov	edx,4
	int	80h
	pop	ebp
	ret	4

EscreverChar:
	push	ebp
	mov	ebp,esp
	mov	eax,4
	mov	ebx,1
	mov	ecx,ebp
	add ecx,8
	mov	edx,4
	int	80h
	pop	ebp
	ret	4

LerString:
	push	ebp
	mov	ebp,esp
	mov	eax,3
	mov	ebx,0
	mov	ecx,[ebp+12]
	mov	edx,[ebp+8]
	shl	edx,2
	int	80h
	pop	ebp
	ret	8

EscreverString:
	push	ebp
	mov	ebp,esp
	mov	eax,4
	mov	ebx,1
	mov	ecx,[ebp+12]
	mov	edx,[ebp+8]
	shl	edx,2
	int	80h
	pop	ebp
	ret	8
