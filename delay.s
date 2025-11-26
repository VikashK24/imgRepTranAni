# delay.s - Assembly implementation of delay_ms function
# Syntax: AT&T (GCC/Linux style)

.section .text
.globl delay_ms

# Function: delay_ms
# Input: %rdi = milliseconds to delay (first argument in x86-64 calling convention)
# Output: none
# Description: Busy-wait loop to create delay

delay_ms:
    # Save registers we'll modify (callee-saved registers)
    pushq   %rbx                # Save %rbx
    pushq   %rcx                # Save %rcx
    
    # Convert milliseconds to iterations
    # iterations = milliseconds * 1000
    movq    %rdi, %rax          # Move milliseconds (arg1) to %rax
    imulq   $1000, %rax         # Multiply: %rax = milliseconds * 1000
    movq    %rax, %rcx          # Store iterations count in %rcx
    
    # Initialize loop counter to 0
    xorq    %rbx, %rbx          # %rbx = 0 (loop variable i)
    
.L_loop_start:
    # Check loop condition: if (i >= iterations) exit
    cmpq    %rcx, %rbx          # Compare i (%rbx) with iterations (%rcx)
    jge     .L_loop_end         # Jump to end if i >= iterations
    
    # Loop body - burn CPU cycles
    movl    $0, %edx            # dummy = 0
    addl    $1, %edx            # dummy = dummy + 1
    
    # Increment loop counter
    incq    %rbx                # i++
    
    # Jump back to loop start
    jmp     .L_loop_start
    
.L_loop_end:
    # Restore saved registers
    popq    %rcx                # Restore %rcx
    popq    %rbx                # Restore %rbx
    
    # Return to caller
    ret

# ADD THIS LINE AT THE END:
.section .note.GNU-stack,"",@progbits
