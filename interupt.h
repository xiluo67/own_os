#ifndef __INTERUPTS_H
#define __INTERUPTS_H

#include "types.h"
#include "port.h"
#include "gdt.h"

class InteruptManager
{
    protected:
        /* interupts Descriptor Table */
        struct GateDescriptor
        {
            uint16_t handlerAddressLowBits;
            uint16_t gdt_codeSegmentSelector;
            uint8_t reserved;
            uint8_t access;
            uint16_t handlerAddressHighBits;

        }__attribute__((packed));
    
        static GateDescriptor interuptDescriptorTable[256];

        struct interuptDescriptorTablePointer
        {
            uint16_t size;
            uint32_t base; //address of the table and the size

        }__attribute__((packed)); 
        static void SetInteruptDescriptprTableEntry(
            uint8_t interuptNumber,
            uint16_t codeSegmentSeletorOffset,
            void (*handler)(),
            uint8_t DescriptorPrivilegeLevel,
            uint8_t DescriptorType

        );

    Port8BitSlow picMasterCommand;
    Port8BitSlow picMasterData;
    Port8BitSlow picSlaveCommand;
    Port8BitSlow picSlaveData;

    public:
    InteruptManager(GlobalDescriptorTable *gdt);
    ~InteruptManager();

    /*Note: How to put the exact compiled function into assembly code?*/
    static void IgnoreInteruptRequest(); 
    
    static uint32_t handleInterupt(uint8_t interuptNumber, uint32_t esp);

    /* Create the interupt handler */
    /*Note: How to put the exact compiled function into assembly code?*/
    static void HandleInteruptRequest0x00(); //keyboard interupt
    static void HandleInteruptRequest0x01(); //timer interupt

    void activate();

};

#endif