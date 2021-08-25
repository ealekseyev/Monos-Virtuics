# $@ = target file
# $< = first dependency
# $^ = all dependencies

# First rule is the one executed when no parameters are fed to the Makefile
all: run

kernel.bin: kernel_entry.o kernel.o
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary
kernel_entry.o: kernel/kernel_entry.s
	nasm $< -f elf -o $@
kernel.o: kernel/kernel.c
	gcc -m32 -ffreestanding -c $< -o $@
bootloader.bin: boot/bootloader.s
	nasm $< -f bin -o $@
os-image.bin: bootloader.bin kernel.bin
	cat $^ > $@
run: os-image.bin
	qemu-system-i386 -fda $<
clean:
	$(RM) *.bin *.o *.dis