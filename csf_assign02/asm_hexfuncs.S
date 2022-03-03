/* Assembly language implementation of hexdump functions */
/* Emily Berger - eberge11@jh.edu
   Aya Habbas - ahabbas1@jh.edu */

	.section .rodata

	/* TODO: add read-only data such as strings here */

	.section .text

	/* TODO: add functions here */

.globl hex_read
hex_read:
	pushq %rbp
	subq $8, %rsp //subtracting 8 from stack pointer
	movq %rsp, %rbp

	//rdi has data_buf
	//store return values in rax
	//return read(0, data_buf, 16)
	movq %rdi, %rsi   //move data_buf into second argument for function
	movq $0, %rdi //move 0 into first argument for function read (must come after the usage of rdi as data_buf)
	movq $16, %rdx    //move 16 into third argument for function read	

	call read  //will return an int in %rax

	addq $8, %rsp //adding 8 back to stack pointer
	popq %rbp
	ret


.globl hex_write_string
hex_write_string:
	pushq %rbx  // rbx = Index  
	pushq %r12  // r12 = data_buf 
	pushq %r13  // r13 = char
	movq $0, %rbx  //Index
	movq %rdi, %r12  //data_buf

	.LWriteWhileNotNullTerminator:
		movb (%r12, %rbx, 1), %r13b  // store r12[rbx] in r13b
		cmpb $0x00, %r13b  // if it is a null terminator
		je .LWriteExitLoop  // exit the loop

		movq $1, %rdi  // otherwise set first term to be 1
		leaq (%r12, %rbx, 1), %rsi  //set character
		movq $1, %rdx  // set third term to be 1 as well

		call write  //function call
		incq %rbx   // incq rbx by 1
		jmp .LWriteWhileNotNullTerminator
	.LWriteExitLoop:
		popq %r13 
		popq %r12  
		popq %rbx
		ret

.globl hex_format_offset
hex_format_offset:
	subq $8, %rsp
	movq %rdi, %r8 //Move offset to r8
	movq $0, %r9  //Move increasing index to r9
	movq $7, %r10 //Move decreasing index to r10
	//rsi is the buf
	.LOffsetLoopBody:
		cmpq $8, %r9
		je .LOffsetDone
		movq %r8, %r11  //r11 will be temp
		andq $0xf, %r11 //temp is anded with 0xf
		cmpq $9, %r11 //Compare temp to 9
		jg .LOffsetGreaterThan9 //if it is greater than 9, jump to that point
		//It will be greater than 0 (unsigned)
		addq $48L, %r11 //Add 48 to it
		movb %r11b, (%rsi, %r10, 1)
		decq %r10
		jmp .LOffsetContinueLoop
	.LOffsetGreaterThan9:
		cmpq $15, %r11
		jg .LOffsetContinueLoop
		addq $87L, %r11 //Add 87 to it
		movb %r11b, (%rsi, %r10, 1)
		decq %r10
		jmp .LOffsetContinueLoop
	.LOffsetContinueLoop:
		shrq $4, %r8
		incq %r9
		jmp .LOffsetLoopBody
	.LOffsetDone: 
		movq $8, %r10
		movb $0x00, 8(%rsi)
		addq $8, %rsp
		ret


.globl hex_format_byte_as_hex
hex_format_byte_as_hex:
	subq $8, %rsp                                 			 // adjust stack
	movb %dil, %r8b                                   		 // move byteval to num
	movq $1, %rcx                                     		 // create index counter
	.LFormatByteAsHexBody:                                           // for loop
		cmpq $-1, %rcx                                   // compare index to 2 (for loop comp)
		je .LByteFinished                                        // jump to .LFinished if index == -1 (for loop comp)
		movb %r8b, %dl                                	 // copy num into temp
		andb $0xf, %dl                               	 // mask byte val with 0xf
		cmpb $9, %dl                                  	 // compare temp to 9 (if statement)
		jg .LByteSecondBranch                             	 // jump to next comparison if temp > 9
		addb $48L, %dl                                   // add temp and 48L
		movb %dl, (%rsi, %rcx, 1)               	 // move temp to sbuf[index]
		decq %rcx                                        // decrement index
		jmp .LByteRestOfLoop    	                       	 // jump to outside of if/else
	.LByteSecondBranch:
		addb $87L, %dl                                   // add temp and 87L
		movb %dl, (%rsi, %rcx, 1)  	         	 // move temp to index
		decq %rcx    	                                 // decrement index
		jmp .LByteRestOfLoop                                    // jump to outside of if/else
	.LByteRestOfLoop:                                     		// outside if statement
		shrb $4, %r8b                                    // shift num right 4
		jmp .LFormatByteAsHexBody                               // jump to for loop
	.LByteFinished:      
		movq $2, %rcx                                    // move index 2 into rcx
		movb $0x00, 2(%rsi)                              // move ‘\0’ into sbuf[2]
		addq $8, %rsp                                    // fix stack pointer
		ret                                              // return


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
