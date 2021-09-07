import os, sys
# compile kernel asm's
for root, dirs, files in os.walk(sys.argv[1]):
    for file in files:
        if file.endswith(".s"):
            filepath = os.path.join(root, "")
            file_complete = os.path.join(root, file)
            print("nasm {} -f elf -o {}".format(file_complete, os.path.join(sys.argv[2], file.replace(".s", ".o"))))
            os.system("nasm {} -f elf -o {}".format(file_complete, os.path.join(sys.argv[2], file.replace(".s", ".o"))))