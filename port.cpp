#include "port.h"
Port::Port(uint16_t portnumber) {
	this->portnumber = portnumber;
}

Port::~Port() {
}

_8BitPort::_8BitPort(uint16_t portnumber) : Port(portnumber) {

}

_8BitPort::~_8BitPort() {
}

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
	// A Write followed by two jumps to waste time. -_-
	__asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (data), "Nd" (this->portnumber));
}

_16BitPort::_16BitPort(uint16_t portnumber) : Port(portnumber) {
}

_16BitPort::~_16BitPort() {
}

void _16BitPort::Write(uint16_t data) {
	__asm__ volatile("outw %0, %1" : : "a" (data), "Nd" (this->portnumber));
}
uint16_t _16BitPort::Read() {
	uint16_t result;
	__asm__ volatile("inw %1, %0" : "=a" (result) : "Nd" (this->portnumber));
	return result;
}

_32BitPort::_32BitPort(uint16_t portnumber) : Port(portnumber) {
}

_32BitPort::~_32BitPort() {
}

void _32BitPort::Write(uint32_t data) {
	__asm__ volatile("outl %0, %1" : : "a" (data), "Nd" (this->portnumber));
}
uint32_t _32BitPort::Read() {
	uint32_t result;
	__asm__ volatile("inl %1, %0" : "=a" (result) : "Nd" (this->portnumber));
	return result;
}