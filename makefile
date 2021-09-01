# $@ = target file
# $< = first dependency
# $^ = all dependencies

# First rule is the one executed when no parameters are fed to the Makefile
# ld -m elf_i386 -l link.ld -o $@ -Ttext 0x1000 $^ --oformat binary
INCL1="kernel/std"
INCL2="kernel/libkern32"
BOOTLOADER_SIZE=16383 # size of bootloader_s2.bin - 1
KERNEL="iso/monos-0_0_1-i386.bin"
KERNEL_IMG="iso/monos-0_0_1-i386.img"

all: build

#### object files
obj/lkern_entry.o: boot/kernloader/lkern_entry.s
	nasm $< -f elf -o $@
obj/load_kernel.o: boot/kernloader/load_kernel.c
	/home/linuxbrew/.linuxbrew/Cellar/x86_64-elf-gcc/11.2.0/bin/x86_64-elf-gcc -nostdlib -fno-common -m32 -ffreestanding -fno-pie -I $(INCL1) -I $(INCL2) -c $< -o $@
obj/kernel_entry.o: kernel/kernel_entry.s
	nasm $< -f elf -o $@
obj/kernel.o: kernel/kernel.c
	/home/linuxbrew/.linuxbrew/Cellar/x86_64-elf-gcc/11.2.0/bin/x86_64-elf-gcc -nostdlib -fno-common -m32 -ffreestanding -fno-pie -I $(INCL1) -I $(INCL2) -c $< -o $@

### compiled binary
obj/bin/bootloader.bin: boot/bootloader.s 						# stage 1: bootloader (bootsect)
	nasm $< -f bin -o $@
obj/bin/bootloader_s2.bin: obj/lkern_entry.o obj/load_kernel.o	# stage 2: bootloader (pmode)
	ld -m elf_i386 -T boot/kernloader/link.ld $^ -o $@ --oformat binary 
	dd if=/dev/zero of=$@ bs=1 count=1 seek=$(BOOTLOADER_SIZE)
obj/bin/kernel.bin: obj/kernel_entry.o obj/kernel.o				# stage 3: kernel (pmode + paging, etc)	
	ld -m elf_i386 -T kernel/link.ld $^ -o $@ --oformat binary

### compile the disk image
$(KERNEL): obj/bin/bootloader.bin obj/bin/bootloader_s2.bin #obj/bin/kernel.bin 
	touch $(KERNEL)
	cat $^ >> $(KERNEL)

iso: 
	iat iso/$(KERNEL) iso/generic-i386.iso

run: $(KERNEL)
	qemu-system-i386 $<
build: $(KERNEL)
	dd if=/dev/zero of=$(KERNEL_IMG) bs=512 count=100
	dd conv=notrunc if=$(KERNEL) of=$(KERNEL_IMG) bs=512 seek=0
clean:
	rm obj/bin/*.bin obj/*.o iso/*.bin > /dev/null