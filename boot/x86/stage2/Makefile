CC=/opt/bin/i686-mtgos-gcc
LD=/opt/bin/i686-mtgos-ld
CFLAGS16=-m16
ASFLAGS=-f elf
core.img: start.o init.o
	$(LD) -T stage2.ld -o core.img start.o init.o

%.o: %.c
	$(CC) $(CFLAGS16) -o $@ $^

%.o: %.S
	nasm $(ASFLAGS) -o $@ $^