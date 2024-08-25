#include "interupt.h"

void printf(char* string);
InteruptManager::GateDescriptor InteruptManager::interuptDescriptorTable[256];

void InteruptManager::SetInteruptDescriptprTableEntry
/* Set the entry of InteruptDescriptorTable */
(
            uint8_t interuptNumber,
            uint16_t codeSegmentSeletorOffset,
            void (*handler)(),
            uint8_t DescriptorPrivilegeLevel,
            uint8_t DescriptorType)
{
    /*Note: how the magic number 0x80 comes?*/
    const uint8_t IDT_DESC_PRESENT = 0x80;
    interuptDescriptorTable[interuptNumber].handlerAddressLowBits = ((uint32_t)handler) & 0xFFFF; //keep the lowbits of handler address;
    interuptDescriptorTable[interuptNumber].handlerAddressHighBits = (((uint32_t)handler) >> 16) & 0xFFFF; //keep the high 16 bits of handler address;;
    interuptDescriptorTable[interuptNumber].gdt_codeSegmentSelector = codeSegmentSeletorOffset;
    interuptDescriptorTable[interuptNumber].access = IDT_DESC_PRESENT | DescriptorType | ((DescriptorPrivilegeLevel&3) << 5);
    interuptDescriptorTable[interuptNumber].reserved = 0;

}

InteruptManager::InteruptManager(GlobalDescriptorTable *gdt)
: picMasterCommand(0x20),
  picMasterData(0x20),
  picSlaveCommand(0xA0),
  picSlaveData(0xA1)
{
    uint16_t codeSegment = gdt->CodeSegmentSelector();
    const uint8_t IDT_INTERUPT_GATE = 0xE;

    for (uint16_t i = 0; i < 256; i++)
    {
        SetInteruptDescriptprTableEntry(i, codeSegment, &IgnoreInteruptRequest, 0, IDT_INTERUPT_GATE);
        /*Note: How to set the number of IDT_INTERUPT_GATE*/
    }
    SetInteruptDescriptprTableEntry(0x20, codeSegment, &HandleInteruptRequest0x00, 0, IDT_INTERUPT_GATE);
    SetInteruptDescriptprTableEntry(0x21, codeSegment, &HandleInteruptRequest0x01, 0, IDT_INTERUPT_GATE);
    
    picMasterCommand.Write(0x11);
    picSlaveCommand.Write(0x11);

    /*if master pic got the interup number, just add the 0x20, slave 28, each pic 8*/
    picMasterData.Write(0x20);
    picSlaveData.Write(0x28);

    picMasterData.Write(0x04); // mark pic as master
    picSlaveData.Write(0x02); //mark pic as slave

    /*Note: search purpose for the 4 writes below*/
    picMasterData.Write(0x01);
    picSlaveData.Write(0x01);

    picMasterData.Write(0x00);
    picSlaveData.Write(0x00);
    
    interuptDescriptorTablePointer idt;

    idt.size = 256 * sizeof(GateDescriptor) - 1;
    idt.base = (uint32_t)&interuptDescriptorTable;
    asm volatile("lidt %0" : : "m" (idt));
};

void InteruptManager::activate()
{
    asm("sti");
};

InteruptManager::~InteruptManager()
{

};

uint32_t InteruptManager::handleInterupt(uint8_t interuptNumber, uint32_t esp)
{
    printf("interupt");
    return esp;
};