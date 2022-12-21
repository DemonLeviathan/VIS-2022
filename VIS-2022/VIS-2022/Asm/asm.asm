.586
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
includelib "D:\Универ\3-й семестр\Лабы КПО\Term_Project\VIS-2022\VIS-2022\Debug\StaticLib.lib"
ExitProcess PROTO:DWORD 
.stack 4096


 outnum PROTO : DWORD

 outstr PROTO : DWORD

 concat PROTO : DWORD, : DWORD, : DWORD

 lenght PROTO : DWORD, : DWORD

 strton PROTO : DWORD, : DWORD

.const
		newline byte 13, 10, 0
		LTRL1 byte 'a = ', 0
		LTRL2 byte ', b = ', 0
		LTRL3 byte 'inversion of a = ', 0
		LTRL4 byte 'logical or - (a, b) = ', 0
		LTRL5 byte 'logical and - (a, b) = ', 0
		LTRL6 sdword 2
		LTRL7 byte '(b + a) * (b + a) / 2 = ', 0
		LTRL8 sdword 6
		LTRL9 sdword 10
		LTRL10 byte 'hello', 0
		LTRL11 byte 'length of *hello* is ', 0
		LTRL12 byte ' world!', 0
		LTRL13 byte 'x is bigger than y', 0
		LTRL14 byte 'y is bigger than x', 0
		LTRL15 byte '8', 0
		LTRL16 sdword 1
.data
		temp sdword ?
		buffer byte 256 dup(0)
		sysl sdword 0
		opc sdword 0
		mainx sdword 0
		mainy sdword 0
		mainz sdword 0
		mainstrx dword ?
		mainstry dword ?
		mainresult dword ?
		opres sdword 0
		ops dword ?
.code

;----------- sys ------------
sys PROC,
	sysstr : dword  
; --- save registers ---
push ebx
push edx
; ----------------------

push sysstr
push offset buffer
call lenght
push eax

pop ebx
mov sysl, ebx

; --- restore registers ---
pop edx
pop ebx
; -------------------------
mov eax, sysl
ret
sys ENDP
;------------------------------


;----------- op ------------
op PROC,
	opa : sdword, opb : sdword  
; --- save registers ---
push ebx
push edx
; ----------------------

push offset LTRL1
call outstr


push opa
call outnum


push offset LTRL2
call outstr


push opb
call outnum

push offset newline
call outstr

push opa
	pop eax
	xor eax, 00001111b
	push eax

pop ebx
mov opc, ebx


push offset LTRL3
call outstr


push opc
call outnum

push offset newline
call outstr

push opa
push opb
	pop eax
	pop ebx
	or eax, ebx
	push eax

pop ebx
mov opc, ebx


push offset LTRL4
call outstr


push opc
call outnum

push offset newline
call outstr

push opa
push opb
	pop eax
	pop ebx
	and eax, ebx
	push eax

pop ebx
mov opc, ebx


push offset LTRL5
call outstr


push opc
call outnum

push offset newline
call outstr

push opb
push opa
pop ebx
pop eax
add eax, ebx
push eax
push opb
push opa
pop ebx
pop eax
add eax, ebx
push eax
pop ebx
pop eax
imul eax, ebx
push eax
push LTRL6
pop ebx
pop eax
cdq
idiv ebx
push eax

pop ebx
mov opc, ebx


push offset LTRL7
call outstr


push opc
call outnum

push offset newline
call outstr

; --- restore registers ---
pop edx
pop ebx
; -------------------------
ret
op ENDP
;------------------------------


;----------- MAIN ------------
main PROC
push LTRL8

pop ebx
mov mainx, ebx

push LTRL9

pop ebx
mov mainy, ebx


push mainy
push mainx
call op

mov mainstrx, offset LTRL10

push mainstrx
push offset buffer
call lenght
push eax

pop ebx
mov mainz, ebx


push offset LTRL11
call outstr


push mainz
call outnum

push offset newline
call outstr

mov mainstry, offset LTRL12

push mainstry
push mainstrx
push offset buffer
call concat
mov mainresult, eax

push mainresult
call outstr

push offset newline
call outstr

mov edx, mainx
cmp edx, mainy

jg right1
jl wrong1
right1:

push offset LTRL13
call outstr

jmp next1
wrong1:

push offset LTRL14
call outstr

next1:
push offset newline
call outstr

mov ops, offset LTRL15

push ops
push offset buffer
call strton
push eax

pop ebx
mov opres, ebx


push opres
call outnum

push offset newline
call outstr

mov edx, opres
cmp edx, LTRL16

jg cycle2
jmp cyclenext2
cycle2:

push opres
call outnum

push offset newline
call outstr

push opres
push LTRL6
pop ebx
pop eax
cdq
idiv ebx
push eax

pop ebx
mov opres, ebx

mov edx, opres
cmp edx, LTRL16

jg cycle2
cyclenext2:
push 0
call ExitProcess
main ENDP
end main
