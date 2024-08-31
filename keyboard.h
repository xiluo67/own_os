#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "types.h"
#include "interrupts.h"
#include "port.h"

class KeyBoardDriver : public InterrupHandler
{
    Port8Bit dataport;
    Port8Bit commandport;

    public:
    KeyBoardDriver(InterruptManager* manager);
    ~KeyBoardDriver();
    virtual uint32_t HandleInterrupt(uint32_t esp);
};

#endif