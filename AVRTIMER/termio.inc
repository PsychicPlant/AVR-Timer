;
; Filename: termio.inc
; 
; The following code supplied to the Fall 2018 243-513-DW students for educational/study
; purposes. The use of these routines, in whole or in part, without proper reference to 
; origin is a violation of ISEP regarding Academic Integrity and Plagerism.
;
;Description: Subroutine framework for use in the project (do not modify) 
;
;Author: Mr. Markou

.equ  UBRR  = 25   ;see p.138 (important)	
.equ  FRAME = $86      ;8N1 standard frame
.equ  TXE = $18        ;Transmit & receive enable     
.equ  LF = $0A		   ;ASCII line feed
.equ  CR = $0D		   ;ASCII carriage return
.equ  NUL = 0	       ;string terminating value
.equ  EOL = 0	       ;string terminating value
.equ  EOT = $04	       ;string terminating value

;
; Standard USART init which logically belongs here
; 
init_uart:                 
	ldi R16, 0	       ;always zero (mostly)
	out UBRRH, R16    
	ldi R16, UBRR	 
	out UBRRL, R16     ;config. the rate of data tx 
	ldi R16, TXE      
	out UCSRB, R16     ;enable port tx (see p.158)
	ldi R16, FRAME     ;defined in calling     
	out UCSRC, R16     ;config. frame elements 
	ret
;
;*gets - asm workalike C routine which inputs from UART RxD
; Entry: Z reg must point to buffer  
;        R17 must contain the size of the buffer.
gets:
	dec   R17	       ;leave room for NUL w/o exceeding buffer limit
gtx: 
    rcall getche
	st	  Z+, R16
	cpi	  R16, CR      ;check for returm
	breq  gty
	dec	  R17
	brne  gtx
gty:
	ldi   R16, NUL     ;place end of string
	st    Z, R16	
	ret
;
;*getch - asm workalike C routine to receive char from UART
; Exit:  R16 contain rx char
getch:
	push  r16
	lds	  r16, FLAG3
	cpi	  r16, 0
	brne  getch_ret
	pop   r16
	in    R16,UCSRA
	andi  R16, $80
	breq  getch		   ;poll status for key press
	in	  R16,UDR      ;get char from UART 
	ret
;
;*getche - like above but w/echo
; Exit:  R16 contain rx char
getche:
	rcall getch
	push  R16          ;save R16 on stack
	rcall putchar
	pop  R16           ;and retrieve it back
	ret
;
;*putchar - char tx UART routine 
; Entry: R16  char. to send
putchar:	
	cli
    out  UDR,R16	   ;txmt char. out the TxD 
putc1:		
    in   R16, UCSRA    ;poll status reg
	andi R16, $20      ;check for tx complete
	breq putc1
	sei
	ret	
;
;*puts - asm workalike routine to puts() in C
; Entry: Z index points to an NUL terminated string in cseg or dseg
puts:
	cli
	push r16
puts_loop:
	lpm   R16, Z+      ;get char from memory
	cpi   R16, NUL     
	breq  px		   ;this is the end, my only friend, the end
	rcall putchar      ;send it to uart
	rjmp  puts_loop
px: 
	pop r16
	sei
	ret
;
;*newl - issues a new line (CR&LF) which comes in handy 
; Entry: R16
newl:
	cli
	push r16
	ldi   R16, LF	    ;nothing new here
    rcall putchar
	ldi   R16, CR	 
    rcall putchar
	pop r16
	sei
	ret	

getch_ret:
	pop r16
	ret

;----------
getdata:			;getdata sbr inspired by the one done in class. 
	push r16
	ldi   R17, MAX              ;leave room for a final CR
	ldi   R30, low(input_buff)  ;point Z-reg and buffer
	ldi   R31, high(input_buff)
	rcall gets
	pop r16
	ret

;.include "termio.inc"   ;append library subroutines from same folder
;Dedicated space for string buffers for both the LCD and UART. LCD strings are terminated by $A0 and UART ones, by a NUL
;LED
;The following code is programmed for a 7seg display connected in PORTC
;with the following order: g, f, a, b, c, d, e
seg7code: .db $3f, $0c, $5b, $5e, $6c, $76, $77, $1c, $7f, $7c, $7d, $67, $33, $4f, $73, $71, $0d, 0

;LCD
buff_lcd: .db "SCADA Mon 1.3v ", $A0
buff_lcd0: .db "ID: 1834747", $A0
buff_lcd1: .db "Supervisor ", $A0
buff_lcd2: .db "Node ", $A0
buff_lcd3: .db "Mode Standby ", $A0
buff_lcd4:	.db "Mode Stdby ", $A0
buff_nodeS: .db " S:", $A0
buff_nodeT: .db " T=", $A0
buff_nodeL: .db " L:", $A0
buff_nodeH: .db "H:", $A0, 0

;UART
banner: .db "SCADA Mon 1.3v", LF, CR, "ID: 1834747", LF, CR, NUL
buff_utx1: .db "Svr#", NUL, 0
buff_utx2: .db "Node [a, s, t]>", NUL
adc_start: .db "-----Sampling Voltages, please wait----- ", LF, CR, NUL
adc_stop: .db "-----Sampling done!----- ", LF, CR, NUL
adc_display: .db "-----Displaying Statistics to LCD-----", LF, CR, NUL, 0
adc_trim: .db "Insert new trim level in HEX. (USE UPPERCASE ONLY)", LF, CR,"[$00 >= x >= $FF]> ", NUL
adc_trim_error: .db "!!!INVALID INPUT, RETRY!!! ", LF, CR, NUL
sample_set: .db "-----Displaying Sample Set Sum to LCD-----", LF, CR, NUL, 0