#include "interrupts.h"

void printf(char* str);

InterrupHandler::InterrupHandler(uint8_t interruptNumber, InterruptManager* interrupManager)
{
    this->interruptNumber = interruptNumber;
    this->interrupManager = interrupManager;
    interrupManager->handlers[interruptNumber] = this;
};

InterrupHandler::~InterrupHandler()
{
    if (interrupManager->handlers[interruptNumber] == this)
        interrupManager->handlers[interruptNumber] = 0;
};

uint32_t InterrupHandler::HandleInterrupt(uint32_t esp)
{
    printf("base class handler handling");
    return esp;
};

InterruptManager::GateDescriptor InterruptManager::interuptDescriptorTable[256];
InterruptManager* InterruptManager::ActiveInterruptManager = 0;
void InterruptManager::SetInteruptDescriptprTableEntry
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
    interuptDescriptorTable[interuptNumber].access = IDT_DESC_PRESENT | ((DescriptorPrivilegeLevel&3) << 5) | DescriptorType;
    interuptDescriptorTable[interuptNumber].reserved = 0;

}

InterruptManager::InterruptManager(uint16_t hardwareInterruptOffset, GlobalDescriptorTable* globalDescriptorTable)
: picMasterCommand(0x20),
  picMasterData(0x21),
  picSlaveCommand(0xA0),
  picSlaveData(0xA1)
{
    this->hardwareInterruptOffset = hardwareInterruptOffset;
    uint32_t codeSegment = globalDescriptorTable->CodeSegmentSelector();
    const uint8_t IDT_INTERUPT_GATE = 0xE;
    
    for (uint8_t i = 255; i > 0; --i)
    {
        /* initialize the handler to be zero when start*/
        SetInteruptDescriptprTableEntry(i, codeSegment, &IgnoreInteruptRequest, 0, IDT_INTERUPT_GATE);
        handlers[i] = 0;
        /*Note: How to set the number of IDT_INTERUPT_GATE*/
    }
    SetInteruptDescriptprTableEntry(0, codeSegment, &IgnoreInteruptRequest, 0, IDT_INTERUPT_GATE);
    handlers[0] = 0;
    SetInteruptDescriptprTableEntry(0x00, codeSegment, &HandleException0x00, 0, IDT_INTERUPT_GATE);
    SetInteruptDescriptprTableEntry(0x01, codeSegment, &HandleException0x01, 0, IDT_INTERUPT_GATE);
    SetInteruptDescriptprTableEntry(0x02, codeSegment, &HandleException0x02, 0, IDT_INTERUPT_GATE);
    SetInteruptDescriptprTableEntry(0x03, codeSegment, &HandleException0x03, 0, IDT_INTERUPT_GATE);
    SetInteruptDescriptprTableEntry(0x04, codeSegment, &HandleException0x04, 0, IDT_INTERUPT_GATE);
    SetInteruptDescriptprTableEntry(0x05, codeSegment, &HandleException0x05, 0, IDT_INTERUPT_GATE);
    SetInteruptDescriptprTableEntry(0x06, codeSegment, &HandleException0x06, 0, IDT_INTERUPT_GATE);
    SetInteruptDescriptprTableEntry(0x07, codeSegment, &HandleException0x07, 0, IDT_INTERUPT_GATE);
    SetInteruptDescriptprTableEntry(0x08, codeSegment, &HandleException0x08, 0, IDT_INTERUPT_GATE);
    SetInteruptDescriptprTableEntry(0x09, codeSegment, &HandleException0x09, 0, IDT_INTERUPT_GATE);
    SetInteruptDescriptprTableEntry(0x0A, codeSegment, &HandleException0x0A, 0, IDT_INTERUPT_GATE);
    SetInteruptDescriptprTableEntry(0x0B, codeSegment, &HandleException0x0B, 0, IDT_INTERUPT_GATE);
    SetInteruptDescriptprTableEntry(0x0C, codeSegment, &HandleException0x0C, 0, IDT_INTERUPT_GATE);
    SetInteruptDescriptprTableEntry(0x0D, codeSegment, &HandleException0x0D, 0, IDT_INTERUPT_GATE);
    SetInteruptDescriptprTableEntry(0x0E, codeSegment, &HandleException0x0E, 0, IDT_INTERUPT_GATE);
    SetInteruptDescriptprTableEntry(0x0F, codeSegment, &HandleException0x0F, 0, IDT_INTERUPT_GATE);
    SetInteruptDescriptprTableEntry(0x10, codeSegment, &HandleException0x10, 0, IDT_INTERUPT_GATE);
    SetInteruptDescriptprTableEntry(0x11, codeSegment, &HandleException0x11, 0, IDT_INTERUPT_GATE);
    SetInteruptDescriptprTableEntry(0x12, codeSegment, &HandleException0x12, 0, IDT_INTERUPT_GATE);
    SetInteruptDescriptprTableEntry(0x13, codeSegment, &HandleException0x13, 0, IDT_INTERUPT_GATE);

    SetInteruptDescriptprTableEntry(hardwareInterruptOffset + 0x00, codeSegment, &HandleInteruptRequest0x00, 0, IDT_INTERUPT_GATE);
    SetInteruptDescriptprTableEntry(hardwareInterruptOffset + 0x01, codeSegment, &HandleInteruptRequest0x01, 0, IDT_INTERUPT_GATE);
    SetInteruptDescriptprTableEntry(hardwareInterruptOffset + 0x02, codeSegment, &HandleInteruptRequest0x02, 0, IDT_INTERUPT_GATE);
    SetInteruptDescriptprTableEntry(hardwareInterruptOffset + 0x03, codeSegment, &HandleInteruptRequest0x03, 0, IDT_INTERUPT_GATE);
    SetInteruptDescriptprTableEntry(hardwareInterruptOffset + 0x04, codeSegment, &HandleInteruptRequest0x04, 0, IDT_INTERUPT_GATE);
    SetInteruptDescriptprTableEntry(hardwareInterruptOffset + 0x05, codeSegment, &HandleInteruptRequest0x05, 0, IDT_INTERUPT_GATE);
    SetInteruptDescriptprTableEntry(hardwareInterruptOffset + 0x06, codeSegment, &HandleInteruptRequest0x06, 0, IDT_INTERUPT_GATE);
    SetInteruptDescriptprTableEntry(hardwareInterruptOffset + 0x07, codeSegment, &HandleInteruptRequest0x07, 0, IDT_INTERUPT_GATE);
    SetInteruptDescriptprTableEntry(hardwareInterruptOffset + 0x08, codeSegment, &HandleInteruptRequest0x08, 0, IDT_INTERUPT_GATE);
    SetInteruptDescriptprTableEntry(hardwareInterruptOffset + 0x09, codeSegment, &HandleInteruptRequest0x09, 0, IDT_INTERUPT_GATE);
    SetInteruptDescriptprTableEntry(hardwareInterruptOffset + 0x0A, codeSegment, &HandleInteruptRequest0x0A, 0, IDT_INTERUPT_GATE);
    SetInteruptDescriptprTableEntry(hardwareInterruptOffset + 0x0B, codeSegment, &HandleInteruptRequest0x0B, 0, IDT_INTERUPT_GATE);
    SetInteruptDescriptprTableEntry(hardwareInterruptOffset + 0x0C, codeSegment, &HandleInteruptRequest0x0C, 0, IDT_INTERUPT_GATE);
    SetInteruptDescriptprTableEntry(hardwareInterruptOffset + 0x0D, codeSegment, &HandleInteruptRequest0x0D, 0, IDT_INTERUPT_GATE);
    SetInteruptDescriptprTableEntry(hardwareInterruptOffset + 0x0E, codeSegment, &HandleInteruptRequest0x0E, 0, IDT_INTERUPT_GATE);
    SetInteruptDescriptprTableEntry(hardwareInterruptOffset + 0x0F, codeSegment, &HandleInteruptRequest0x0F, 0, IDT_INTERUPT_GATE);
    
    picMasterCommand.Write(0x11);
    picSlaveCommand.Write(0x11);

    // /*if master pic got the interup number, just add the 0x20, slave 28, each pic 8*/
    picMasterData.Write(0x20);
    picSlaveData.Write(0x28);

    picMasterData.Write(0x04); // mark pic as master
    picSlaveData.Write(0x02); //mark pic as slave

    // /*Note: search purpose for the 4 writes below*/
    picMasterData.Write(0x01);
    picSlaveData.Write(0x01);

    picMasterData.Write(0x00);
    picSlaveData.Write(0x00);
    
    interuptDescriptorTablePointer idt;

    idt.size = 256 * sizeof(GateDescriptor) - 1;
    idt.base = (uint32_t)interuptDescriptorTable;
    asm volatile("lidt %0" : : "m" (idt));
};

void InterruptManager::activate()
{
    uint32_t privilegeLevel;

    // Inline assembly to check the current privilege level
    asm volatile (
        "mov %%cs, %%ax;"   // Move the value of CS into AX
        "and $0x3, %%ax;"   // Mask the lower 2 bits to get the CPL
        "movzx %%ax, %0;"     // Move the result into the C++ variable
        : "=r" (privilegeLevel)  // Output operand
        :  // No input operands
        : "ax"  // Clobbered registers
    );

    // Check if the privilege level is 0 (Ring 0)
    if (privilegeLevel == 0){ 
        if(ActiveInterruptManager != 0) {
            ActiveInterruptManager->deactivate();
        }
        // Inline assembly to enable interrupts
        ActiveInterruptManager = this;
        asm volatile("sti");
    }
    else {
        printf("Not running in Ring 0, cannot execute sti!");
    }
};

void InterruptManager::deactivate()
{
    uint32_t privilegeLevel;

    // Inline assembly to check the current privilege level
    asm volatile (
        "mov %%cs, %%ax;"   // Move the value of CS into AX
        "and $0x3, %%ax;"   // Mask the lower 2 bits to get the CPL
        "movzx %%ax, %0;"     // Move the result into the C++ variable
        : "=r" (privilegeLevel)  // Output operand
        :  // No input operands
        : "ax"  // Clobbered registers
    );

    // Check if the privilege level is 0 (Ring 0)
    if (privilegeLevel == 0 && ActiveInterruptManager == this) {
        ActiveInterruptManager = 0;
        // Inline assembly to enable interrupts
        asm volatile("cli");
    } else {
        printf("Not running in Ring 0, cannot execute cli!");
    }
};

InterruptManager::~InterruptManager()
{
    deactivate();
};

uint16_t InterruptManager::HardwareInterruptOffset()
{
    return hardwareInterruptOffset;
};

uint32_t InterruptManager::DohandleInterupt(uint8_t interuptNumber, uint32_t esp)
{
    if (handlers[interuptNumber] != 0)
    {
        esp = handlers[interuptNumber]->HandleInterrupt(esp);
    }
    else if (interuptNumber != hardwareInterruptOffset)
    {
        char* foo = "INTERRUPT 0x00";
        char* hex = "0123456789ABCDEF";

        foo[12] = hex[(interuptNumber >> 4) & 0xF];
        foo[13] = hex[interuptNumber & 0xF];
        printf(foo);
    }

    // if(interuptNumber >= 0x20 && interuptNumber < 0x30)
    // {
    //     printf("hello?");
    //     picMasterCommand.Write(0x20);
    //     if (interuptNumber >= 0x28 && interuptNumber < 0x30)
    //     {
    //         picSlaveCommand.Write(0x20);
    //     }
    // }
    // hardware interrupts must be acknowledged
    if(hardwareInterruptOffset <= interuptNumber && interuptNumber < hardwareInterruptOffset+16)
    {
        picMasterCommand.Write(0x20);
        if(hardwareInterruptOffset + 8 <= interuptNumber)
            picSlaveCommand.Write(0x20);
    }
    return esp;
};

uint32_t InterruptManager::handleInterupt(uint8_t interuptNumber, uint32_t esp)
{
    if (ActiveInterruptManager != 0)
        ActiveInterruptManager->DohandleInterupt(interuptNumber, esp);
    return esp;
};