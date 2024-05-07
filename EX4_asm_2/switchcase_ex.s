		AREA     SwitchCaseEX, CODE, READONLY
SWI_WriteC  	EQU      &0
SWI_Exit	EQU      &11
		ENTRY				; on entry r0=175, r1=3,
						; c_val=256(c), r5=switch expression(a)
		LDR	r4, b_addr 		; desitination address
		MOV	r0, #0xAF		; stored value
		MOV	r1, #0x3
		MOV	r5, #0x2		; #0x0 : L0, #0x1:L1, #0x2 : L2
			
		CMP	r5, #4			; check value for overrun
		ADDLS	r15, r15, r5, LSL #2	; if OK, add r5 to pc (+8)
		LDMDB 	r11, {r4,r5,r11,r13,pc}	; if not OK, return
		B	L0			; case 0
		B	L1			; case 1
		B 	L2			; case 2
		LDMDB 	r11, {r4,r5,r11,r13,pc}	; case 3 (return)
		MOV	r0, #2			; case 4
		STR	r0, [r4]		; *b ¡ê [r4]
		LDMDB 	r11, {r4,r5,r11,r13,pc}	; return
L0 		STR	r0, [r4]
 		LDMDB 	r11, {r4,r5,r11,r13,pc}	; return
L1 		LDR	r2, c_val		; get c
		CMP	r2, #&64		; c>100?..
		STRLE	r1, [r4]		; no: *b = 3
		STRGT	r0, [r4]		; yes: *b = 175
		LDMDB 	r11, {r4,r5,r11,r13,pc}	; return
c_val 		DCD	&1F
L2 		MOV	r0, #1
		STR	r0, [r4]		; *b = 1
		LDMDB 	r11, {r4,r5,r11,r13,pc}	; return
b_addr		DCD	&20000000	
VALUE2		DCD	&12345678	
            END
