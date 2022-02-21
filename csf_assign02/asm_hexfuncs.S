/* Assembly language implementation of hexdump functions */

	.section .rodata

	/* TODO: add read-only data such as strings here */

	.section .text

	/* TODO: add functions here */

	.globl hex_format_offset

	hex_format_offset:
		ret


.globl hex_format_byte_as_hex
hex_format_byte_as_hex:
	subq $8, %rsp                                 			 // adjust stack
	movb %dil, %r8b                                   		 // move byteval to num
	movq $1, %rcx                                     		 // create index counter
	.LLoops:                                               		 // for loop
		cmpq $-1, %rcx                                     	 // compare index to 2 (for loop comp)
		je .LFinished                                       	 // jump to .LFinished if index == -1 (for loop comp)
		movb %r8b, %dl                                	 // copy num into temp
		andb $0xf, %dl                               	 // mask byte val with 0xf
		cmpb $9, %dl                                  	 // compare temp to 9 (if statement)
		jg .LSecondBranch                             	 // jump to next comparison if temp > 9
		addb $48L, %dl                                 // add temp and 48L
		movb %dl, (%rsi, %rcx, 1)               	 // move temp to sbuf[index]
		decq %rcx                                           // decrement index
		jmp .LRestOfLoop    	                       	 // jump to outside of if/else
	.LSecondBranch:
		addb $87L, %dl                                // add temp and 87L
		movb %dl, (%rsi, %rcx, 1)  	         	// move temp to index
		decq %rcx    	                                   // decrement index
		jmp .LRestOfLoop                              // jump to outside of if/else
	.LRestOfLoop:                                     		// outside if statement
		shrb $4, %r8b                                      	// shift num right 4
		jmp .LLoops                                         	// jump to for loop
	.LFinished:      
		movq $2, %rcx                                  		// move index 2 into rcx
		movb $0x00, 2(%rsi)                         // move ‘\0’ into sbuf[2]
		addq $8, %rsp                                    	// fix stack pointer
		ret                                                 // return


.globl hex_to_printable
hex_to_printable:
	subq $8, %rsp           	    // adjust stack
	cmpb $0x20, %dil    			// compare byte to value 0x20
	jl .LnotPrintable       	   	// jump if %rdi < 0x20 (may not be correct)
	cmpb $0x7E, %dil    	        // comp byte to value 0x7e
	jg .LnotPrintable      	        // jump if %rdi > 0x70 (may not be correct)
	movb %dil, %al      	        // move %rdi to return register
	jmp .LOut                  	    // leave branch
	.LnotPrintable:
		movb $0x2e , %al           // move ‘.’ Into return register
		jmp .LOut   	         	// leave branch
	.LOut:
		addq $8, %rsp           	// Readjust stack
		ret    	                    // return



/* vim:ft=gas:
 */
