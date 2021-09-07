# $@ = target file
# $< = first dependency
# $^ = all dependencies

# First rule is the one executed when no parameters are fed to the Makefile
# ld -m elf_i386 -l link.ld -o $@ -Ttext 0x1000 $^ --oformat binary

# bootsector, bootloader, stage 1 - sector 0
# bootloader, stage 2 - sector 1-8
# kernel, 32 bit - sector 9-?

INCL1="kernel/std"
INCL2="kernel/libkern32"
# 7 sectors
BOOTLOADER_SIZE=8191 # size of bootloader_s2.bin - 1
# 64 sectors
KERNEL32_SIZE=32767 # size of kernel.elf - 1
KERNEL="iso/monos-0_0_7-i386.bin"
KERNEL_IMG="iso/monos-0_0_7-i386.img"
CUR_DIR=$(shell pwd)

#/home/linuxbrew/.linuxbrew/Cellar/x86_64-elf-gcc/11.2.0/bin/x86_64-elf-gcc

all: build

#### object files
obj/bls2/obj/lkern_entry.o: boot/kernloader/lkern_entry.s
	nasm $< -f elf -o $@
obj/bls2/obj/load_kernel.o: boot/kernloader/load_kernel.c
	clang -Werror -nostdlib -fno-common -m32 -ffreestanding -fno-pie -I $(INCL1) -I $(INCL2) -c $< -o $@
### include all required asm files for stage2 bootloader libraries
### must manually add nasm for each file
bls2_lib_obj: kernel/libkern32/gdt_fun.s
	echo "\n\n"
	python3 compile_asm_files.py $^ "obj/bls2/obj"

obj/kernel/obj/kernel_entry.o: kernel/kernel_entry.s
	nasm $< -f elf -o $@
# dosent WORK!
obj/kernel/obj/kernel.o: kernel/kernel.c
	python3 compile_asm_dir.py "$(CUR_DIR)/kernel" "obj/kernel/obj"
	clang -Werror -nostdlib -fno-common -m32 -ffreestanding -fno-pie -I $(INCL1) -I $(INCL2) -c $< -o $@

### compiled binary
obj/bootsect/bin/bootloader.bin: boot/bootsect/bootloader.s 						# stage 1: bootloader (bootsect)
	nasm $< -f bin -o $@
obj/bls2/bin/bootloader_s2.bin: obj/bls2/obj/lkern_entry.o obj/bls2/obj/load_kernel.o obj/global/kern_const.o	# stage 2: bootloader (pmode)
	#ld -m elf_i386 -T boot/kernloader/link.ld $^ -o $@ --oformat binary 
	make bls2_lib_obj
	python3 link_bls2_obj.py $@ "$(CUR_DIR)/obj/bls2/obj" "$(CUR_DIR)/obj/global"
	dd if=/dev/zero of=$@ bs=1 count=1 seek=$(BOOTLOADER_SIZE)
obj/kernel/bin/kernel.elf: obj/kernel/obj/kernel_entry.o obj/kernel/obj/kernel.o obj/global/kern_const.o				# stage 3: kernel (pmode + paging, etc)	
	python3 link_kern_obj.py $@ "$(CUR_DIR)/obj/kernel/obj" "$(CUR_DIR)/obj/global"
	dd if=/dev/zero of=$@ bs=1 count=1 seek=$(KERNEL32_SIZE)
#	ld -m elf_i386 -nostdlib -T kernel/link.ld $^ -o $@ --oformat binary
#	ld -m elf_i386 -nostdlib -T kernel/link.ld obj/kernel/obj/* -o $@ --oformat binary

# global variables file
obj/global/kern_const.o: kern_const.c
	clang -Werror -nostdlib -fno-common -m32 -ffreestanding -fno-pie -c $< -o $@
	#cp $@ obj/kernel/obj
	#cp $@ obj/bls2/obj

### compile the disk image
$(KERNEL): obj/bootsect/bin/bootloader.bin obj/bls2/bin/bootloader_s2.bin obj/kernel/bin/kernel.elf
	touch $(KERNEL)
	cat $^ >> $(KERNEL)

run: $(KERNEL)
	qemu-system-i386 $<
build: $(KERNEL)
	dd if=/dev/zero of=$(KERNEL_IMG) bs=512 count=100
	dd conv=notrunc if=$(KERNEL) of=$(KERNEL_IMG) bs=512 seek=0
clean:
	rm obj/bls2/obj/* obj/bls2/bin/* obj/kernel/obj/* obj/kernel/bin/* obj/bootsect/obj/* obj/bootsect/bin/* obj/global/* iso/* > /dev/null 2>&1
	echo "\n\n"