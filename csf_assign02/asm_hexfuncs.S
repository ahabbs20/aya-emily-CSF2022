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
	subq $16, % // i thought when you call a function, it's subq 8, not 16?
	movq %rsp, %rbp

	//rdi has data_buf
	//store return values in rax
	//return read(0, data_buf, 16)
	movq $0, %rdi     //move 0 into first argument for function read
	movq %rdi, %rsi   //move data_buf into second argument for function
	movq $16, %rdx    //move 16 into third argument for function read	
	movq $0, %rax

	call read  //will return an int in %rax

	addq $16, %rsp // same here, aren't you supposed to addq 10? especially bc we have not pushed onto stack here
	popq %rbp
	ret


.globl hex_write_string
hex_write_string:
	//rdi has data_buf
	pushq %rbp   		// push pbq
	subq $16, %rsp		// A little unsure about the alignment, i dont think the total is mod 16 = 0!
	movq %rsp, %rbp		// moved stack to rbp
	pushq %rbx			// rbx = Index  subq 8, rsp
	pushq %r12			// r12 = data_buf subq 8, rsp
	pushq %r13			// r13 = char subq 8, rsp --> See 8 + 16 + 8 + 8 + 8 mod 16 = 3 not aligned?
	movq $0, %rbx //Index
	movq %rdi, %r12  //data_buf

	.LWhileNotNullTerminator:
		movq (%r12, %rbx, 1), %r13 //store r12[rbx] in r9 // wrong size for r13 -> char = 1 byte
		cmpq $0x00, %r13 //if it is a null terminator
		je .LExitLoop //exit the loop
		movq $1, %rdi //otherwise set first term to be 1
		movq $1, %rdx //set third term to be 1 as well
		movq %r13, %rsi //set character // use smaller 
		
		call write
		
		incq %rbx 	// incq rbx by 1 byte
		jmp .LWhileNotNullTerminator
	.LExitLoop:
		popq %r13
		popq %r12
		popq %rbx
		addq $16, %rsp
		popq %rbp
		ret

.globl hex_format_offset
hex_format_offset:
	subq $8, %rsp
	movq %rdi, %r8 //Move offset to r8
	movq $0, %r9  //Move increasing index to r9
	movq $7, %r10 //Move decreasing index to r10
	//rsi is the buf
	.LMainLoopBody:
		cmpq $8, %r9
		je .LDone
		movq %r8, %r11  //r11 will be temp
		andq $0xf, %r11 //temp is anded with 0xf
		cmpq $9, %r11 //Compare temp to 9
		jg .LGreaterThan9 //if it is greater than 9, jump to that point
		//It will be greater than 0 (unsigned)
		addq $48L, %r11 //Add 48 to it
		movb %r11b, (%rsi, %r10, 1)
		decq %r10
		jmp .LContinueLoop
	.LGreaterThan9:
		cmpq $15, %r11
		jg .LContinueLoop
		addq $87L, %r11 //Add 87 to it
		movb %r11b, (%rsi, %r10, 1)
		decq %r10
		jmp .LContinueLoop
	.LContinueLoop:
		shrq $4, %r8
		incq %r9
		jmp .LMainLoopBody
	.LDone: 
		movq $8, %r10
		movb $0x00, 8(%rsi)
		addq $8, %rsp
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
