all:
	mkdir -p kernel/lib/arch/memory/asm
	mkdir -p kernel/lib/proc/asm
	mkdir -p kernel/lib/interrupts/asm kernel/lib/interrupts/sycall kernel/lib/interrupts/syscall/asm
	mkdir -p kernel/lib/drivers/ps2/asm
	mkdir -p kernel/lib/protection
	mkdir -p kernel/lib/protection/asm
	mkdir -p kernel/lib/userspace
	mkdir -p kernel/lib/proc kernel/lib/proc/asm
	mkdir -p kernel/lib/interrupts/syscall kernel/lib/interrupts/syscall/asm
	bash builduserspace.sh
	mv *.o kernel/lib/userspace/
	cd gnu-efi/; make; make bootloader; cd ../; cd kernel; make; make buildimg
	@ echo -e "\n\nNOTE FOR DEVELOPERS WORKING ON THIS: Please run python3 check.py if you are a developer.\n\n"

run:
	cd kernel/; make debug

dump:
	objdump -d -M intel kernel/bin/kernel.elf
