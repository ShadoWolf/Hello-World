	org 7C00h
	cli
	xor ax,ax
	mov sp,ax
	mov ss,ax
	sti
	mov ds,ax
	mov es,ax
	jmp	0:f1
f1:
	mov ax,0204h
	mov cx,0002h
	mov dh,ch
	mov bx,buffer
	int 13h
	jc	_error
	
	cli
	
	call	PIC_Configure
	
	lgdt [GDT_Pointer]
	lidt [IDT_Pointer]
	
	mov ax,8*2
	mov ss,ax
	
	mov eax,cr0
	or al,1
	mov cr0,eax
	jmp 8:run
run:
	sti
_repeat:
	jmp	_repeat
	
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

PIC1 equ 0x20 ; IO base address for master PIC
PIC1_COMMAND equ PIC1 ; IO base address for slave PIC
PIC1_DATA equ PIC1+1

PIC2 equ 0xA0
PIC2_COMMAND equ PIC2
PIC2_DATA equ PIC2+1

PIC_START equ 020h

ICW1_ICW4 equ 0x01 ; ICW4 (not) needed
ICW1_SINGLE equ 0x02 ; Single (cascade) mode
ICW1_INTERVAL4 equ 0x04 ; Call address interval 4 (8)
ICW1_LEVEL equ 0x08 ; Level triggered (edge) mode
ICW1_INIT equ 0x10 ; Initialization - required!

ICW4_8086 equ 0x01 ; 8086/88 (MCS-80/85) mode
ICW4_AUTO equ 0x02 ; Auto (normal) EOI
ICW4_BUF_SLAVE equ 0x08 ; Buffered mode/slave
ICW4_BUF_MASTER equ 0x0C ; Buffered mode/master
ICW4_SFNM equ 0x10 ; Special fully nested (not)

PIC_Configure:
	push ax
; re-initialize interrupt #1 contoller

	mov al, ICW1_INIT+ICW1_ICW4
	out PIC1_COMMAND,al
	nop
	mov al, ICW1_INIT+ICW1_ICW4
	out PIC2_COMMAND,al
	nop


	mov al, PIC_START
	out PIC1_DATA, al
	nop
	mov al, PIC_START+8
	out PIC2_DATA, al
	nop
	mov al, 4
	out PIC1_DATA, al
	nop
	mov al, 2
	out PIC2_DATA, al
	nop


	mov al, ICW4_8086
	out PIC1_DATA, al
	nop
	mov al, ICW4_8086
	out PIC2_DATA, al
	nop

	mov al,11111101b
	out PIC1_DATA,al
	nop
	mov al,0FFh
	out PIC2_DATA,al
	nop

	pop ax
	ret

PrintString:
	pusha
	mov ax,8*3
	mov es,ax
	xor di,di
	mov ds,di
	mov al,12
PrintString_loop:
	movsb
	stosb
	loop PrintString_loop
	popa
	ret

Int_13_Message db 'int13'
Int_13:
	push	cx
	push	si
	mov si,Int_13_Message
	mov cx,5
	call PrintString
	pop	si
	pop	cx
	jmp	_halt

Int_Keyb_Message db 20
Int_Keyb:
	push	ax
	push	cx
	push	si
	in al, 60h
	mov byte [Int_Keyb_Message],al
	mov al, 20h
	out 20h, al
	mov si,Int_Keyb_Message
	mov cx,1
	call PrintString
	pop	si
	pop	cx
	pop	ax
	jmp	_halt

message db 'int__'
Int_Other:
	push	cx
	push	si
	mov cx,5
	mov si,message
	call PrintString
	pop	si
	pop	cx
	jmp	_halt
	
GDT_Pointer:
        dw      32 - 1
        dw      GDT_Table
        dw      0
IDT_Pointer:
        dw      256*8
        dw      buffer
        dd      0
GDT_Table:
        times   8 db 0
        db      0xFF,0xFF,  0x00,0x00,0x00,  10011000b, 00000000b, 0x00
        db      0xFF,0xFF,  0x00,0x00,0x00,  10010010b, 00001111b, 0x00
        db      0x00,0x10,  0x00,0x80,0x0B,  10010010b, 00000000b, 0x00

buffer:
times	510 - ($ - $$) db 0
db	055h
db	0AAh

IDT_Table:
	%rep	13
		dw	Int_Other
		dw	8
		db	0, 10000110b, 0 ,0
	%endrep
	
		dw	Int_13
		dw	8
		db	0, 10000110b, 0 ,0
		
	%rep	19
		dw	Int_Other
		dw	8
		db	0, 10000110b, 0 ,0
	%endrep
	
		dw	Int_Keyb
		dw	8
		db	0, 10000110b, 0 ,0
		
	%rep 256 - 34
		dw	Int_Other
		dw	8
		db	0, 10000110b, 0 ,0
	%endrep