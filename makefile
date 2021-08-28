# $@ = target file
# $< = first dependency
# $^ = all dependencies

# First rule is the one executed when no parameters are fed to the Makefile
# ld -m elf_i386 -l link.ld -o $@ -Ttext 0x1000 $^ --oformat binary
INCLUDE="kernel/std"
all: run

obj/bin/kernel.bin: obj/kernel_entry.o obj/kernel.o
	ld -m elf_i386 -T link.ld $^ -o $@ --oformat binary
obj/kernel_entry.o: kernel/kernel_entry.s
	nasm $< -f elf -o $@
obj/kernel.o: kernel/kernel.c
	gcc -m32 -ffreestanding -fno-pie -I $(INCLUDE) -c $< -o $@
obj/bin/bootloader.bin: boot/bootloader.s
	nasm $< -f bin -o $@
$(KERNEL): obj/bin/bootloader.bin obj/bin/kernel.bin
	touch $(KERNEL)
	cat $^ >> $(KERNEL)

run: $(KERNEL)
	qemu-system-i386 $<
clean:
	rm obj/bin/*.bin obj/*.o *.bin