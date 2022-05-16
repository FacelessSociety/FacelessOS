for i in $(find userspace -name "*.c"); do
	gcc -c -ffreestanding -fshort-wchar -mgeneral-regs-only $i -I userspace
done

for i in $(find userspace -name "*.asm"); do
	nasm -felf64 $i 
done

for i in $(find userspace -name "*.o"); do
	mv $i .
done
