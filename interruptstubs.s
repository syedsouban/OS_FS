.set IRQ_BASE, 0x20

.section .text

.extern _ZN16InterruptManager15handleInterruptEj
.global _ZN16InterruptManager22IgnoreInterruptRequestEv

# .macro HandleInterruptException num
# .global _ZN16InterruptManager28HandleInterruptException\num\()Ev
# _ZN16InterruptManager28HandleInterruptException\num\()Ev:
# 	movb $\num, (interruptnumber)
# 	jmp int_bottom
# .endm

# macros are used for generating assembly code in preprocessing stage of assembler
# the syntax for assembler is:
# .macro macro_name param1 param2 ....
# macro_body
# .endm
# all params passed to the macro will be replaced by the calling parameters after assembler's preprocessing step
# the .endm denotes the end of the macro
# and then wherever params are used they have to be refered with a backslash(they have to be escaped)
# and to separate out params from other symbols written inside the macro using /() as the separator
# in the above macro if we would not have used \() the global label would have been:
# _ZN16InterruptManager26handleInterruptException\numEv
# As you can see the assembler will not be able to distinguish between params and symbols
# So to mark boundary to any parameter we can use \()


# .macro handleInterruptRequest num
# .global _ZN16InterruptManager26HandleInterruptRequest\num\()Ev
# _ZN16InterruptManager26HandleInterruptRequest\num\()Ev:
#	movb $\num + IRQ_BASE, (interruptnumber)
#	jmp int_bottom
# .endm

# _ZN16InterruptManager26HandleInterruptRequest0x20Ev:
# 	# movb $\num + IRQ_BASE, (interruptnumber)
# 	jmp int_bottom

# _ZN16InterruptManager26HandleInterruptRequest0x21Ev:
# 	# movb $\num + IRQ_BASE, (interruptnumber)
# 	jmp int_bottom

_ZN16InterruptManager22IgnoreInterruptRequestEv:
	pushl %esp
	call _ZN16InterruptManager15handleInterruptEj
	mov %eax, %esp
	iret
	# jmp int_bottom

# handleInterruptRequest 0x00
# handleInterruptRequest 0x01

int_bottom:

	pusha
	pushl %ds
	pushl %es
	pushl %fs
	pushl %gs
	push %esp
	call _ZN16InterruptManager15handleInterruptEj
	# #addl $5, %esp
	movl %eax, %esp

	pop %gs
	pop %fs
	pop %es
	pop %ds
	popa
	# iret



.data
	interruptnumber: .byte 0
