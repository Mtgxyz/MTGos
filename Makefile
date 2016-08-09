all: loader.bin firm.bin
	$(MAKE) -C modules
loader.bin:
	$(MAKE) -C boot
	mv boot/loader.bin .
firm.bin:
	$(MAKE) -C kernel
	mv kernel/mtgos.elf .
	objcopy -O binary mtgos.elf mtgos.bin
	./firmlink mtgos
	rm -rf mtgos.bin
clean:
	find . -name '*.o' -delete