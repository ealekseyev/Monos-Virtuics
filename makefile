# $@ = target file
# $< = first dependency
# $^ = all dependencies

# First rule is the one executed when no parameters are fed to the Makefile
# ld -m elf_i386 -l link.ld -o $@ -Ttext 0x1000 $^ --oformat binary

# bootsector, bootloader, stage 1 - sector 0
# bootloader, stage 2 - sector 1-8
# kernel, 32 bit - sector 9-?

INCL_STD="shared/libkern32std"
INCL_HARD32_SHARED="shared/libhardware32"
INCL_KERN="kernel/libkern32"
INCL_BLS2="boot/kernloader/libboot32"

# 32 sectors
BOOTLOADER_SIZE=16383 # size of bootloader_s2.bin - 1
# 64 sectors
KERNEL32_SIZE=32767 # size of kernel.elf - 1

KERNEL="iso/monos-0_2_0-i386.bin"
KERNEL_IMG="iso/monos-i386.img"
CUR_DIR=$(shell pwd)

#/home/linuxbrew/.linuxbrew/Cellar/x86_64-elf-gcc/11.2.0/bin/x86_64-elf-gcc
all:	build

#### object files
obj/bls2/obj/kernloader_entry.o: boot/kernloader/kernloader_entry.s
	nasm $< -f elf -o $@
obj/bls2/obj/kernloader.o: boot/kernloader/kernloader.c
	python3 compile_asm_dir.py "$(CUR_DIR)/boot/kernloader/libboot32" "obj/bls2/obj" #> /dev/null
	python3 compile_asm_dir.py "$(CUR_DIR)/shared/libhardware32/" "obj/bls2/obj" #> /dev/null
	clang -Werror -nostdlib -fno-common -m32 -ffreestanding -fno-pie -I $(INCL_STD) -I $(INCL_BLS2) -I $(INCL_HARD32_SHARED) -c $< -o $@
### include all required asm files for stage2 bootloader libraries
### must manually add nasm for each file
### TODO: rewrite compile_asm_* sysargv use intel syntax like link_*_*
#bls2_lib_obj: shared/libhardware32/intel/idt_routing.s shared/libhardware32/intel/gdt_fun.s shared/libhardware32/intel/paging_fun.s
#	python3 compile_asm_files.py $^ "obj/bls2/obj" > /dev/null

obj/kernel/obj/kernel_entry.o: kernel/kernel_entry.s
	nasm $< -f elf -o $@
# dosent WORK!
obj/kernel/obj/kernel.o: kernel/kernel.c
	python3 compile_asm_dir.py "$(CUR_DIR)/kernel/libkern32" "obj/kernel/obj" #> /dev/null
	python3 compile_asm_dir.py "$(CUR_DIR)/shared/libhardware32" "obj/kernel/obj" #> /dev/null
	#python3 compile_asm_dir.py "$(CUR_DIR)/kernel" "obj/kernel/obj" #> /dev/null
	clang -Werror -nostdlib -nodefaultlibs -fno-common -m32 -ffreestanding -fno-pie -I $(INCL_STD) -I $(INCL_KERN) -I $(INCL_HARD32_SHARED) -c $< -o $@

### compiled binary
obj/bootsect/bin/bootloader.bin: boot/bootsect/bootloader.s 						# stage 1: bootloader (bootsect)
	echo "\033[31mBUILDING bootloader.bin\033[0m"
	nasm $< -f bin -o $@
obj/bls2/bin/bootloader_s2.bin: obj/bls2/obj/kernloader_entry.o obj/bls2/obj/kernloader.o obj/global/kern_const.o	# stage 2: bootloader (pmode)
	echo "\033[31mBUILDING bootloader_s2.bin\033[0m"
	#ld -m elf_i386 -T boot/kernloader/link.ld $^ -o $@ --oformat binary 
	#make bls2_lib_obj
	python3 link_bls2_obj.py $@ "$(CUR_DIR)/obj/bls2/obj" "$(CUR_DIR)/obj/global" > /dev/null
	printf "\033[32mBin Size: "
	du --apparent-size --block-size=1 $@
	printf "Padded:   "
	dd if=/dev/zero of=$@ bs=1 count=1 seek=$(BOOTLOADER_SIZE) status=none
	du --apparent-size --block-size=1 $@
	printf "\033[0m"
obj/kernel/bin/kernel.elf: obj/kernel/obj/kernel_entry.o obj/kernel/obj/kernel.o obj/global/kern_const.o			# stage 3: kernel (pmode + paging, etc)	
	echo "\033[31mBUILDING kernel.elf\033[0m"
	python3 link_kern_obj.py $@ "$(CUR_DIR)/obj/kernel/obj" "$(CUR_DIR)/obj/global" > /dev/null
	printf "\033[32mBin Size: "
	du --apparent-size --block-size=1 $@
	printf "Padded:   "
	dd if=/dev/zero of=$@ bs=1 count=1 seek=$(KERNEL32_SIZE) status=none
	du --apparent-size --block-size=1 $@
	printf "\033[0m"
#	ld -m elf_i386 -nostdlib -T kernel/link.ld $^ -o $@ --oformat binary
#	ld -m elf_i386 -nostdlib -T kernel/link.ld obj/kernel/obj/* -o $@ --oformat binary

# global variables file
obj/global/kern_const.o: kern_const.c
	clang -Werror -nostdlib -nodefaultlibs -fno-common -m32 -ffreestanding -fno-pie -c $< -o $@
	#cp $@ obj/kernel/obj
	#cp $@ obj/bls2/obj

### compile the disk image
$(KERNEL): obj/bootsect/bin/bootloader.bin obj/bls2/bin/bootloader_s2.bin obj/kernel/bin/kernel.elf
	echo "\033[31mBUILDING $(KERNEL)\033[0m"
	touch $(KERNEL)
	cat $^ >> $(KERNEL)

run:
	qemu-system-i386 $(KERNEL) -s -monitor stdio
build: $(KERNEL)
	dd if=/dev/zero of=$(KERNEL_IMG) bs=512 count=2048
	dd conv=notrunc if=$(KERNEL) of=$(KERNEL_IMG) bs=512 seek=0
clean:
	rm obj/bls2/obj/* obj/bls2/bin/* obj/kernel/obj/* obj/kernel/bin/* obj/bootsect/obj/* obj/bootsect/bin/* obj/global/* iso/* > /dev/null 2>&1
	echo "\n\n"	