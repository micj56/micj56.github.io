	.import	printi
	.import	printc
	.import	readi
	.import	readc
	.import	exit
	.import	time
	.import	clearAll
	.import	setPixel
	.import	drawLine

	.code
	.align	4

	.export	drawLine1
drawLine1:
	sub	$29,$29,48	; allocate frame
	stw	$25,$29,16	; save old frame pointer
	add	$25,$29,48	; setup new frame pointer
	stw	$31,$25,-36	; save return register
	add	$8,$25,-4
	add	$9,$25,0
	ldw	$9,$9,0
	stw	$9,$8,0
	add	$8,$25,-8
	add	$9,$25,4
	ldw	$9,$9,0
	stw	$9,$8,0
	add	$8,$25,-4
	ldw	$8,$8,0
	stw	$8,$29,0	; store arg #0
	add	$8,$25,-8
	ldw	$8,$8,0
	stw	$8,$29,4	; store arg #1
	add	$8,$25,16
	ldw	$8,$8,0
	stw	$8,$29,8	; store arg #2
	jal	setPixel
	add	$8,$25,-12
	add	$9,$25,8
	ldw	$9,$9,0
	add	$10,$25,0
	ldw	$10,$10,0
	sub	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-16
	add	$9,$25,12
	ldw	$9,$9,0
	add	$10,$25,4
	ldw	$10,$10,0
	sub	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-12
	ldw	$8,$8,0
	add	$9,$0,0
	bge	$8,$9,L0
	add	$8,$25,-16
	ldw	$8,$8,0
	add	$9,$0,0
	bge	$8,$9,L2
	add	$8,$0,0
	add	$9,$25,-12
	ldw	$9,$9,0
	sub	$8,$8,$9
	add	$9,$0,0
	add	$10,$25,-16
	ldw	$10,$10,0
	sub	$9,$9,$10
	bge	$8,$9,L4
	add	$8,$25,-20
	add	$9,$0,0
	add	$10,$25,-16
	ldw	$10,$10,0
	sub	$9,$9,$10
	add	$10,$0,2
	add	$11,$25,-12
	ldw	$11,$11,0
	mul	$10,$10,$11
	add	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-24
	add	$9,$0,0
	add	$10,$0,2
	sub	$9,$9,$10
	add	$10,$25,-16
	ldw	$10,$10,0
	mul	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-28
	add	$9,$0,2
	add	$10,$25,-12
	ldw	$10,$10,0
	mul	$9,$9,$10
	stw	$9,$8,0
L6:
	add	$8,$25,-8
	ldw	$8,$8,0
	add	$9,$25,12
	ldw	$9,$9,0
	ble	$8,$9,L7
	add	$8,$25,-20
	ldw	$8,$8,0
	add	$9,$0,0
	bgt	$8,$9,L8
	add	$8,$25,-4
	add	$9,$25,-4
	ldw	$9,$9,0
	add	$10,$0,1
	sub	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-20
	add	$9,$25,-20
	ldw	$9,$9,0
	add	$10,$25,-24
	ldw	$10,$10,0
	add	$9,$9,$10
	stw	$9,$8,0
L8:
	add	$8,$25,-8
	add	$9,$25,-8
	ldw	$9,$9,0
	add	$10,$0,1
	sub	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-20
	add	$9,$25,-20
	ldw	$9,$9,0
	add	$10,$25,-28
	ldw	$10,$10,0
	add	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-4
	ldw	$8,$8,0
	stw	$8,$29,0	; store arg #0
	add	$8,$25,-8
	ldw	$8,$8,0
	stw	$8,$29,4	; store arg #1
	add	$8,$25,16
	ldw	$8,$8,0
	stw	$8,$29,8	; store arg #2
	jal	setPixel
	j	L6
L7:
	j	L5
L4:
	add	$8,$25,-20
	add	$9,$0,0
	add	$10,$25,-12
	ldw	$10,$10,0
	sub	$9,$9,$10
	add	$10,$0,2
	add	$11,$25,-16
	ldw	$11,$11,0
	mul	$10,$10,$11
	add	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-24
	add	$9,$0,0
	add	$10,$0,2
	sub	$9,$9,$10
	add	$10,$25,-12
	ldw	$10,$10,0
	mul	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-28
	add	$9,$0,2
	add	$10,$25,-16
	ldw	$10,$10,0
	mul	$9,$9,$10
	stw	$9,$8,0
L9:
	add	$8,$25,-4
	ldw	$8,$8,0
	add	$9,$25,8
	ldw	$9,$9,0
	ble	$8,$9,L10
	add	$8,$25,-20
	ldw	$8,$8,0
	add	$9,$0,0
	bgt	$8,$9,L11
	add	$8,$25,-8
	add	$9,$25,-8
	ldw	$9,$9,0
	add	$10,$0,1
	sub	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-20
	add	$9,$25,-20
	ldw	$9,$9,0
	add	$10,$25,-24
	ldw	$10,$10,0
	add	$9,$9,$10
	stw	$9,$8,0
L11:
	add	$8,$25,-4
	add	$9,$25,-4
	ldw	$9,$9,0
	add	$10,$0,1
	sub	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-20
	add	$9,$25,-20
	ldw	$9,$9,0
	add	$10,$25,-28
	ldw	$10,$10,0
	add	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-4
	ldw	$8,$8,0
	stw	$8,$29,0	; store arg #0
	add	$8,$25,-8
	ldw	$8,$8,0
	stw	$8,$29,4	; store arg #1
	add	$8,$25,16
	ldw	$8,$8,0
	stw	$8,$29,8	; store arg #2
	jal	setPixel
	j	L9
L10:
L5:
	j	L3
L2:
	add	$8,$0,0
	add	$9,$25,-12
	ldw	$9,$9,0
	sub	$8,$8,$9
	add	$9,$25,-16
	ldw	$9,$9,0
	bge	$8,$9,L12
	add	$8,$25,-20
	add	$9,$25,-16
	ldw	$9,$9,0
	add	$10,$0,2
	add	$11,$25,-12
	ldw	$11,$11,0
	mul	$10,$10,$11
	add	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-24
	add	$9,$0,2
	add	$10,$25,-16
	ldw	$10,$10,0
	mul	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-28
	add	$9,$0,2
	add	$10,$25,-12
	ldw	$10,$10,0
	mul	$9,$9,$10
	stw	$9,$8,0
L14:
	add	$8,$25,-8
	ldw	$8,$8,0
	add	$9,$25,12
	ldw	$9,$9,0
	bge	$8,$9,L15
	add	$8,$25,-20
	ldw	$8,$8,0
	add	$9,$0,0
	bge	$8,$9,L16
	add	$8,$25,-4
	add	$9,$25,-4
	ldw	$9,$9,0
	add	$10,$0,1
	sub	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-20
	add	$9,$25,-20
	ldw	$9,$9,0
	add	$10,$25,-24
	ldw	$10,$10,0
	add	$9,$9,$10
	stw	$9,$8,0
L16:
	add	$8,$25,-8
	add	$9,$25,-8
	ldw	$9,$9,0
	add	$10,$0,1
	add	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-20
	add	$9,$25,-20
	ldw	$9,$9,0
	add	$10,$25,-28
	ldw	$10,$10,0
	add	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-4
	ldw	$8,$8,0
	stw	$8,$29,0	; store arg #0
	add	$8,$25,-8
	ldw	$8,$8,0
	stw	$8,$29,4	; store arg #1
	add	$8,$25,16
	ldw	$8,$8,0
	stw	$8,$29,8	; store arg #2
	jal	setPixel
	j	L14
L15:
	j	L13
L12:
	add	$8,$25,-20
	add	$9,$0,0
	add	$10,$25,-12
	ldw	$10,$10,0
	sub	$9,$9,$10
	add	$10,$0,2
	add	$11,$25,-16
	ldw	$11,$11,0
	mul	$10,$10,$11
	sub	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-24
	add	$9,$0,0
	add	$10,$0,2
	sub	$9,$9,$10
	add	$10,$25,-12
	ldw	$10,$10,0
	mul	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-28
	add	$9,$0,0
	add	$10,$0,2
	sub	$9,$9,$10
	add	$10,$25,-16
	ldw	$10,$10,0
	mul	$9,$9,$10
	stw	$9,$8,0
L17:
	add	$8,$25,-4
	ldw	$8,$8,0
	add	$9,$25,8
	ldw	$9,$9,0
	ble	$8,$9,L18
	add	$8,$25,-20
	ldw	$8,$8,0
	add	$9,$0,0
	bge	$8,$9,L19
	add	$8,$25,-8
	add	$9,$25,-8
	ldw	$9,$9,0
	add	$10,$0,1
	add	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-20
	add	$9,$25,-20
	ldw	$9,$9,0
	add	$10,$25,-24
	ldw	$10,$10,0
	add	$9,$9,$10
	stw	$9,$8,0
L19:
	add	$8,$25,-4
	add	$9,$25,-4
	ldw	$9,$9,0
	add	$10,$0,1
	sub	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-20
	add	$9,$25,-20
	ldw	$9,$9,0
	add	$10,$25,-28
	ldw	$10,$10,0
	add	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-4
	ldw	$8,$8,0
	stw	$8,$29,0	; store arg #0
	add	$8,$25,-8
	ldw	$8,$8,0
	stw	$8,$29,4	; store arg #1
	add	$8,$25,16
	ldw	$8,$8,0
	stw	$8,$29,8	; store arg #2
	jal	setPixel
	j	L17
L18:
L13:
L3:
	j	L1
L0:
	add	$8,$25,-16
	ldw	$8,$8,0
	add	$9,$0,0
	bge	$8,$9,L20
	add	$8,$25,-12
	ldw	$8,$8,0
	add	$9,$0,0
	add	$10,$25,-16
	ldw	$10,$10,0
	sub	$9,$9,$10
	bge	$8,$9,L22
	add	$8,$25,-20
	add	$9,$0,0
	add	$10,$25,-16
	ldw	$10,$10,0
	sub	$9,$9,$10
	add	$10,$0,2
	add	$11,$25,-12
	ldw	$11,$11,0
	mul	$10,$10,$11
	sub	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-24
	add	$9,$0,0
	add	$10,$0,2
	sub	$9,$9,$10
	add	$10,$25,-16
	ldw	$10,$10,0
	mul	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-28
	add	$9,$0,0
	add	$10,$0,2
	sub	$9,$9,$10
	add	$10,$25,-12
	ldw	$10,$10,0
	mul	$9,$9,$10
	stw	$9,$8,0
L24:
	add	$8,$25,-8
	ldw	$8,$8,0
	add	$9,$25,12
	ldw	$9,$9,0
	ble	$8,$9,L25
	add	$8,$25,-20
	ldw	$8,$8,0
	add	$9,$0,0
	bgt	$8,$9,L26
	add	$8,$25,-4
	add	$9,$25,-4
	ldw	$9,$9,0
	add	$10,$0,1
	add	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-20
	add	$9,$25,-20
	ldw	$9,$9,0
	add	$10,$25,-24
	ldw	$10,$10,0
	add	$9,$9,$10
	stw	$9,$8,0
L26:
	add	$8,$25,-8
	add	$9,$25,-8
	ldw	$9,$9,0
	add	$10,$0,1
	sub	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-20
	add	$9,$25,-20
	ldw	$9,$9,0
	add	$10,$25,-28
	ldw	$10,$10,0
	add	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-4
	ldw	$8,$8,0
	stw	$8,$29,0	; store arg #0
	add	$8,$25,-8
	ldw	$8,$8,0
	stw	$8,$29,4	; store arg #1
	add	$8,$25,16
	ldw	$8,$8,0
	stw	$8,$29,8	; store arg #2
	jal	setPixel
	j	L24
L25:
	j	L23
L22:
	add	$8,$25,-20
	add	$9,$25,-12
	ldw	$9,$9,0
	add	$10,$0,2
	add	$11,$25,-16
	ldw	$11,$11,0
	mul	$10,$10,$11
	add	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-24
	add	$9,$0,2
	add	$10,$25,-12
	ldw	$10,$10,0
	mul	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-28
	add	$9,$0,2
	add	$10,$25,-16
	ldw	$10,$10,0
	mul	$9,$9,$10
	stw	$9,$8,0
L27:
	add	$8,$25,-4
	ldw	$8,$8,0
	add	$9,$25,8
	ldw	$9,$9,0
	bge	$8,$9,L28
	add	$8,$25,-20
	ldw	$8,$8,0
	add	$9,$0,0
	bgt	$8,$9,L29
	add	$8,$25,-8
	add	$9,$25,-8
	ldw	$9,$9,0
	add	$10,$0,1
	sub	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-20
	add	$9,$25,-20
	ldw	$9,$9,0
	add	$10,$25,-24
	ldw	$10,$10,0
	add	$9,$9,$10
	stw	$9,$8,0
L29:
	add	$8,$25,-4
	add	$9,$25,-4
	ldw	$9,$9,0
	add	$10,$0,1
	add	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-20
	add	$9,$25,-20
	ldw	$9,$9,0
	add	$10,$25,-28
	ldw	$10,$10,0
	add	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-4
	ldw	$8,$8,0
	stw	$8,$29,0	; store arg #0
	add	$8,$25,-8
	ldw	$8,$8,0
	stw	$8,$29,4	; store arg #1
	add	$8,$25,16
	ldw	$8,$8,0
	stw	$8,$29,8	; store arg #2
	jal	setPixel
	j	L27
L28:
L23:
	j	L21
L20:
	add	$8,$25,-12
	ldw	$8,$8,0
	add	$9,$25,-16
	ldw	$9,$9,0
	bge	$8,$9,L30
	add	$8,$25,-20
	add	$9,$25,-16
	ldw	$9,$9,0
	add	$10,$0,2
	add	$11,$25,-12
	ldw	$11,$11,0
	mul	$10,$10,$11
	sub	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-24
	add	$9,$0,2
	add	$10,$25,-16
	ldw	$10,$10,0
	mul	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-28
	add	$9,$0,0
	add	$10,$0,2
	sub	$9,$9,$10
	add	$10,$25,-12
	ldw	$10,$10,0
	mul	$9,$9,$10
	stw	$9,$8,0
L32:
	add	$8,$25,-8
	ldw	$8,$8,0
	add	$9,$25,12
	ldw	$9,$9,0
	bge	$8,$9,L33
	add	$8,$25,-20
	ldw	$8,$8,0
	add	$9,$0,0
	bge	$8,$9,L34
	add	$8,$25,-4
	add	$9,$25,-4
	ldw	$9,$9,0
	add	$10,$0,1
	add	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-20
	add	$9,$25,-20
	ldw	$9,$9,0
	add	$10,$25,-24
	ldw	$10,$10,0
	add	$9,$9,$10
	stw	$9,$8,0
L34:
	add	$8,$25,-8
	add	$9,$25,-8
	ldw	$9,$9,0
	add	$10,$0,1
	add	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-20
	add	$9,$25,-20
	ldw	$9,$9,0
	add	$10,$25,-28
	ldw	$10,$10,0
	add	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-4
	ldw	$8,$8,0
	stw	$8,$29,0	; store arg #0
	add	$8,$25,-8
	ldw	$8,$8,0
	stw	$8,$29,4	; store arg #1
	add	$8,$25,16
	ldw	$8,$8,0
	stw	$8,$29,8	; store arg #2
	jal	setPixel
	j	L32
L33:
	j	L31
L30:
	add	$8,$25,-20
	add	$9,$25,-12
	ldw	$9,$9,0
	add	$10,$0,2
	add	$11,$25,-16
	ldw	$11,$11,0
	mul	$10,$10,$11
	sub	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-24
	add	$9,$0,2
	add	$10,$25,-12
	ldw	$10,$10,0
	mul	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-28
	add	$9,$0,0
	add	$10,$0,2
	sub	$9,$9,$10
	add	$10,$25,-16
	ldw	$10,$10,0
	mul	$9,$9,$10
	stw	$9,$8,0
L35:
	add	$8,$25,-4
	ldw	$8,$8,0
	add	$9,$25,8
	ldw	$9,$9,0
	bge	$8,$9,L36
	add	$8,$25,-20
	ldw	$8,$8,0
	add	$9,$0,0
	bge	$8,$9,L37
	add	$8,$25,-8
	add	$9,$25,-8
	ldw	$9,$9,0
	add	$10,$0,1
	add	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-20
	add	$9,$25,-20
	ldw	$9,$9,0
	add	$10,$25,-24
	ldw	$10,$10,0
	add	$9,$9,$10
	stw	$9,$8,0
L37:
	add	$8,$25,-4
	add	$9,$25,-4
	ldw	$9,$9,0
	add	$10,$0,1
	add	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-20
	add	$9,$25,-20
	ldw	$9,$9,0
	add	$10,$25,-28
	ldw	$10,$10,0
	add	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-4
	ldw	$8,$8,0
	stw	$8,$29,0	; store arg #0
	add	$8,$25,-8
	ldw	$8,$8,0
	stw	$8,$29,4	; store arg #1
	add	$8,$25,16
	ldw	$8,$8,0
	stw	$8,$29,8	; store arg #2
	jal	setPixel
	j	L35
L36:
L31:
L21:
L1:
	ldw	$31,$25,-36	; restore return register
	ldw	$25,$29,16	; restore old frame pointer
	add	$29,$29,48	; release frame
	jr	$31		; return

	.export	main
main:
	sub	$29,$29,4	; allocate frame
	stw	$25,$29,0	; save old frame pointer
	add	$25,$29,4	; setup new frame pointer
	ldw	$25,$29,0	; restore old frame pointer
	add	$29,$29,4	; release frame
	jr	$31		; return
