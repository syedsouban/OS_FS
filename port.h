#include "types.h"
#ifndef __PORT_H
#define __PORT_H



class Port
{
    protected:
        uint16_t portnumber;
        Port(uint16_t portnumber);
        ~Port();
};


class _8BitPort : public Port
{
    public:
        _8BitPort(uint16_t portnumber);
        ~ _8BitPort();
        virtual void Write(uint8_t data);
        virtual uint8_t Read(); 
};

class _8BitPortSlow : public Port
{
    public:
        _8BitPortSlow(uint16_t portnumber);
        ~ _8BitPortSlow();
        virtual void Write(uint8_t data);
};

class _16BitPort : public Port
{
    public:
        _16BitPort(uint16_t portnumber);
        ~ _16BitPort(); // @suppress("Class has a virtual method and non-virtual destructor")
        virtual void Write(uint16_t data);
        virtual uint16_t Read();
};




class _32BitPort : public Port
{
    public:
        _32BitPort(uint16_t portnumber);
        ~ _32BitPort();
        virtual void Write(uint32_t data);
        virtual uint32_t Read();
};

#endif
