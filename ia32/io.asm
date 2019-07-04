LerChar: push ebp
mov ebp,esp
mov eax,3
mov ebx,0
mov ecx,[ebp+8]
mov edx,2
int 128
pop ebp
ret 4
EscreverChar: push ebp
mov ebp,esp
mov eax,4
mov ebx,0
mov ecx,[ebp+8]
mov edx,1
int 128
dec ebp
mov byte [ebp+1],10
mov eax,4
mov ebx,0
mov ecx,ebp
add ecx,1
mov edx,1
int 128
inc ebp
pop ebp
ret 4
EscreverHexa: push ebp
mov ebp,esp
push 10000000h
push 16
push dword [ebp+8]
call EscreverNum
pop ebp
ret 0
EscreverInteiro: push ebp
mov ebp,esp
push 1000000000
push 10
push dword [ebp+8]
call EscreverNum
pop ebp
ret 0
EscreverNum: push ebp
mov ebp,esp
push eax
push ebx
push 0
cmp dword [ebp+8],0
jge writeloop
cmp dword [ebp+12],10
jne writeloop
mov eax,4
mov ebx,1
push 45
mov ecx,esp
mov edx,1
int 80h
add esp,4
neg dword [ebp+8]
writeloop: cmp dword [ebp+16],0
je endwriteloop
mov eax,[ebp+8]
mov edx,0
div dword [ebp+16]
mov dword [ebp+8],edx
push eax
cmp eax,10
jl isnumber
add eax,57h
jmp endnumber
isnumber: add eax,30h
endnumber: 
cmp eax,30h
jg naoZero
cmp dword [ebp+16],1
je print
cmp dword [ebp-12],0
je endprint
jmp print
naoZero:
mov dword [ebp-12],1
print:
mov dword [ebp-16],eax
mov eax,4
mov ebx,1
mov ecx,ebp
sub ecx,16
mov edx,1
int 128
endprint: add esp,4
mov eax,[ebp+16]
mov edx,0
div dword [ebp+12]
mov dword [ebp+16],eax
jmp writeloop
endwriteloop:
push 0x0a
mov eax,4
mov ebx,1
mov ecx,esp
mov edx,1
int 128
add esp,8
pop ebx
pop eax
pop ebp
ret 12
LerHexa: push ebp
mov ebp,esp
push 16
push dword [ebp+8]
call LerNum
pop ebp
ret 4
LerInteiro: push ebp
mov ebp,esp
push 10
push dword [ebp+8]
call LerNum
pop ebp
ret 4
LerNum: push ebp
mov ebp,esp
push 0
push 0
push 0
push eax
push ebx
readloop: mov dword [ebp-4],0
mov eax,3
mov ebx,0
mov ecx,ebp
sub ecx,4
mov edx,1
int 80h
cmp dword [ebp-4],10
je endread
cmp dword [ebp-4],45
jne endNegative
mov dword [ebp-12],1
jmp readloop
endNegative: cmp dword [ebp-4],97
jge letter
sub dword [ebp-4],30h
jmp endletter
letter: sub dword [ebp-4],57h
endletter: mov eax,[ebp-8]
mul dword [ebp+12]
cmp dword [ebp-12],1
jne positiveNumber
sub eax,[ebp-4]
jmp endSign
positiveNumber: add eax,[ebp-4]
endSign: mov dword [ebp-8],eax
jmp readloop
endread: mov eax,[ebp+8]
mov ebx,[ebp-8]
mov dword [eax],ebx
pop ebx
pop eax
add esp,12
pop ebp
ret 8
LerString:

    push ebp            ;frame de pilha, salva antigo ebp
    mov ebp,esp
	push eax
	push ebx

ler:cmp dword [ebp+8],0
	jle endler
    mov eax,3           ;chamada de sistema para leitura de char
    mov ebx,0
    mov ecx,[ebp + 12]  ;salva char lido no espaço que foi empilhado antes do call
    mov edx,1          
    int 128             ;Chamada de sistema
	mov eax,[ebp + 12]
    cmp dword [eax],0ah
    je endler
    add dword [ebp + 12],4
    dec dword [ebp+8]
    jmp ler
endler:
	pop ebx
	pop eax
    pop ebp             ;retorna valor antigo de ebp
    ret 8               ;retorna limpando o parâmetro 


EscreverString:

    push ebp            ;frame de pilha, salva antigo ebp
    mov ebp,esp      
	push eax
	push ebx

escrever:
	cmp dword [ebp+8],0
	jle endescrever
	mov eax,[ebp + 12]
    cmp dword [eax],0ah
    je endescrever
    mov eax,4           ;chamada de sistema para leitura de char
    mov ebx,0
    mov ecx,[ebp + 12]  ;salva char lido no espaço que foi empilhado antes do call
    mov edx,1          
    int 128             ;Chamada de sistema
    add dword [ebp + 12],4
    dec dword [ebp+8]
    jmp escrever
endescrever:
    dec esp
    mov byte [esp],10
    mov eax,4           ;chamada de sistema para escrita de do /n
    mov ebx,0
    mov ecx,esp         ;imprimir char no espaço que foi empilhado antes do calls
    mov edx,1           ;escrever apenas um único char
    int 128             ;Chamada de sistema
    inc esp
    
	pop ebx
	pop eax
    pop ebp             ;retorna valor antigo de ebp
    ret 8               ;retorna limpando o parâmetro
