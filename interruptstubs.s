.set IRQ_BASE, 0x20

.section .text

.extern _ZN16InterruptManager15handleInterruptEj
.global _ZN16InterruptManager22IgnoreInterruptRequestEv

_ZN16InterruptManager22IgnoreInterruptRequestEv:
	pushl %esp
	call _ZN16InterruptManager15handleInterruptEj
	mov %eax, %esp
	iret