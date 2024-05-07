    AREA   addition, CODE, READONLY
    
	EXPORT adder2      ; Export the function name to be used by external modules.

adder2
					  ; R0 has the pointer of arr_in
                      ; R1 has the value of arr_size
	MOV  R2, #0		  ; R2 is sum
	MOV  R3, #0		  ; R3 is counter
loop
	LDR  R4, [R0], #4 ; R4 = arr_in[i]
	ADD  R2, R2, R4   ; sum = sum+1
	ADD  R3, R3, #1   ; counter = counter+1
	CMP  R3, R1		  ; compare counter with arr_size
	BLT  loop
	
	MOV  R0, R2       ; result = sum
    
	BX   lr           ; Return. lr is subroutine return address.
    END
