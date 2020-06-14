#include "port.h"
Port::Port(uint16_t portnumber) {
	this->portnumber = portnumber;
}
Port::~Port() { }
_8BitPort::_8BitPort(uint16_t portnumber) : Port(portnumber) { }
_8BitPort::~_8BitPort() { }

void _8BitPort::Write(uint8_t data) {
	__asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (portnumber));
}

uint8_t _8BitPort::Read() {
	uint8_t result;
	__asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (portnumber) );
	return result;
}

_8BitPortSlow::_8BitPortSlow(uint16_t port_number) : Port(portnumber) { }

_8BitPortSlow::~_8BitPortSlow() {}

void _8BitPortSlow::Write(uint8_t data) {
	__asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (data), "Nd" (this->portnumber));
}