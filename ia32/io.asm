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
endnumber: mov dword [ebp-16],eax
mov eax,4
mov ebx,1
mov ecx,ebp
sub ecx,16
mov edx,1
int 128
add esp,4
mov eax,[ebp+16]
mov edx,0
div dword [ebp+12]
mov dword [ebp+16],eax
jmp writeloop
endwriteloop: add esp,4
push 0x0a
mov eax,4
mov ebx,1
mov ecx,esp
mov edx,1
int 128
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
