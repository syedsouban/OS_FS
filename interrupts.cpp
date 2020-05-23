/*
 * interrupts.cpp
 *
 *  Created on: 02-May-2020
 *      Author: saubaan
 */

#include "interrupts.h"

void printf(char *);

InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];

uint32_t InterruptManager::handleInterrupt(uint8_t interruptNumber,
		uint32_t esp) {
	printf("INTERRUPT");
	return esp;
}

//InterruptManager::~InterruptManager() {
//	// TODO Auto-generated destructor stub
//}

//void InterruptManager::HandleInterruptRequest0x00() {
//	printf("Int");
//}

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
	for (uint8_t i = 0;  i < 256; ++ i) {
		SetInterruptDescriptorTableEntry(i,CodeSegment,&IgnoreInterruptRequest,0,IDT_INTERRUPT_GATE);
	}
	SetInterruptDescriptorTableEntry(0x20,CodeSegment,&HandleInterruptRequest0x00,0,IDT_INTERRUPT_GATE);
	SetInterruptDescriptorTableEntry(0x21,CodeSegment,&HandleInterruptRequest0x01,0,IDT_INTERRUPT_GATE);


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

void InterruptManager::Activate() {

	__asm__("sti");

}
//
//void InterruptManager::HandleInterruptRequest0x01() {
//	printf("Int");
//}
