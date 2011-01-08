	org 	7c00h
	xor	ax,ax
	cli
	xor	ax,ax
	mov	ds,ax
	mov	ss,ax
	mov	es,ax
	sti
	mov	ax,0204h
	mov	cx,2
	mov	dh,0
	mov	bx,zz
	int	13h
	jc	_error
	jmp	zz
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
	times	510 - ($ - $$) db 0
	db	055h
	db	0AAh
zz:
	jmp	start
msg	db	'Hello, Video!',0
msgerr  db	'Incorrect video mode',24h
start:
	mov	ah,0fh
	int	10h
	mov	di,0b000h
	cmp	al,7
	je	_2
	mov	di,0b800h
	cmp	al,3
	jbe	_2
_2:
	push	di
	pop	es
;
	mov	ax,[44ch]
	mov	cx,ax	

	shr	ax,4
	xchg	bl,bh
	xor	bh,bh
	mul	bx
	push	es
	pop	bx
	add	ax,bx
	push	ax
	pop	es
	push	cs
	pop	ds
;
	shr	cx,1
	xor	di,di
	mov	ax,0e020h
	rep	stosw
	
;
	mov	si,msg
	xor	di,di
	mov	ah,0d0h
;
_3:
	lodsb
	test	al,al
	jz	_4
	stosw
	jmp	_3

_4:	xor	ah,ah
	int	16h
	cli
	hlt
	times	2024	db	0DDh
