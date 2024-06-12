run : boot.img
	qemu-system-x86_64 -drive file=boot.img,format=raw

Boot.img : Boot.asm
	nasm -f bin -o Boot.img Boot.asm

clean :
	rm *.img
