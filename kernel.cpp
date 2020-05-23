#include "types.h"
#include "gdt.h"
#include "interrupts.h"
#define HORIZONTAL 80
#define VERTICAL 25
void printf(char *str) {
	static uint16_t *VideoMemory = (uint16_t*) 0xb8000;

	static uint8_t x = 0, y = 0;

	for (int i = 0; str[i] != '\0'; ++i) {

		switch (str[i]) {
		case '\n':
			y++;
			x = 0;
			break;
		default:
			VideoMemory[HORIZONTAL * y + x] = (VideoMemory[HORIZONTAL * y + x] & 0xFFAB) | str[i];
			x++;
			break;
		}
		if (x >= HORIZONTAL) {
			x = 0;
			y++;
		}
		if (y >= VERTICAL) {
			for (int i = 0; i < HORIZONTAL; i++) {
				for (int j = 0; j < VERTICAL; j++) {
					VideoMemory[HORIZONTAL * y + x] = (VideoMemory[HORIZONTAL * y + x] & 0xFF00)
							| ' ';
				}
			}
			x = 0;
			y = 0;
		}

	}

}

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors() {
	for (constructor *i = &start_ctors; i != &end_ctors; i++)
		(*i)();
}

extern "C" void kernelMain(const void *multiboot_structure,
		uint32_t /*multiboot_magic*/) {

	GlobalDescriptorTable gdt;

	InterruptManager interruptsManager(&gdt);

	printf("Hello World from Syed OS\n");
		printf("OS Development is a challenging yet fun task :)!!!!!");

	interruptsManager.Activate();

	while (1)
		;
}
