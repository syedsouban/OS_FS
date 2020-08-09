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

class InterruptManager;

class InterruptHandler {
	protected:
		uint8_t interruptNumber;
		InterruptManager* interruptManager;
		InterruptHandler(uint8_t interruptNumber, InterruptManager* interruptManager);
		~InterruptHandler();
	public:
		virtual uint32_t HandleInterrupt(uint32_t esp);
};

class InterruptManager {

friend class InterruptHandler;
protected:

	static InterruptManager* ActiveInterruptManager;
	InterruptHandler* handlers[256];

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

	Port8BitSlow picMasterCommandPort;
	Port8BitSlow picSlaveCommandPort;
	Port8BitSlow picMasterDataPort;
	Port8BitSlow picSlaveDataPort;

public:
	static uint32_t handleInterrupt(uint8_t interruptNumber,uint32_t esp);
	InterruptManager(GlobalDescriptorTable* gdt);
	~InterruptManager();
	void Activate();
	void Deactivate();
	
	static void handleInterruptRequest0x00();
    static void handleInterruptRequest0x01();
	static void IgnoreInterruptRequest();
	uint32_t doHandleInterrupt(uint8_t interruptNumber,uint32_t esp);
	// static void HandleInterruptRequest0x20();
	// static void HandleInterruptRequest0x21();
//	static void HandleInterruptRequest0x99();

//	static void HandleInterruptException();
};
#endif /* INTERRUPTS_H_ */

