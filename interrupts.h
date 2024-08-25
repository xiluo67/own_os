#ifndef __INTERUPTS_H
#define __INTERUPTS_H

#include "types.h"
#include "port.h"
#include "gdt.h"

class InterruptManager
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
        uint16_t hardwareInterruptOffset;

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

        /*Note: How to put the exact compiled function into assembly code?*/
        static void IgnoreInteruptRequest(); 
        static uint32_t handleInterupt(uint8_t interuptNumber, uint32_t esp);

        /* Create the interupt handler */
        /*Note: How to put the exact compiled function into assembly code?*/
        static void HandleInteruptRequest0x00(); //keyboard interupt
        static void HandleInteruptRequest0x01(); //timer interupt
        static void HandleInteruptRequest0x02();
        static void HandleInteruptRequest0x03();
        static void HandleInteruptRequest0x04();
        static void HandleInteruptRequest0x05();
        static void HandleInteruptRequest0x06();
        static void HandleInteruptRequest0x07();
        static void HandleInteruptRequest0x08();
        static void HandleInteruptRequest0x09();
        static void HandleInteruptRequest0x0A();
        static void HandleInteruptRequest0x0B();
        static void HandleInteruptRequest0x0C();
        static void HandleInteruptRequest0x0D();
        static void HandleInteruptRequest0x0E();
        static void HandleInteruptRequest0x0F();
        static void HandleInteruptRequest0x31();


        static void HandleException0x00();
        static void HandleException0x01();
        static void HandleException0x02();
        static void HandleException0x03();
        static void HandleException0x04();
        static void HandleException0x05();
        static void HandleException0x06();
        static void HandleException0x07();
        static void HandleException0x08();
        static void HandleException0x09();
        static void HandleException0x0A();
        static void HandleException0x0B();
        static void HandleException0x0C();
        static void HandleException0x0D();
        static void HandleException0x0E();
        static void HandleException0x0F();
        static void HandleException0x10();
        static void HandleException0x11();
        static void HandleException0x12();
        static void HandleException0x13();
        Port8BitSlow picMasterCommand;
        Port8BitSlow picMasterData;
        Port8BitSlow picSlaveCommand;
        Port8BitSlow picSlaveData;

    public:
        InterruptManager(uint16_t hardwareInterruptOffset, GlobalDescriptorTable* globalDescriptorTable);
        ~InterruptManager();
    
        uint16_t HardwareInterruptOffset();
        void activate();

};

#endif