import os, sys
# compile kernel asm's
file_list = ""
linker_file = sys.argv[2]
for dir in sys.argv[3::]:
    for root, dirs, files in os.walk(dir):
        for file in files:
            if file.endswith(".o"):
                file_list += os.path.join(root, file) + " "
print("\n\nld -m elf_i386 -nostdlib -T {} {} -o {} --oformat binary".format(linker_file, file_list, sys.argv[1]))
#os.system("ld -m elf_i386 -nostdlib -T {} {} -o {} --oformat binary".format(linker_file, file_list, sys.argv[1]))
