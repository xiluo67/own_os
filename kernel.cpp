#include "types.h"
#include "gdt.h"
#include "interrupts.h"
void printf(char* str)
{
	static uint16_t* VideoMemeroy = (uint16_t*)0xb8000;

	static uint8_t x = 0, y = 0;


	for (int i=0; str[i]!= '\0'; i++)
	{
		switch (str[i])
		{
		case '\n':
			/* start a new line */
			y++;
			x=0;
			break;
		
		default:
			VideoMemeroy[80*y + x] = (VideoMemeroy[80*y + x] & 0xFF00) | str[i];
			x++;
			break;
		}

		if(x >= 80)
		{
			y++;
			x = 0;
		}

		if (y >= 25)
		{
			for (y = 0; y < 25; y++)
			{
				for(x = 0; x < 80; x++)
				{
					VideoMemeroy[80*y + x] = (VideoMemeroy[80*y + x] & 0xFF00) | ' ';
				}
			}
		}
	}
}

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;

extern "C" void callConstructors()
{
	for (constructor* i = &start_ctors; i != &end_ctors; i++)
	{
		(*i)();
	}
}

extern "C" void kernelMain(const void* multiboot_structure, uint32_t magicnumber)
{
	// printf("Hello World!---------------------\n");
	GlobalDescriptorTable gdt;
	InterruptManager interupts(0x20, &gdt);
	/*Instaniate the hardware*/
	/*the idt should ready to use*/
	
	interupts.activate();
	while(1);

}
