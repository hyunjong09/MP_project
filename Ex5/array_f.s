	PRESERVE8
	AREA	ArrayFunction, CODE, READONLY
	EXPORT 	array_f
	ENTRY
array_f
	LDR		r0,	array_a				;r0 	<- 	0x10000000 (start addr of array_a)
	LDR		r12, array_b			;r12 	<- 	0x11000000 (start addr of array_b)
	LDR		r1, n					;r1 	<- 	0x1000 (value of n)	
	MOV 	r2, #0					;r2 	<- 0 (initialize i=0)
	
Loopj00							
	MOV		r3,#0					;r3 <- 0 (initialize j=0)	
	LDM		r0!,{r4,r5,r6,r7}		;load contents of a[0][0],a[1][1],a[2][2],a[3][3] to r4,r5,r6,r7, respectively
	LDR		r8,[r12]				;load b[0][0]
	ADD		r8,r8,r4				;b[0] <- b[0] + a[0] 
	B		Loopjb					

Loopi
	MOV 	r3, #0					;r3 <- 0 (reset i=0 after each loop)
Loopj
	LDM		r0!,{r4,r5,r6,r7}		;load contents of a[i][j],a[i][j+1],a[i][j+2],a[i][j+3] to r4,r5,r6,r7, respectively
	ADD		r8,r11,r4				;b[i][j] <- b[i][j-1] + a[i][j]		
Loopjb	
	ADD		r9,r8,r5				;b[i][j+1] <- b[i][j] + a[i][j+1]
	ADD		r10,r9,r6				;b[i][j+2] <- b[i][j+1] + a[i][j+2]
	ADD		r11,r10,r7				;b[i][j+3] <- b[i][j+2] + a[i][j+3]
	STM		r12!,{r8,r9,r10,r11}	;store b[i][j], b[i][j+1], b[i][j+2], b[i][j+3]	
	ADD		r3,r3,#4				;r3 <- r3+4 (j=j+4)
	CMP		r3,r1					;j reach to n?
	BLT		Loopj					;if no, jump loopj
	ADD		r2,r2,#1				;if yes, i<-i+1
	CMP		r2,r1					;i reach to n?
	BLT		Loopi					;if no, jump loopi
	BX 		LR						;if yes, return 
n			DCD	0x1000
array_a		DCD 0x10000000
array_b		DCD 0x11000000

	END