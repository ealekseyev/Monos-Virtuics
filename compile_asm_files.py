import os, sys

files = sys.argv[1:len(sys.argv)-1]
o_path = sys.argv[-1]
for file in files:
    print("nasm {} -f elf -o {}".format(file, os.path.join(o_path, os.path.basename(file).replace(".s", ".o"))))
    os.system("nasm {} -f elf -o {}".format(file, os.path.join(o_path, os.path.basename(file).replace(".s", ".o"))))