#include "gdt.h"
#include "printf.h"

struct GdtDescriptionStructure {
	uint16_t sizeof_gdt;
	uint32_t addressof_gdt;
} __attribute__((packed));

GlobalDescriptorTable::GlobalDescriptorTable()
: nullSegmentSelector(0,0,0),
unusedSegmentSelector(0,0,0),
codeSegmentSelector(0,64*1024*1024,0x9A), //here code and data segment are there in same segment
dataSegmentSelector(0,64*1024*1024,0x92)
{

    uint32_t i[2];
    i[1] = (uint32_t)this;
    i[0] = sizeof(GlobalDescriptorTable) << 16;
    asm volatile("lgdt (%0)": :"p" (((uint8_t *) i)+2));

	// printf("gdt constructor called\n");
    // volatile static GdtDescriptionStructure gdt_locator;
	// gdt_locator.addressof_gdt = (uint32_t) this;
    // gdt_locator.sizeof_gdt = sizeof(GlobalDescriptorTable) - 1;
	/*asm volatile ("lgdt (%0)": :"m" (gdt_locator)); */
}

void doNothing() {
    int a = 2;
}

GlobalDescriptorTable::~GlobalDescriptorTable()
{

}

//offset of datasegmentselector
//my understanding of the offset is that it indicates the position of datasegmentselector entry in the global descriptor table
uint16_t GlobalDescriptorTable::GetDataSegmentOffset()
{
    return (uint8_t*)&dataSegmentSelector - (uint8_t*)this;
}

uint16_t GlobalDescriptorTable::GetCodeSegmentOffset()
{
    return (uint8_t*)&codeSegmentSelector - (uint8_t*)this;
}

GlobalDescriptorTable::SegmentDescriptor::SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t access_byte)
{
	// printf("setting up segment descriptor\n");
    uint8_t* target = (uint8_t*)this;
    if(limit<=65536)
    {
        target[6]=0x40;
    } 
    else
    {
        if((limit&0xFFF)!=0xFFF)
            limit = (limit>>12)-1;
        else
            limit = limit>>12;
        target[6] = 0xC0;           
    }
    target[0] = limit & 0xFF;
    target[1] = (limit>>8) & 0xFF;
    target[6] |= (limit>>16) & 0xF; 

    target[2] = base & 0xFF;
    target[3] = (base>>8) & 0xFF;
    target[4] = (base>>16) & 0xFF;
    target[7] = (base>>24) && 0xFF;

    target[5] = access_byte;
}

uint32_t GlobalDescriptorTable::SegmentDescriptor::GetBase()
{
    uint8_t* target = (uint8_t*)this;

    uint32_t base = target[7];
    base = (base<<8) + target[4];
    base = (base<<8) + target[3];
    base = (base<<8) + target[2];
    return base;
}

uint32_t GlobalDescriptorTable::SegmentDescriptor::GetLimit()
{
    uint8_t* target = (uint8_t*)this;
    uint32_t limit = target[6] & 0x0F;
    limit = (limit<<8) + target[1];
    limit = (limit<<8) + target[0];
    if((target[6] & 0xC0) == 0xC0)
        limit = (limit<<12) | 0xFFF;
    return limit;
}
