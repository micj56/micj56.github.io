;
; bootmon.s -- the bootstrap monitor ROM
;

	.set	termbase,0xFF001000	; terminal base address
	.set	diskbase,0xFF002000	; disk base address
	.set	physboot,0x00000400	; where to load the bootstrap sector
	.set	virtbase,0xC0000000	; base of kernel virtual addresses
	.set	virtboot,0xC0000400	; where to start the bootstrap sector
	.set	stacktop,0xC000E000	; where the stack is located
	.set	virtgo,0xC0000000	; where to start a loaded program

	.set	PSW,0
	.set	TLB_INDEX,1
	.set	TLB_ENTRY_HI,2
	.set	TLB_ENTRY_LO,3
	.set	TLB_ENTRIES,32

	.code

reset:
	j	start

start:
	mvts	$0,PSW			; disable interrupts and user mode
	mvts	$0,TLB_ENTRY_LO		; invalidate all TLB entries
	add	$8,$0,virtbase		; by impossible virtual page number
	add	$9,$0,$0
	add	$10,$0,TLB_ENTRIES
tlblp:
	mvts	$8,TLB_ENTRY_HI
	mvts	$9,TLB_INDEX
	tbwi
	add	$8,$8,0x1000		; all entries must be different
	add	$9,$9,1
	bne	$9,$10,tlblp
	add	$29,$0,stacktop		; set stack pointer
	add	$4,$0,signon		; show sign-on message
	jal	mout
cmdlp:
	add	$4,$0,prompt		; show prompt
	jal	mout
	jal	echo
	sub	$8,$2,0x0D		; '<ret>'?
	beq	$8,$0,rcmdlp
	sub	$8,$2,'b'		; 'b'?
	beq	$8,$0,boot
	sub	$8,$2,'g'		; 'g'?
	beq	$8,$0,go
	add	$4,$0,'?'		; unknown command
	jal	cout
rcmdlp:
	jal	crlf
	j	cmdlp

boot:
	jal	crlf
	add	$8,$0,diskbase		; check if disk present
	ldw	$16,$8,16
	beq	$16,$0,nodsk		; no - report error
	add	$4,$0,dfmsg		; say that we found a disk
	jal	mout
	add	$4,$16,$0		; and how many sectors it has
	jal	wout
	add	$4,$0,sctmsg
	jal	mout
	add	$8,$0,diskbase		; try to load the boot sector
	add	$9,$0,1			; sector count
	stw	$9,$8,4
	add	$9,$0,0			; first sector
	stw	$9,$8,8
	add	$9,$0,physboot		; transfer address
	stw	$9,$8,12
	add	$9,$0,1			; start command
	stw	$9,$8,0
dskwt:
	ldw	$9,$8,0
	and	$9,$9,0x10		; busy?
	bne	$9,$0,dskwt
	ldw	$9,$8,0
	and	$9,$9,0x08		; error?
	bne	$9,$0,dskerr
	add	$8,$0,virtboot		; signature present?
	ldbu	$9,$8,512-2
	sub	$9,$9,0x55
	bne	$9,$0,nosig
	ldbu	$9,$8,512-1
	sub	$9,$9,0xAA
	bne	$9,$0,nosig
	jalr	$8			; finally... lift off
	j	cmdlp			; in case we ever return to here

nodsk:
	add	$4,$0,dnfmsg		; there is no disk
	jal	mout
	j	cmdlp

dskerr:
	add	$4,$0,demsg		; disk error
	jal	mout
	j	cmdlp

nosig:
	add	$4,$0,nsgmsg		; no signature
	jal	mout
	j	cmdlp

go:
	jal	crlf
	add	$8,$0,virtgo
	jalr	$8
	j	cmdlp

crlf:
	sub	$29,$29,4
	stw	$31,$29,0
	add	$4,$0,0x0D
	jal	cout
	add	$4,$0,0x0A
	jal	cout
	ldw	$31,$29,0
	add	$29,$29,4
	jr	$31

mout:
	sub	$29,$29,8
	stw	$31,$29,4
	stw	$16,$29,0
	add	$16,$4,$0
mout1:
	ldbu	$4,$16,0
	beq	$4,$0,mout2
	jal	cout
	add	$16,$16,1
	j	mout1
mout2:
	ldw	$16,$29,0
	ldw	$31,$29,4
	add	$29,$29,8
	jr	$31

wout:
	sub	$29,$29,8
	stw	$31,$29,4
	stw	$16,$29,0
	add	$16,$4,$0
	slr	$4,$16,16
	jal	hout
	add	$4,$16,$0
	jal	hout
	ldw	$16,$29,0
	ldw	$31,$29,4
	add	$29,$29,8
	jr	$31

hout:
	sub	$29,$29,8
	stw	$31,$29,4
	stw	$16,$29,0
	add	$16,$4,$0
	slr	$4,$16,8
	jal	bout
	add	$4,$16,$0
	jal	bout
	ldw	$16,$29,0
	ldw	$31,$29,4
	add	$29,$29,8
	jr	$31

bout:
	sub	$29,$29,8
	stw	$31,$29,4
	stw	$16,$29,0
	add	$16,$4,$0
	slr	$4,$16,4
	jal	nout
	add	$4,$16,$0
	jal	nout
	ldw	$16,$29,0
	ldw	$31,$29,4
	add	$29,$29,8
	jr	$31

nout:
	sub	$29,$29,4
	stw	$31,$29,0
	and	$4,$4,0x0F
	add	$4,$4,0x30
	sub	$8,$4,0x3A
	blt	$8,$0,nout1
	add	$4,$4,7
nout1:
	jal	cout
	ldw	$31,$29,0
	add	$29,$29,4
	jr	$31

cin:
	add	$8,$0,termbase
cin1:
	ldw	$9,$8,0
	and	$9,$9,1
	beq	$9,$0,cin1
	ldw	$2,$8,4
	jr	$31

cout:
	add	$8,$0,termbase
cout1:
	ldw	$9,$8,8
	and	$9,$9,1
	beq	$9,$0,cout1
	stw	$4,$8,12
	jr	$31

echo:
	sub	$29,$29,4
	stw	$31,$29,0
	jal	cin
	add	$4,$0,$2
	jal	cout
	ldw	$31,$29,0
	add	$29,$29,4
	jr	$31

signon:
	.byte	0x0D, 0x0A
	.byte	"Bootmon Version 1"
	.byte	0x0D, 0x0A, 0x0D, 0x0A, 0

prompt:
	.byte	"#"
	.byte	0

dnfmsg:
	.byte	"Disk not found!"
	.byte	0x0D, 0x0A, 0

dfmsg:
	.byte	"Disk with 0x"
	.byte	0

sctmsg:
	.byte	" sectors found, booting..."
	.byte	0x0D, 0x0A, 0

demsg:
	.byte	"Disk error!"
	.byte	0x0D, 0x0A, 0

nsgmsg:
	.byte	"MBR signature missing!"
	.byte	0x0D, 0x0A, 0
