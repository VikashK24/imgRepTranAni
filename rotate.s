# rotate.s - Assembly implementation of rotate_right function
# Rotates a grayscale image 90 degrees clockwise

.section .text
.globl rotate_right

# Function: rotate_right
# Input parameters (x86-64 calling convention):
#   %rdi = src pointer (source image buffer)
#   %rsi = dst pointer (destination image buffer)
#   %rdx = old_w (original width)
#   %rcx = old_h (original height)
#
# Algorithm:
#   for (int y = 0; y < old_h; y++)
#       for (int x = 0; x < old_w; x++)
#           dst[x * old_h + (old_h - y - 1)] = src[y * old_w + x];

rotate_right:
    # Save callee-saved registers
    pushq   %rbx
    pushq   %r12
    pushq   %r13
    pushq   %r14
    pushq   %r15
    
    # Save parameters in callee-saved registers (to preserve them)
    movq    %rdi, %r12              # r12 = src pointer
    movq    %rsi, %r13              # r13 = dst pointer
    movq    %rdx, %r14              # r14 = old_w (original width)
    movq    %rcx, %r15              # r15 = old_h (original height)
    
    # Initialize outer loop counter (y = 0)
    xorq    %rbx, %rbx              # rbx = 0 (y counter)
    
.L_outer_loop_start:
    # Check outer loop condition: if (y >= old_h) exit
    cmpq    %r15, %rbx              # Compare y (rbx) with old_h (r15)
    jge     .L_outer_loop_end       # Exit if y >= old_h
    
    # Initialize inner loop counter (x = 0)
    xorq    %rax, %rax              # rax = 0 (x counter)
    
.L_inner_loop_start:
    # Check inner loop condition: if (x >= old_w) go to next y
    cmpq    %r14, %rax              # Compare x (rax) with old_w (r14)
    jge     .L_inner_loop_end       # Go to next y iteration if x >= old_w
    
    # Calculate source index: src_idx = y * old_w + x
    # src_idx = y * old_w + x
    movq    %rbx, %r8               # r8 = y
    imulq   %r14, %r8               # r8 = y * old_w
    addq    %rax, %r8               # r8 = y * old_w + x (source index)
    
    # Get source pixel value
    movzbl  (%r12, %r8, 1), %r9d    # r9d = src[source_index] (zero-extend to 32-bit)
    
    # Calculate destination index: dst_idx = x * old_h + (old_h - y - 1)
    # dst_idx = x * old_h + (old_h - y - 1)
    movq    %rax, %r10              # r10 = x
    imulq   %r15, %r10              # r10 = x * old_h
    
    movq    %r15, %r11              # r11 = old_h
    subq    %rbx, %r11              # r11 = old_h - y
    subq    $1, %r11                # r11 = old_h - y - 1
    
    addq    %r11, %r10              # r10 = x * old_h + (old_h - y - 1) (dest index)
    
    # Write destination pixel: dst[dest_index] = src_pixel
    movb    %r9b, (%r13, %r10, 1)   # dst[dest_index] = r9b (source pixel)
    
    # Increment x (x++)
    incq    %rax                    # x++
    jmp     .L_inner_loop_start     # Jump back to inner loop condition
    
.L_inner_loop_end:
    # Increment y (y++)
    incq    %rbx                    # y++
    jmp     .L_outer_loop_start     # Jump back to outer loop condition
    
.L_outer_loop_end:
    # Restore callee-saved registers
    popq    %r15
    popq    %r14
    popq    %r13
    popq    %r12
    popq    %rbx
    
    # Return
    ret

# Mark stack as non-executable (security)
.section .note.GNU-stack,"",@progbits