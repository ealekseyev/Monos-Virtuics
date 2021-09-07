import os, sys
# compile kernel asm's
file_list = ""
for dir in sys.argv[2::]:
    for root, dirs, files in os.walk(dir):
        for file in files:
            if file.endswith(".o"):
                file_list += os.path.join(root, file) + " "
print("\n\nld -m elf_i386 -nostdlib -T kernel/link.ld {} -o {} --oformat binary".format(file_list, sys.argv[1]))
os.system("ld -m elf_i386 -nostdlib -T kernel/link.ld {} -o {} --oformat binary".format(file_list, sys.argv[1]))
