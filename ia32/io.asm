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
