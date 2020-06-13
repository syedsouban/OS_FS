#ifndef __GDT_H
#define __GDT_H

#include "types.h"
void doNothing();
class GlobalDescriptorTable
{
private:
    /* data */
public:

    class SegmentDescriptor
    {
        private:
            uint16_t limit_lo;
            uint16_t base_lo;
            uint8_t base_hi;
            uint8_t type;
            uint8_t flags_limit_hi;
            uint8_t base_vhi;
        public:
            SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type);
            
            uint32_t GetBase();
            uint32_t GetLimit();
                
            
    } __attribute__((packed));

    SegmentDescriptor nullSegmentSelector;
    SegmentDescriptor unusedSegmentSelector;
    SegmentDescriptor codeSegmentSelector;
    SegmentDescriptor dataSegmentSelector;


    GlobalDescriptorTable(/* args */);
    ~GlobalDescriptorTable();
    uint16_t GetDataSegmentOffset();
    uint16_t GetCodeSegmentOffset();
}__attribute__((packed));



#endif
