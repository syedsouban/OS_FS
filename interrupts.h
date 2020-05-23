/*
 * interrupts.h
 *
 *  Created on: 02-May-2020
 *      Author: saubaan
 */

#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#include "types.h"
#include "gdt.h"
#include "port.h"



class InterruptManager {


protected:
	struct GateDescriptor {
		uint16_t handlerAddressLowBits;
		uint16_t gdt_cs_selector;
		uint8_t reserved;
		uint8_t access;
		uint16_t handlerAddressHighBits;
	} __attribute__((packed));

	static GateDescriptor interruptDescriptorTable[256];

	struct interruptDescriptorTablePointer
	{
		uint16_t size;
		uint32_t base;
	} __attribute__((packed));

	static void SetInterruptDescriptorTableEntry(uint8_t interruptNumber, uint16_t csSelectorOffset,void (*handler)(),
			uint8_t DescriptorPriviligeLevel, uint8_t DescriptorType);

	_8BitPortSlow picMasterCommandPort;
	_8BitPortSlow picSlaveCommandPort;
	_8BitPortSlow picMasterDataPort;
	_8BitPortSlow picSlaveDataPort;

public:
	static uint32_t handleInterrupt(uint8_t interruptNumber, uint32_t esp);
	InterruptManager(GlobalDescriptorTable* gdt);
//	~InterruptManager();
	void Activate();
	static void IgnoreInterruptRequest();
	static void HandleInterruptRequest0x00();
	static void HandleInterruptRequest0x01();

//	static void HandleInterruptException();
};
#endif /* INTERRUPTS_H_ */

