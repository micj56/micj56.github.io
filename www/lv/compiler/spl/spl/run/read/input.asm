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

	.export	readLine
readLine:
	sub	$29,$29,20	; allocate frame
	stw	$25,$29,8	; save old frame pointer
	add	$25,$29,20	; setup new frame pointer
	stw	$31,$25,-16	; save return register
	add	$8,$25,-4
	add	$9,$0,0
	stw	$9,$8,0
L0:
	add	$8,$25,-4
	ldw	$8,$8,0
	add	$9,$0,79
	bge	$8,$9,L1
	add	$8,$25,-8
	stw	$8,$29,0	; store arg #0
	jal	readc
	add	$8,$25,-8
	ldw	$8,$8,0
	add	$9,$0,32
	blt	$8,$9,L2
	add	$8,$25,-8
	ldw	$8,$8,0
	stw	$8,$29,0	; store arg #0
	jal	printc
	add	$8,$25,0
	ldw	$8,$8,0
	add	$9,$25,-4
	ldw	$9,$9,0
	mul	$9,$9,4
	add	$8,$8,$9
	add	$9,$25,-8
	ldw	$9,$9,0
	stw	$9,$8,0
	add	$8,$25,-4
	add	$9,$25,-4
	ldw	$9,$9,0
	add	$10,$0,1
	add	$9,$9,$10
	stw	$9,$8,0
	j	L3
L2:
	add	$8,$25,-8
	ldw	$8,$8,0
	add	$9,$0,13
	bne	$8,$9,L4
	add	$8,$25,-8
	ldw	$8,$8,0
	stw	$8,$29,0	; store arg #0
	jal	printc
	add	$8,$0,10
	stw	$8,$29,0	; store arg #0
	jal	printc
	add	$8,$25,0
	ldw	$8,$8,0
	add	$9,$25,-4
	ldw	$9,$9,0
	mul	$9,$9,4
	add	$8,$8,$9
	add	$9,$0,0
	stw	$9,$8,0
	add	$8,$25,-4
	add	$9,$0,79
	stw	$9,$8,0
	j	L5
L4:
	add	$8,$25,-8
	ldw	$8,$8,0
	add	$9,$0,8
	bne	$8,$9,L6
	add	$8,$25,-4
	ldw	$8,$8,0
	add	$9,$0,0
	ble	$8,$9,L7
	add	$8,$0,8
	stw	$8,$29,0	; store arg #0
	jal	printc
	add	$8,$0,32
	stw	$8,$29,0	; store arg #0
	jal	printc
	add	$8,$0,8
	stw	$8,$29,0	; store arg #0
	jal	printc
	add	$8,$25,-4
	add	$9,$25,-4
	ldw	$9,$9,0
	add	$10,$0,1
	sub	$9,$9,$10
	stw	$9,$8,0
L7:
L6:
L5:
L3:
	j	L0
L1:
	add	$8,$25,0
	ldw	$8,$8,0
	add	$9,$0,79
	mul	$9,$9,4
	add	$8,$8,$9
	add	$9,$0,0
	stw	$9,$8,0
	ldw	$31,$25,-16	; restore return register
	ldw	$25,$29,8	; restore old frame pointer
	add	$29,$29,20	; release frame
	jr	$31		; return

	.export	readInt
readInt:
	sub	$29,$29,20	; allocate frame
	stw	$25,$29,8	; save old frame pointer
	add	$25,$29,20	; setup new frame pointer
	stw	$31,$25,-16	; save return register
	add	$8,$25,-4
	add	$9,$0,1
	stw	$9,$8,0
L8:
	add	$8,$25,-4
	ldw	$8,$8,0
	add	$9,$0,1
	bne	$8,$9,L9
L10:
	add	$8,$25,0
	ldw	$8,$8,0
	add	$9,$25,4
	ldw	$9,$9,0
	ldw	$9,$9,0
	mul	$9,$9,4
	add	$8,$8,$9
	ldw	$8,$8,0
	add	$9,$0,32
	bne	$8,$9,L11
	add	$8,$25,4
	ldw	$8,$8,0
	add	$9,$25,4
	ldw	$9,$9,0
	ldw	$9,$9,0
	add	$10,$0,1
	add	$9,$9,$10
	stw	$9,$8,0
	j	L10
L11:
	add	$8,$25,0
	ldw	$8,$8,0
	add	$9,$25,4
	ldw	$9,$9,0
	ldw	$9,$9,0
	mul	$9,$9,4
	add	$8,$8,$9
	ldw	$8,$8,0
	add	$9,$0,0
	bne	$8,$9,L12
	add	$8,$25,0
	ldw	$8,$8,0
	stw	$8,$29,0	; store arg #0
	jal	readLine
	add	$8,$25,4
	ldw	$8,$8,0
	add	$9,$0,0
	stw	$9,$8,0
	j	L13
L12:
	add	$8,$25,-4
	add	$9,$0,0
	stw	$9,$8,0
L13:
	j	L8
L9:
	add	$8,$25,0
	ldw	$8,$8,0
	add	$9,$25,4
	ldw	$9,$9,0
	ldw	$9,$9,0
	mul	$9,$9,4
	add	$8,$8,$9
	ldw	$8,$8,0
	add	$9,$0,45
	bne	$8,$9,L14
	add	$8,$25,-8
	add	$9,$0,0
	add	$10,$0,1
	sub	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,4
	ldw	$8,$8,0
	add	$9,$25,4
	ldw	$9,$9,0
	ldw	$9,$9,0
	add	$10,$0,1
	add	$9,$9,$10
	stw	$9,$8,0
	j	L15
L14:
	add	$8,$25,-8
	add	$9,$0,1
	stw	$9,$8,0
L15:
	add	$8,$25,8
	ldw	$8,$8,0
	add	$9,$0,0
	stw	$9,$8,0
	add	$8,$25,-4
	add	$9,$0,1
	stw	$9,$8,0
L16:
	add	$8,$25,-4
	ldw	$8,$8,0
	add	$9,$0,1
	bne	$8,$9,L17
	add	$8,$25,0
	ldw	$8,$8,0
	add	$9,$25,4
	ldw	$9,$9,0
	ldw	$9,$9,0
	mul	$9,$9,4
	add	$8,$8,$9
	ldw	$8,$8,0
	add	$9,$0,48
	blt	$8,$9,L18
	add	$8,$25,0
	ldw	$8,$8,0
	add	$9,$25,4
	ldw	$9,$9,0
	ldw	$9,$9,0
	mul	$9,$9,4
	add	$8,$8,$9
	ldw	$8,$8,0
	add	$9,$0,57
	bgt	$8,$9,L20
	add	$8,$25,8
	ldw	$8,$8,0
	add	$9,$25,8
	ldw	$9,$9,0
	ldw	$9,$9,0
	add	$10,$0,10
	mul	$9,$9,$10
	add	$10,$25,0
	ldw	$10,$10,0
	add	$11,$25,4
	ldw	$11,$11,0
	ldw	$11,$11,0
	mul	$11,$11,4
	add	$10,$10,$11
	ldw	$10,$10,0
	add	$9,$9,$10
	add	$10,$0,48
	sub	$9,$9,$10
	stw	$9,$8,0
	add	$8,$25,4
	ldw	$8,$8,0
	add	$9,$25,4
	ldw	$9,$9,0
	ldw	$9,$9,0
	add	$10,$0,1
	add	$9,$9,$10
	stw	$9,$8,0
	j	L21
L20:
	add	$8,$25,-4
	add	$9,$0,0
	stw	$9,$8,0
L21:
	j	L19
L18:
	add	$8,$25,-4
	add	$9,$0,0
	stw	$9,$8,0
L19:
	j	L16
L17:
	add	$8,$25,8
	ldw	$8,$8,0
	add	$9,$25,8
	ldw	$9,$9,0
	ldw	$9,$9,0
	add	$10,$25,-8
	ldw	$10,$10,0
	mul	$9,$9,$10
	stw	$9,$8,0
	ldw	$31,$25,-16	; restore return register
	ldw	$25,$29,8	; restore old frame pointer
	add	$29,$29,20	; release frame
	jr	$31		; return

	.export	main
main:
	sub	$29,$29,348	; allocate frame
	stw	$25,$29,16	; save old frame pointer
	add	$25,$29,348	; setup new frame pointer
	stw	$31,$25,-336	; save return register
	add	$8,$25,-324
	add	$9,$0,0
	stw	$9,$8,0
	add	$8,$25,-320
	add	$9,$0,0
	mul	$9,$9,4
	add	$8,$8,$9
	add	$9,$0,0
	stw	$9,$8,0
L22:
	add	$8,$0,1
	add	$9,$0,1
	bne	$8,$9,L23
	add	$8,$25,-320
	stw	$8,$29,0	; store arg #0
	add	$8,$25,-324
	stw	$8,$29,4	; store arg #1
	add	$8,$25,-328
	stw	$8,$29,8	; store arg #2
	jal	readInt
	add	$8,$25,-328
	ldw	$8,$8,0
	stw	$8,$29,0	; store arg #0
	jal	printi
	add	$8,$0,10
	stw	$8,$29,0	; store arg #0
	jal	printc
	j	L22
L23:
	ldw	$31,$25,-336	; restore return register
	ldw	$25,$29,16	; restore old frame pointer
	add	$29,$29,348	; release frame
	jr	$31		; return
