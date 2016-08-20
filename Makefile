all: loader.bin mtgos.firm
	$(MAKE) -C modules
loader.bin:
	$(MAKE) -C boot
	mv boot/loader.bin .
mtgos.firm:
	$(MAKE) -C kernel
	mv kernel/mtgos.elf .
	objcopy -O binary mtgos.elf mtgos.bin
	./firmlink mtgos
	rm -rf mtgos.bin
clean:
	find . -name '*.o' -delete
