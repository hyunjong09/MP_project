    AREA   addition, CODE, READONLY
    
	EXPORT adder      ; Export the function name to be used by external modules.

adder
					  ; R0 has the pointer of arr_in
                      ; R1 has the pointer of arr_out
					  ; R2 has the value of arr_size
	MOV  R8, #0		  ; R3 is result
	MOV  R4, #1		  ; R4 is counter
loop
	LDR  R5, [R0], #4 ; R5 = arr_in[i]
	LDR  R6, [R0]     ; R6 = arr_in[i+1]
	ADD  R7, R5, R6   ; R7 = R5 + R6
	STR  R7, [R1], #4 ; arr_out[i] = R7
	ADD  R8, R8, R5   ; sum = sum + arr_in[i]: i=0~8
	ADD  R4, R4, #1   ; counter = counter+1
	CMP  R4, R2		  ; compare counter with arr_size
	BLT  loop
	

	ADD  R8, R8, R6   ; sum = sum + R6
	STR  R8, [R1]     ; arr_out[9]=sum
    STR	 R8, [R3]		; result = sum
	BX   r14           ; Return. lr is subroutine return address.
    END
