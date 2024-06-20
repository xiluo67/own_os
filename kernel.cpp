void printf(char* str)
{
	static unsigned short* VideoMemeroy = (unsigned short*)0xb8000;

	for (int i-0; str[i]!= '\0'; i++)
	{
		VideoMemory[i] = (VideoMemory[i] & 0xFF00) | str[i];
	}
}

typedef void (*constructor)();
extern 'C' constructor star_ctors;
extern 'C' constructor end_ctors;

extern 'C' void Callconstructors()
{
	for (constructor* i = &start_stors; i != end_stors; i++)
	{
		(*i)();
	}
}

extern 'C' void kernelMain(const void* multiboost_structure, unsigned_int)
{
	printf("Hello World!---------------------")
	while(1);

}
