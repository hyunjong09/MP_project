	PRESERVE8
	AREA	arraya, CODE, READONLY		
	EXPORT 	init_a
	ENTRY
init_a
	LDR		r0,	array_a
	LDR		r12, array_b	
	LDR		r1, n						
	LDR		r1,[r1] 				
	MOV 	r2, #0					
	
Loopi
	MOV 	r3, #0					
Loopj
	
	ADD		r4,r2,r3
	ADD		r3,r3,#1
	ADD		r5,r2,r3
	ADD		r3,r3,#1
	ADD		r6,r2,r3
	ADD		r3,r3,#1
	ADD		r7,r2,r3
	STM		r0!,{r4,r5,r6,r7}
	ADD		r3,r3,#1
	CMP		r3,r1
	BLT		Loopj
	ADD		r2,r2,#1
	CMP		r2,r1
	BLT		Loopi
	BX 		LR

n			DCD	0x1000
array_a		DCD 0x10000000
array_b		DCD 0x11000000	
	
	END