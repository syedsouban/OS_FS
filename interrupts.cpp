#include "interrupts.h"

void printf(char *);

InterruptHandler::InterruptHandler(uint8_t interruptNumber,InterruptManager* interruptManager)
{
	this->interruptNumber = interruptNumber;
	this->interruptManager = interruptManager;
	interruptManager->handlers[interruptNumber] = this;
}

uint32_t InterruptHandler::HandleInterrupt(uint32_t esp) {
	return esp;
}

InterruptHandler::~InterruptHandler() {
	if(interruptManager->handlers[interruptNumber] == this)
		interruptManager->handlers[interruptNumber] = 0;
}

InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];

InterruptManager* InterruptManager::ActiveInterruptManager = 0;

void InterruptManager::SetInterruptDescriptorTableEntry(uint8_t interruptNumber,
		uint16_t csSelectorOffset, void (*handler)(),
		uint8_t DescriptorPriviligeLevel, uint8_t DescriptorType) {
	const uint8_t IDT_DESC_PRESENT = 0x80;
	interruptDescriptorTable[interruptNumber].handlerAddressLowBits = ((uint32_t)handler) & 0xFFFF;
	interruptDescriptorTable[interruptNumber].handlerAddressHighBits = ((uint32_t)handler >> 16) & 0xFFFF;
	interruptDescriptorTable[interruptNumber].gdt_cs_selector = csSelectorOffset;
	interruptDescriptorTable[interruptNumber].access = IDT_DESC_PRESENT | DescriptorType | ((DescriptorPriviligeLevel&3)<<5);
	interruptDescriptorTable[interruptNumber].reserved = 0;
}

InterruptManager::InterruptManager(GlobalDescriptorTable *gdt)
:picMasterCommandPort(0x20),
 picMasterDataPort(0x21),
 picSlaveCommandPort(0xA0),
 picSlaveDataPort(0xA1)
	{

	uint8_t CodeSegment = gdt->GetCodeSegmentOffset();
	const uint8_t IDT_INTERRUPT_GATE = 0xE;
	for (uint16_t i = 0;  i < 256; ++ i) {
		handlers[i] = 0;
		SetInterruptDescriptorTableEntry(i,CodeSegment,&IgnoreInterruptRequest,0,IDT_INTERRUPT_GATE);
	}
	SetInterruptDescriptorTableEntry(0x20,CodeSegment,&handleInterruptRequest0x00,0,IDT_INTERRUPT_GATE);
	SetInterruptDescriptorTableEntry(0x21,CodeSegment,&handleInterruptRequest0x01,0,IDT_INTERRUPT_GATE);

	picMasterCommandPort.Write(0x11);
	picSlaveCommandPort.Write(0x11);

	picMasterDataPort.Write(0x20);
	picSlaveDataPort.Write(0x28);

	picMasterDataPort.Write(0x04);
	picSlaveDataPort.Write(0x02);

	picMasterDataPort.Write(0x01);
	picSlaveDataPort.Write(0x01);

	picMasterDataPort.Write(0x00);
	picSlaveDataPort.Write(0x00);


	interruptDescriptorTablePointer idt;
	idt.size = 256*sizeof(GateDescriptor) - 1;
	idt.base = (uint32_t)interruptDescriptorTable;
	__asm__ volatile("lidt %0" : : "m" (idt));
}
void InterruptManager::Activate()
{
	if(ActiveInterruptManager != 0)
		ActiveInterruptManager->Deactivate();
	ActiveInterruptManager = this;
	asm("sti");
}

void InterruptManager::Deactivate()
{
	if(ActiveInterruptManager == this) {
		ActiveInterruptManager = 0;
		asm("cli");
	}
}

uint32_t InterruptManager::handleInterrupt(uint8_t interruptNumber,uint32_t esp) {
	if(ActiveInterruptManager!=0) {
		return ActiveInterruptManager->doHandleInterrupt(interruptNumber,esp);
	}
	return esp;
}

uint32_t InterruptManager::doHandleInterrupt(uint8_t interruptNumber,uint32_t esp) {

	if(handlers[interruptNumber]!=0) {
		esp = handlers[interruptNumber]->HandleInterrupt(esp);
	}
	else if(interruptNumber!=0x20) {
		char* foo = "Unhandled Interrupt 0x00";
		char* hex = "0123456789ABCDEF";
		foo[22] = hex[(interruptNumber >> 4) & 0x0F];
		foo[23] = hex[interruptNumber & 0x0F];
		printf(foo);
	}
	
	if(0x20 <= interruptNumber  && 0x30 > interruptNumber) {
		// printf("Inside 0x20 to 0x30");
		picMasterCommandPort.Write(0x20);
		if(0x28 <= interruptNumber) {
			// printf("Inside >=0x28");
			picSlaveCommandPort.Write(0x20);
		}
		
	}
	
	return esp;
}