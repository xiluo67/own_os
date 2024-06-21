GPPARAMS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-excaptions -fno-leading-underscore
ASPARAMS = --32
LDPARAMS = melf_i386

objects = loader.o kernel.o


%.o: %.cpp
	g++ $($GPPARAMS) -o %@ -c $<

%.o: %.s
	as $(ASPARAMS) -o %@ $<

myhernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

install: mykernel.bin
	sudo cp $< /boot/mykernel.bin
