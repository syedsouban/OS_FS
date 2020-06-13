#include "types.h"
#include "gdt.h"
#include "interrupts.h"
#include "printf.h"
typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors() {
	for (constructor *i = &start_ctors; i != &end_ctors; i++)
		(*i)();
}

extern "C" void kernelMain(const void *multiboot_structure,
		uint32_t /*multiboot_magic*/) {


	// printf("H");
		printf("OS Development is a challenging yet fun task!");
	GlobalDescriptorTable gdt;
	
//
		// printf("%d",(*base).GetBase());
//
	InterruptManager interruptsManager(&gdt);
//
//
	interruptsManager.Activate();

	while (1)
		;
}
