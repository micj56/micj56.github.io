	.import	printi
	.import	printc
	.import	readi
	.import	readc
	.import	exit
	.import	time
	.import	clearAll
	.import	setPixel
	.import	drawLine
	.import	_indexError

	.code
	.align	4

	.export	drawCircle1
drawCircle1:
	sub	$29,$29,40		; allocate frame
	stw	$25,$29,16		; save old frame pointer
	add	$25,$29,40		; setup new frame pointer
	stw	$31,$25,-28		; save return register
	add	$8,$25,-4
	add	$9,$0,0
	stw	$9,$8,0
	add	$8,$25,-8
	add	$9,$25,8
	ldw	$9,$9,0
	stw	$9,$8,0
	add	$8,$25,-12
	add	$9,$0,1
	add	$10,$25,8
	ldw	$10,$10,0
	sub	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-16
	add	$9,$0,3
	stw	$9,$8,0
	add	$8,$25,-20
	add	$9,$0,0
	add	$10,$0,2
	sub	$9,$9,$10
	add	$10,$25,8
	ldw	$10,$10,0
	mul	$9,$9,$10
	add	$10,$0,5
	add	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,0
	ldw	$8,$8,0
	add	$9,$25,-4
	ldw	$9,$9,0
	add	$8,$8,$9
	stw	$8,$29,0		; store arg #0
	add	$8,$25,4
	ldw	$8,$8,0
	add	$9,$25,-8
	ldw	$9,$9,0
	add	$8,$8,$9
	stw	$8,$29,4		; store arg #1
	add	$8,$25,12
	ldw	$8,$8,0
	stw	$8,$29,8		; store arg #2
	jal	setPixel
	add	$8,$25,0
	ldw	$8,$8,0
	add	$9,$25,-8
	ldw	$9,$9,0
	add	$8,$8,$9
	stw	$8,$29,0		; store arg #0
	add	$8,$25,4
	ldw	$8,$8,0
	add	$9,$25,-4
	ldw	$9,$9,0
	sub	$8,$8,$9
	stw	$8,$29,4		; store arg #1
	add	$8,$25,12
	ldw	$8,$8,0
	stw	$8,$29,8		; store arg #2
	jal	setPixel
	add	$8,$25,0
	ldw	$8,$8,0
	add	$9,$25,-4
	ldw	$9,$9,0
	sub	$8,$8,$9
	stw	$8,$29,0		; store arg #0
	add	$8,$25,4
	ldw	$8,$8,0
	add	$9,$25,-8
	ldw	$9,$9,0
	sub	$8,$8,$9
	stw	$8,$29,4		; store arg #1
	add	$8,$25,12
	ldw	$8,$8,0
	stw	$8,$29,8		; store arg #2
	jal	setPixel
	add	$8,$25,0
	ldw	$8,$8,0
	add	$9,$25,-8
	ldw	$9,$9,0
	sub	$8,$8,$9
	stw	$8,$29,0		; store arg #0
	add	$8,$25,4
	ldw	$8,$8,0
	add	$9,$25,-4
	ldw	$9,$9,0
	add	$8,$8,$9
	stw	$8,$29,4		; store arg #1
	add	$8,$25,12
	ldw	$8,$8,0
	stw	$8,$29,8		; store arg #2
	jal	setPixel
	add	$8,$25,-4
	ldw	$8,$8,0
	add	$9,$25,-8
	ldw	$9,$9,0
	beq	$8,$9,L0
	add	$8,$25,0
	ldw	$8,$8,0
	add	$9,$25,-8
	ldw	$9,$9,0
	add	$8,$8,$9
	stw	$8,$29,0		; store arg #0
	add	$8,$25,4
	ldw	$8,$8,0
	add	$9,$25,-4
	ldw	$9,$9,0
	add	$8,$8,$9
	stw	$8,$29,4		; store arg #1
	add	$8,$25,12
	ldw	$8,$8,0
	stw	$8,$29,8		; store arg #2
	jal	setPixel
	add	$8,$25,0
	ldw	$8,$8,0
	add	$9,$25,-4
	ldw	$9,$9,0
	add	$8,$8,$9
	stw	$8,$29,0		; store arg #0
	add	$8,$25,4
	ldw	$8,$8,0
	add	$9,$25,-8
	ldw	$9,$9,0
	sub	$8,$8,$9
	stw	$8,$29,4		; store arg #1
	add	$8,$25,12
	ldw	$8,$8,0
	stw	$8,$29,8		; store arg #2
	jal	setPixel
	add	$8,$25,0
	ldw	$8,$8,0
	add	$9,$25,-8
	ldw	$9,$9,0
	sub	$8,$8,$9
	stw	$8,$29,0		; store arg #0
	add	$8,$25,4
	ldw	$8,$8,0
	add	$9,$25,-4
	ldw	$9,$9,0
	sub	$8,$8,$9
	stw	$8,$29,4		; store arg #1
	add	$8,$25,12
	ldw	$8,$8,0
	stw	$8,$29,8		; store arg #2
	jal	setPixel
	add	$8,$25,0
	ldw	$8,$8,0
	add	$9,$25,-4
	ldw	$9,$9,0
	sub	$8,$8,$9
	stw	$8,$29,0		; store arg #0
	add	$8,$25,4
	ldw	$8,$8,0
	add	$9,$25,-8
	ldw	$9,$9,0
	add	$8,$8,$9
	stw	$8,$29,4		; store arg #1
	add	$8,$25,12
	ldw	$8,$8,0
	stw	$8,$29,8		; store arg #2
	jal	setPixel
L0:
L1:
	add	$8,$25,-8
	ldw	$8,$8,0
	add	$9,$25,-4
	ldw	$9,$9,0
	ble	$8,$9,L2
	add	$8,$25,-12
	ldw	$8,$8,0
	add	$9,$0,0
	bge	$8,$9,L3
	add	$8,$25,-12
	add	$9,$25,-12
	ldw	$9,$9,0
	add	$10,$25,-16
	ldw	$10,$10,0
	add	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-16
	add	$9,$25,-16
	ldw	$9,$9,0
	add	$10,$0,2
	add	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-20
	add	$9,$25,-20
	ldw	$9,$9,0
	add	$10,$0,2
	add	$9,$9,$10
	stw	$9,$8,0
	j	L4
L3:
	add	$8,$25,-12
	add	$9,$25,-12
	ldw	$9,$9,0
	add	$10,$25,-20
	ldw	$10,$10,0
	add	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-16
	add	$9,$25,-16
	ldw	$9,$9,0
	add	$10,$0,2
	add	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-20
	add	$9,$25,-20
	ldw	$9,$9,0
	add	$10,$0,4
	add	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,-8
	add	$9,$25,-8
	ldw	$9,$9,0
	add	$10,$0,1
	sub	$9,$9,$10
	stw	$9,$8,0
L4:
	add	$8,$25,-4
	add	$9,$25,-4
	ldw	$9,$9,0
	add	$10,$0,1
	add	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,0
	ldw	$8,$8,0
	add	$9,$25,-4
	ldw	$9,$9,0
	add	$8,$8,$9
	stw	$8,$29,0		; store arg #0
	add	$8,$25,4
	ldw	$8,$8,0
	add	$9,$25,-8
	ldw	$9,$9,0
	add	$8,$8,$9
	stw	$8,$29,4		; store arg #1
	add	$8,$25,12
	ldw	$8,$8,0
	stw	$8,$29,8		; store arg #2
	jal	setPixel
	add	$8,$25,0
	ldw	$8,$8,0
	add	$9,$25,-8
	ldw	$9,$9,0
	add	$8,$8,$9
	stw	$8,$29,0		; store arg #0
	add	$8,$25,4
	ldw	$8,$8,0
	add	$9,$25,-4
	ldw	$9,$9,0
	sub	$8,$8,$9
	stw	$8,$29,4		; store arg #1
	add	$8,$25,12
	ldw	$8,$8,0
	stw	$8,$29,8		; store arg #2
	jal	setPixel
	add	$8,$25,0
	ldw	$8,$8,0
	add	$9,$25,-4
	ldw	$9,$9,0
	sub	$8,$8,$9
	stw	$8,$29,0		; store arg #0
	add	$8,$25,4
	ldw	$8,$8,0
	add	$9,$25,-8
	ldw	$9,$9,0
	sub	$8,$8,$9
	stw	$8,$29,4		; store arg #1
	add	$8,$25,12
	ldw	$8,$8,0
	stw	$8,$29,8		; store arg #2
	jal	setPixel
	add	$8,$25,0
	ldw	$8,$8,0
	add	$9,$25,-8
	ldw	$9,$9,0
	sub	$8,$8,$9
	stw	$8,$29,0		; store arg #0
	add	$8,$25,4
	ldw	$8,$8,0
	add	$9,$25,-4
	ldw	$9,$9,0
	add	$8,$8,$9
	stw	$8,$29,4		; store arg #1
	add	$8,$25,12
	ldw	$8,$8,0
	stw	$8,$29,8		; store arg #2
	jal	setPixel
	add	$8,$25,-4
	ldw	$8,$8,0
	add	$9,$25,-8
	ldw	$9,$9,0
	beq	$8,$9,L5
	add	$8,$25,0
	ldw	$8,$8,0
	add	$9,$25,-8
	ldw	$9,$9,0
	add	$8,$8,$9
	stw	$8,$29,0		; store arg #0
	add	$8,$25,4
	ldw	$8,$8,0
	add	$9,$25,-4
	ldw	$9,$9,0
	add	$8,$8,$9
	stw	$8,$29,4		; store arg #1
	add	$8,$25,12
	ldw	$8,$8,0
	stw	$8,$29,8		; store arg #2
	jal	setPixel
	add	$8,$25,0
	ldw	$8,$8,0
	add	$9,$25,-4
	ldw	$9,$9,0
	add	$8,$8,$9
	stw	$8,$29,0		; store arg #0
	add	$8,$25,4
	ldw	$8,$8,0
	add	$9,$25,-8
	ldw	$9,$9,0
	sub	$8,$8,$9
	stw	$8,$29,4		; store arg #1
	add	$8,$25,12
	ldw	$8,$8,0
	stw	$8,$29,8		; store arg #2
	jal	setPixel
	add	$8,$25,0
	ldw	$8,$8,0
	add	$9,$25,-8
	ldw	$9,$9,0
	sub	$8,$8,$9
	stw	$8,$29,0		; store arg #0
	add	$8,$25,4
	ldw	$8,$8,0
	add	$9,$25,-4
	ldw	$9,$9,0
	sub	$8,$8,$9
	stw	$8,$29,4		; store arg #1
	add	$8,$25,12
	ldw	$8,$8,0
	stw	$8,$29,8		; store arg #2
	jal	setPixel
	add	$8,$25,0
	ldw	$8,$8,0
	add	$9,$25,-4
	ldw	$9,$9,0
	sub	$8,$8,$9
	stw	$8,$29,0		; store arg #0
	add	$8,$25,4
	ldw	$8,$8,0
	add	$9,$25,-8
	ldw	$9,$9,0
	add	$8,$8,$9
	stw	$8,$29,4		; store arg #1
	add	$8,$25,12
	ldw	$8,$8,0
	stw	$8,$29,8		; store arg #2
	jal	setPixel
L5:
	j	L1
L2:
	ldw	$31,$25,-28		; restore return register
	ldw	$25,$29,16		; restore old frame pointer
	add	$29,$29,40		; release frame
	jr	$31			; return

	.export	main
main:
	sub	$29,$29,4		; allocate frame
	stw	$25,$29,0		; save old frame pointer
	add	$25,$29,4		; setup new frame pointer
	ldw	$25,$29,0		; restore old frame pointer
	add	$29,$29,4		; release frame
	jr	$31			; return
