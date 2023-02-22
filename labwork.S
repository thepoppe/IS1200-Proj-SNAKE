  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.


.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm



.global delay
.global enableEI

.data


.text

enableEI:
	ei
	jr $ra
	nop


	#delay tar emot ett antal ms i a0
delay:
	move	$t0, $a0			#flyttar argumentet till t0
	
	
	li	$t2, 8000			#villkor till for loop 
						
						
	while:
		ble	$t0, $0, done		#kontrollerar while villkoret
		nop
		
		addi	$t0, $t0, -1	#t0 = t0 - 1
		addi	$t1, $0, 0			#iterator i
		forloop:
			slt $t3, $t1, $t2	#kollar om i är mindre än villkor			
			#Do nothing.
		
			addi $t1, $t1, 1	# i++
			beq $t3, 1, forloop	#jump to forloop om t1 < t2
			nop
		
		j while				#hoppar till whileloop
		nop
	done:	
	jr	$ra
	nop
	
