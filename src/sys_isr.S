.macro push_regs
  pushq %r15
  pushq %r14
  pushq %r13
  pushq %r12
  pushq %r11
  pushq %r10
  pushq %r9
  pushq %r8
  pushq %rbp
  pushq %rdi
  pushq %rsi
  pushq %rdx
  pushq %rcx
  pushq %rbx
  pushq %rax
.endm

.macro pop_regs
  popq %rax
  popq %rbx
  popq %rcx
  popq %rdx
  popq %rsi
  popq %rdi
  popq %rbp
  popq %r8
  popq %r9
  popq %r10
  popq %r11
  popq %r12
  popq %r13
  popq %r14
  popq %r15
.endm

.extern isr_handler_common
.macro isr_noerr index
  .global _isr_\index
  .type _isr_\index, @function
  .align 4
  _isr_\index:
    pushq $0
    push_regs
    movq $\index, %rdi
    pushq %rdi
    call isr_handler_common
    pop_regs
    popq %rdi
    addq $8, %rsp
    iretq
  .size _isr_\index, .-_isr_\index
.endm
.macro isr_err index
  .global _isr_\index
  .type _isr_\index, @function
  .align 4
  _isr_\index:
    push_regs
    movq $\index, %rdi
    pushq %rdi
    call isr_handler_common
    popq %rdi
    pop_regs
    addq $8, %rsp
    iretq
  .size _isr_\index, .-_isr_\index
.endm

isr_noerr 0
isr_noerr 1
isr_noerr 2
isr_noerr 3
isr_noerr 4
isr_noerr 5
isr_noerr 6
isr_noerr 7
isr_err 8
isr_noerr 9
isr_err 10
isr_err 11
isr_err 12
isr_err 13
isr_err 14
isr_noerr 15
isr_noerr 16
isr_noerr 17
isr_noerr 18
isr_noerr 19
isr_noerr 20
isr_noerr 21
isr_noerr 22
isr_noerr 23
isr_noerr 24
isr_noerr 25
isr_noerr 26
isr_noerr 27
isr_noerr 28
isr_noerr 29
isr_noerr 30
isr_noerr 31

