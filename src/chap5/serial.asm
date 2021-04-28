		SECTION	.data
		EXTERN	inbound_queue	; (defined in main.C)
data		DB	0		; put rcvd byte here

		SECTION	.text
		ALIGN	16
		BITS	32

BASE_PORT	EQU	3F8h		; we have this in our lab

LSR_PORT	EQU	BASE_PORT+5
RBR_PORT	EQU	BASE_PORT
THR_PORT	EQU	BASE_PORT

; ---------------------------------------------------------------------
; void SerialPut(char ch)
; ---------------------------------------------------------------------
; This function uses programmed waiting loop I/O
; to output the ASCII character 'ch' to the UART.

		GLOBAL	SerialPut

SerialPut:	
		; <your code here>	; (1) Wait for THRE = 1
		MOV DX, LSR_PORT 	;Get UART flags
		IN AL, DX
		AND AL, 0x20 ;0b00100000 ;0x10?
		CMP AL, 0x20
		JNE	SerialPut
		; <your code here>	; (2) Output character to UART
		MOV DX, THR_PORT
		MOV AL, [ESP+4]
		OUT DX, AL
		; <your code here>	; (3) Return to caller
		RET

; ---------------------------------------------------------------------
; void interrupt SerialISR(void)
; ---------------------------------------------------------------------
; This is an Interrupt Service Routine (ISR) for
; serial receive interrupts.  Characters received
; are placed in a queue by calling Enqueue(char).

		GLOBAL	SerialISR
		EXTERN	QueueInsert	; (provided by LIBPC)

SerialISR:	
	       	; Enable (higher-priority) IRQs 	
		STI
		; <your code here>	; (1) Preserve all registers 
		PUSHAD ;Push all general purpose registers

		; <your code here>	; (2) Get character from UART
		MOV DX, LSR_PORT 	;Is bit 1 set?
		IN AL, DX
		AND AX, 0x01
		CMP AX, 0x01 			; (AX & (1 << 0))
		JNE _Eoi			; No data jump to end

		MOV DX, RBR_PORT 	;Recive byte register 
		IN AL, DX
		MOV [data], AL 		;Put data rcvd

;SHOULD CHECK FOR ERRORS BUT WONT B)

		; <your code here>	; (3) Put character into queue 
		;MOV EAX, [inbound_queue]
		PUSH dword data
		PUSH dword [inbound_queue]
		; <your code here>	; Param #2: address of data
		; <your code here>	; Param #1: address of queue

		CALL	QueueInsert

		;POP EAX
		;POP DX
		ADD	ESP,8

_Eoi:	; <your code here>	; (4) Enable lower priority interrupts
		
		MOV AX, 0x20
		MOV DX,	0x20 
		OUT DX, AX
			;This is done by writing 0x20 to port 0x20.

		; <your code here>	;       (Send Non-Specific EOI to PIC)
		; <your code here>	; (5) Restore all registers
		POPAD ;Pop all general purpose registers

		; <your code here>	; (6) Return to interrupted code
		IRET
