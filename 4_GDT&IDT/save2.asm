org     7C00h
	cli
	xor	sp,sp
	mov	ss,sp
	sti
	mov	ds,sp
	mov	es,sp
	jmp	0:f1
f1:
	mov	ax,0204h
	mov	cx,0002h
	mov	dh,ch
	mov	bx,buffer
	int	13h
	jc	_error
	cli
	lgdt	[GDTpointer]
	lidt	[IDTpointer]
	mov	eax,cr0
	or	al,1
	mov	cr0,eax
	
	mov	ax,8*2
	mov	ss,ax
	jmp	8:f2
f2: 
    sti
	int	13
	mov	ax,0DEADh
_halt:
	cli
	hlt
	sti
	jmp	_halt
_error:
	mov ax, 1301h
	mov bp, error
	mov cx, 5
	mov bh, 0
	mov bl, 04
	xor dx, dx
	int 10h
	jmp	_halt
	
	error db 'Error'

string  db      'Random INT'
int_all:
	push	cx
	push	dx
	mov	cx,10
	mov	dx,string
	call	print_string
	pop	dx
	pop	cx
	iret

str_13  db      'INT13'
int_13:
	push	cx
	push	dx
	mov	cx,5
	mov	dx,str_13
	call print_string
	pop	dx
	pop	cx
	iret
	
print_string:
	push	ax
	push	si
	push	di
	push	ds
	push	es
	mov	ax,8*3
	mov	es,ax
	xor	di,di
	mov	ds,di
	mov	si,dx
	mov	al,02h
_lp1:
	movsb
	stosb
	loop	_lp1
	pop	es
	pop	ds
	pop	di
	pop	si
	pop	ax
	ret

GDTpointer:
        dw      32 - 1
        dw      GDT
        dw      0
IDTpointer:
        dw      256*8
        dw      buffer
        dd      0
GDT:
        times   8 db 0
        db      0xFF,0xFF,  0x00,0x00,0x00,  10011000b, 00000000b, 0x00
        db      0xFF,0xFF,  0x00,0x00,0x00,  10010010b, 00001111b, 0x00
        db      0x00,0x10,  0x00,0x80,0x0B,  10010010b, 00000000b, 0x00
buffer:
        times      510 - ($ - $$) db 0
        db      0x55, 0xAA
IDT:
        %rep 13
			dw int_all
			dw 8
			db 0, 10000110b, 0, 0
        %endrep

			dw int_13
			dw 8
			db 0, 10000110b, 0, 0

        %rep 256 - 14
			dw int_all
			dw 8
			db 0, 10000110b, 0, 0
        %endrep