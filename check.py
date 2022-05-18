from os import system
import re

system("make dump > contents.txt")
with open("contents.txt", "r") as f:
    a = f.readlines()

KERNEL_END = re.findall(r"[0-9a-f]+", a[-1].strip(' '))[0]

print(f"\n\n\033[1;31;40mNOTE FOR DEVELOPER: BEFORE A GIT COMMIT, PLEASE SET KERNEL_RESERVED_END MACRO TO: 0x{KERNEL_END.upper()}\nALSO PLEASE EXECUTE: (make dump | vim -c \"/kernel_reserved\") AND GET THE FIRST LABEL ADDRESS AND SET KERNEL_RESERVED_START MACRO TO IT.\033[0m\n\n\n")
system("rm contents.txt")
