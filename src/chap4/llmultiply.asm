;
;  Test cases. All numbers are in hex.
;
;      111122223333 *     555566667777 =          5B061D958BF0ECA7C0481B5
;  3456FEDCAAAA1000 * EDBA00112233FF01 = 309A912AF7188C57E62072DD409A1000
;  FFFFEEEEDDDDCCCC * BBBBAAAA99998888 = BBBB9E2692C5DDDCC28F7531048D2C60
;  FFFFFFFFFFFFFFFF * FFFFFFFFFFFFFFFF = FFFFFFFFFFFFFFFE0000000000000001
;  00000001FFFFFFFF * 00000001FFFFFFFF = 0000000000000003FFFFFFFC00000001
;  FFFEFFFFFFFFFFFF * FFFF0001FFFFFFFF = FFFE0002FFFDFFFE0001FFFE00000001
;

;  void llmultiply(unsigned long long int l1,
;                  unsigned long long int l2,
;                  unsigned char *result);
        
;  a = AH * (2 << 32) + AL
;  b = BH * (2 << 32) + BL
;        
;  a * b = (AH * BH            * (2 << 64)) +
;          (AH * BL + AL * BH) * (2 << 32)  +
;           AL * BL

; For more infromation, see the lab manual!
        
SECTION .data



SECTION .text
ALIGN	16
BITS	32

	;; To access parameter BH on the stack you do
	;;   MOV EAX, [EBP + 20]
	;; Replace the zero below with the correct value to be able
	;; to write your assembly code a little bit more readable: 
	;;   MOV EAX, [EBP + BH_OFF]

;; Return address(4 bytes) Databyte + return address 4 byte addressing
AL_OFF	EQU     8	; Offset from EBP to low  bits of a (AL)
AH_OFF	EQU     12	; Offset from EBP to high bits of a (AH)
BL_OFF	EQU     16	; Offset from EBP to low  bits of b (BL)
BH_OFF	EQU     20	; Offset from EBP to high bits of b (BH)
RES_OFF	EQU     24	; Offset from EBP to result array pointer
	;;    ^^^^^ Replace 0 with correct values above
        
	GLOBAL llmultiply

llmultiply:
	PUSH EBP
	MOV EBP , ESP ; Set base pointer to beginning of function


	;Clean result bytes
	MOV ECX  , [EBP + RES_OFF]
	MOV EAX, 0
	MOV [ECX], EAX
	ADD ECX, 4
	MOV [ECX], EAX
	ADD ECX, 4
	MOV [ECX], EAX
	ADD ECX, 4
	MOV [ECX], EAX

	
	; Multiply AL and BL and store it

	MOV EAX , [EBP + AL_OFF]	; move value from EBP + offsett to EAX
	MOV EBX , [EBP + BL_OFF]	; move value from EBP + offsett to EBX
	MUL EBX						; EDX = High, EAX = Low
	; Save carry into RES_OFF
	MOV ECX  , [EBP + RES_OFF]
	MOV [ECX], EAX	
	MOV [ECX + 4] , EDX


	;AH * BL ;Memory location 4 to 7
	MOV EAX , [EBP + AH_OFF]	; move value from EBP + offsett to EAX
	MOV EBX , [EBP + BL_OFF]	; move value from EBP + offsett to EBX
	MUL EBX						; EDX = High, EAX = Low
	
	AND EBX, 0
	ADD [ECX + 4], EAX			;Get old values to ensure it doesn't get overwritten
	ADC [ECX + 8], EDX
	ADC [ECX + 12], EBX

	
	
	;Memory location 8 to 11

	;AL * BH
	MOV EAX , [EBP + AL_OFF]	; move value from EBP + offsett to EAX
	MOV EBX , [EBP + BH_OFF]	; move value from EBP + offsett to EBX
	MUL EBX						; EDX = High, EAX = Low

	;Memory location 4 to 7	
	AND EBX, 0
	ADD [ECX + 4], EAX				;Get old values to ensure it doesn't get overwritten
	ADC [ECX + 8], EDX
	ADC [ECX + 12], EBX


	;AH * BH
	MOV EAX , [EBP + AH_OFF]	; move value from EBP + offsett to EAX
	MOV EBX , [EBP + BH_OFF]	; move value from EBP + offsett to EBX
	MUL EBX						; EDX = High, EAX = Low


	AND EBX, 0	
	ADD [ECX + 8], EAX
	ADC [ECX + 12], EDX

		
	POP EBP				; restore EBP reg
	RET					;  return
