/*
Partners:
Emily Berger - eberge11@jh.edu
Aya Habbas - ahabbas1@jh.edu
*/

/* Hexdump main function, assembly language version */
	.section .rodata
	sNewLine: .string "\n"
	sColon: .string ":"
	sSingleSpace: .string " "

	.section .text

	.globl main
main:
subq $8, %rsp					// align stack		
subq $5, %rsp					// allocate memory for byte as hex
pushq %rbx						// push %rbx
movq %rsp, %rbx					// move byte_as_hex into register %rbx

subq $20, %rsp					// allocate memory for input
pushq %rbp						// push %rbp
movq %rsp, %rbp					// move rsp  into input

subq $10, %rsp					// allocate memory for offset_string
pushq %r12						// push %r12
movq %rsp, %r12					// move rsp  into register %r12

pushq %r13						// push %r13
movq $0, %r13					// move 0 into register r13

pushq %r14						// push r14
pushq %r15						// push r15 onto stack as counter

subq $5, %rsp					// align stack
		
.LWhileCondition:				// condition
	movq %rbp, %rdi 			// move input into rdi
	call hex_read				// call hex_read
	movq %rax, %r14				// move rax into r14

	cmpq $0, %r14				// cmp %r14 and 0
	je .LRestOfFunction			// jmp to end of loop if == 0
	jmp .LWhileLoop				// jmp to loop content if not

.LWhileLoop:
	movq $0, (%rbp, %r14, 1)	// move \0 into input[num_read_charas]

	movq %r13, %rdi				// move offset into rdi
	movq %r12, %rsi				// move offset_string into rsi
	call hex_format_offset		// call hex_format_offset

	movq %r12, %rdi				// move offsetString into rdi
	call hex_write_string		// call hex_write

	movq $sColon, %rdi			// move offsetColon into rdi
	call hex_write_string		// call hex_write
			
	movq $sSingleSpace, %rdi	// move singleSpace into rdi
	call hex_write_string		// call hexwrite
			
	addq $16, %r13				// add 16 to offset
	movq $0, %r15				// move 0 into r15 as counter
	jmp .LForLoop				


.LForLoop:						// top of for loop
	cmpq $16, %r15				// compare counter to 16 (r15 and r14)	
	jge .LRestOfWhile			// jump if counter >= 16 to rest of while
			
	cmpq %r14, %r15				// cmp counter to num_read_charas
	jge .LElse					// jump if counter >= num_read_charas
			
	movb (%rbp, %r15, 1), %dil	// move input(counter) into rdi
	movq %rbx, %rsi				// move byte_as_hex into rsi
	call hex_format_byte_as_hex	// call hex_format_byte_as_hex
			
	movb (%rbp, %r15, 1), %dil	// move input(counter) into rdi
	call hex_to_printable		// call hex_to_printable
	movb %al, (%rbp, %r15, 1) 	// move rax into input(counter)
	jmp .LRestOfFor				// jump to rest of loop

.LElse:							// else
	movq $32, (%rbx)			// move ‘ ‘ into byte_as_hex[0]
	movq $32, 1(%rbx)			// move ‘ ‘ into byte_as_hex[1]
	movq $0, 2(%rbx)			// move ‘\0‘ into byte_as_hex[2]
	jmp .LRestOfFor				// jump to rest of loop

.LRestOfFor: 					// rest of loop
	movq %rbx, %rdi				// Move byte_as_hex into rdi
	call hex_write_string		// Call hex_write_string
	movq $sSingleSpace, %rdi	// move single space into rdi
	call hex_write_string		// call hex_write_string
	incq %r15				    // Incq counter
	jmp .LForLoop				// jmp to top of the loop

.LRestOfWhile: 					// rest of while
	movq $sSingleSpace, %rdi 	// move single space into rdi
	call hex_write_string		// call hex_write

	movq %rbp, %rdi			 	// move input into rdi
	call hex_write_string		// call hex_write

	movq $sNewLine, %rdi 		// move newline into rdi
	call hex_write_string		// call hex write

	jmp .LWhileCondition		// jump to top of while

.LRestOfFunction:				// end of while
	addq $5, %rsp				
	popq %r15					// pop r15
	popq %r14					// pop r14
	popq %r13					// pop r13
	popq %r12					// pop r12
	addq $10, %rsp
	popq %rbp					// pop rbp
	addq $20, %rsp
	popq %rbx					// pop rbx
	addq $5, %rsp
	addq $8, %rsp				// pop 8


	ret

/* vim:ft=gas:
 */
