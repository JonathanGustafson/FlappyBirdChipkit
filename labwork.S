  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #

.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
  #
  # Please add your own code below this line
  #


.global hexasc
.global delay
.global time2string
.global enable_interrupt
.text
hexasc:
	#fel
  	#move $t0, $a0		#move argument to temporary variable
  	andi $a0, $a0,0xf	#mask away upper bits
  	ble $a0, 0x9, number    #if the decimal number can be represented by 0-9
  	nop
  #A-F
  	addi $a0, $a0, 0x37
  	j hexascdone
  	nop
  number:
  	addi $a0, $a0, 0x30
  	j hexascdone
  	nop
  hexascdone:
  	move $v0,$a0
	#andi $v0, $v0, 0x7f
  	jr $ra
  	nop

delay:
  	li $t0, 0				#$t0 = i (loop variabel)
  delaystart:
  	ble $a0, 0, delayend			#while ms > 0
  	nop
  	addi $a0, $a0, -1			#decrement ms
  msstart:
  	beq $t0, 4000000, msend			#börja "räkna" till 1ms
  	nop
  	addi $t0, $t0, 1
  	j msstart
  	nop
  msend:					#1 ms har passerat
  	j delaystart
  	nop
  delayend:
	jr $ra
  	nop

time2string:

  	PUSH $s0			#spara värden av de register som
  	PUSH $s1			#skall använas o subrutinen
  	PUSH $s2
  	PUSH $s3
  	PUSH $s4
  	PUSH $ra
  	PUSH $a0
  	li $s0 , 0
	li $s1 , 0
	li $s2 , 0
	li $s3 , 0
	li $s4 , 0
  	andi $a0, $a1, 0xf000		#minSiffra1
  	srl $a0, $a0, 12
	PUSH $a1
  	jal hexasc
  	nop
	POP $a1
  	or $s0, $s0, $v0
  	andi $a0, $a1, 0xf00		#minSiffra2
  	srl $a0, $a0, 8
	PUSH $a1
  	jal hexasc
  	nop
	POP $a1
  	or $s1, $s1, $v0
  	li $s2, 0x3a			#semikolon
  	andi $a0, $a1, 0xf0		#sekSiffra1
  	srl $a0, $a0, 4
	PUSH $a1
  	jal hexasc
  	nop
	POP $a1
  	or $s3, $s3, $v0
  	andi $a0, $a1, 0xf
	PUSH $a1		#sekSiffra2
  	jal hexasc
  	nop
	POP $a1
  	or $s4, $s4, $v0

  	POP $a0				#återställa adressen som skall skrivas till
  	sb $s0, 0 ($a0)			#Skriv till adressen
  	sb $s1, 1 ($a0)
  	sb $s2, 2 ($a0)
  	sb $s3, 3 ($a0)
  	sb $s4, 4 ($a0)
  	sb $zero, 5 ($a0)

  	POP $ra			 	#återställ använda register
  	POP $s4
  	POP $s3
  	POP $s2
  	POP $s1
  	POP $s0

  	jr $ra
	nop

  enable_interrupt:
    ei
    jr $ra
