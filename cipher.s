	// file header
	.arch armv6                  // armv6 architecture
	.arm                         // arm 32-bit instruction set
	.fpu vfp                     // floating point co-processor
	.syntax unified              // modern syntax

	// constant values you want to use throughout the program
	// could go below like:
	// .equ ONE, 1

	.text                        // start of text segment

	.global encrypt              // make encrypt global for linking to
	.type encrypt, %function     // define encrypt to be a function
	.equ FP_OFFSET, 4            // fp offset distance from sp

encrypt:
	// function prologue
	push {fp, lr}                // stack frame register save
	add fp, sp, FP_OFFSET        // set frame pointer to frame base

	// --- DO NOT EDIT LINES ABOVE ---

	LSR R2, R0, #4		//Shift to the right 4 bits R2=R0/pow(2,4)
	LSL R3, R0, #12		//Shift to the left 12 bits R3=R0*pow(2,12)
	LSR R3, R3, #8		//Shift to the right 8 bits R3=R3/pow(2,8)
	ADD R3, R2, R3		//R3 = R2+R3
	EOR R0, R3, R1		//R0 = R3^R1
	
	// --- DO NOT EDIT LINES BELOW ---

	// function epilogue
	sub sp, fp, FP_OFFSET        // restore stack frame top
	pop {fp, lr}                 // remove frame and restore registers
	bx lr                        // return to caller

	// function footer
	.size encrypt, (. - encrypt) // set size for function

	.global decrypt              // make encrypt global for linking to
	.type decrypt, %function     // define encrypt to be a function
	.equ FP_OFFSET, 4            // fp offset distance from sp

decrypt:
	// function prologue
	push {fp, lr}                // stack frame register save
	add fp, sp, FP_OFFSET        // set frame pointer to frame base

	// --- DO NOT EDIT LINES ABOVE ---

	EOR R2, R0, R1		//R2 = R0^R1
	LSR R3, R2, #4		//Shift to the right 4 bits R3 = R2/pow(2,4)
	LSL R2, R2, #12		//Shift to the left 12 bits R2 = R2*pow(2,12)
	LSR R2, R2, #8		//Shift to the right 8 bits R2 = R2/pow(2,8)
	ADD R0, R2, R3		//R0 = R2+R3

	// --- DO NOT EDIT LINES BELOW ---
	// function epilogue
	sub sp, fp, FP_OFFSET        // restore stack frame top
	pop {fp, lr}                 // remove frame and restore registers
	bx lr                        // return to caller

	// function footer
	.size decrypt, (. - decrypt) // set size for function

	// file footer
	.section .note.GNU-stack, "", %progbits // stack/data non-exec (linker)
.end
	Template is Arm Procedure Call Standard Compliant (for Linux)
