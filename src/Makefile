CC = i686-elf-gcc
ASM = nasm
LD = i686-elf-ld

CFLAGS = -m32 -ffreestanding -nostdlib -nostdinc -fno-pie -fno-stack-protector
ASMFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T linker.ld

OBJS = boot.o kernel.o idt.o vga.o isr.o pic.o gdt.o keyboard.o shell.o memory.o paging.o

all: hadron.iso

boot.o: boot.asm
	$(ASM) $(ASMFLAGS) boot.asm -o boot.o

gdt.o: gdt.asm
	$(ASM) $(ASMFLAGS) gdt.asm -o gdt.o

isr.o: isr.asm
	$(ASM) $(ASMFLAGS) isr.asm -o isr.o

idt.o: idt.c idt.h
	$(CC) $(CFLAGS) -c idt.c -o idt.o

vga.o: vga.c vga.h
	$(CC) $(CFLAGS) -c vga.c -o vga.o

pic.o: pic.c pic.h
	$(CC) $(CFLAGS) -c pic.c -o pic.o

paging.o: paging.c paging.h
	$(CC) $(CFLAGS) -c paging.c -o paging.o

keyboard.o: keyboard.c keyboard.h
	$(CC) $(CFLAGS) -c keyboard.c -o keyboard.o

memory.o: memory.c memory.h
	$(CC) $(CFLAGS) -c memory.c -o memory.o

shell.o: shell.c shell.h
	$(CC) $(CFLAGS) -c shell.c -o shell.o

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

hadron.bin: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o hadron.bin

hadron.iso: hadron.bin
	mkdir -p iso/boot/grub
	cp hadron.bin iso/boot/
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo 'menuentry "Hadron OS" {' >> iso/boot/grub/grub.cfg
	echo '    multiboot /boot/hadron.bin' >> iso/boot/grub/grub.cfg
	echo '    boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	i686-elf-grub-mkrescue -o hadron.iso iso

run: hadron.iso
	qemu-system-i386 -cdrom hadron.iso

clean:
	rm -f *.o *.bin *.iso
	rm -rf iso